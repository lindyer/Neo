#include "Ping.h"


#include <QDateTime>
#include <functional>
#include <QThread>
#include <qtimer.h>
#include "Ping_p.h"
#include <qhostinfo.h>
#include <qqml.h>
#include "Network/NetHelper.h"


namespace neo {

	class ICMPHeader {
	public:
		qint8 type = 0;				//类型
		qint8 code = 0;				//代码
		quint16 checksum = 0;		//校验和
		quint16 id = 0;				//进程ID
		quint16 sequence = 0;		//序号
		quint32 timestamp = 0;		//时间
		QString toString() {
			return QString("type=%1,code=%2,checksum=%3,id=%4,sequence=%5,timestamp=%6").arg(type).arg(code).arg(checksum).arg(id).arg(sequence).arg(timestamp);
		}
	};


	Ping::Ping(QObject* parent) : d_ptr(new PingPrivate(this)) {
		qRegisterMetaType<NetworkTraffic>("NetworkTraffic");
	}


	Ping::Ping(const QString& host, int interval, bool running, QObject* parent)
		: QObject(parent), d_ptr(new PingPrivate(host, interval, running, this)) {
	}


	PingPrivate::PingPrivate(Ping* parent) : pingWorker(new PingWorker(parent)), q_ptr(parent) {
		QObject::connect(&pingThread, &QThread::finished, pingWorker, &QObject::deleteLater);
		pingWorker->moveToThread(&pingThread);
		if (!PingPrivate::pingThread.isRunning()) {
			PingPrivate::pingThread.start();
		}
	}


	PingPrivate::PingPrivate(const QString& host, int interval, bool running, Ping* parent)
		: PingPrivate(parent) {
		pingWorker->interval = interval;
		pingWorker->running = running;
		pingWorker->parseHost(host);
	}


	PingPrivate::~PingPrivate() {
		pingThread.quit();
		pingThread.wait();
	}


	Ping::~Ping() {
		Q_D(Ping);
		delete d;
	}


	void Ping::setClassifyFn(std::function<NetworkTraffic(int)> fn) {
		Q_D(Ping);
		d->pingWorker->setClassifyFn(fn);
	}


	void Ping::classBegin() {
	}


	void Ping::componentComplete() {
	}


	QString Ping::host() const {
		const Q_D(Ping);
		return d->pingWorker->host;
	}


	void Ping::setHost(const QString& host) {
		Q_D(Ping);
		if (d->pingWorker->host == host) {
			return;
		}
		d->pingWorker->parseHost(host);
	}


	int Ping::interval() const {
		const Q_D(Ping);
		return d->pingWorker->interval;
	}


	void Ping::setInterval(int interval) {
		Q_D(Ping);
		if (interval == d->pingWorker->interval) {
			return;
		}
		d->pingWorker->start(interval);
	}


	bool Ping::running() const {
		const Q_D(Ping);
		return d->pingWorker->running;
	}


	void Ping::setRunning(bool running) {
		Q_D(Ping);
		if (running == d->pingWorker->running) {
			return;
		}
		d->pingWorker->running = running;
		if (running) {
			d->pingWorker->start(d->pingWorker->interval);
		}
		else {
			d->pingWorker->stop();
		}
	}


	bool Ping::isReady() {
		Q_D(Ping);
		return d->pingWorker->isReady();
	}


	void Ping::restart(int interval) {
		Q_D(Ping);
		d->pingWorker->start(interval);
	}

	void Ping::stop() {
		Q_D(Ping);
		d->pingWorker->stop();
	}

	PingWorker::PingWorker(Ping* ping) : ping(ping), timer(new QTimer(this)) {
		::WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			emit ping->errorNotify(Ping::InitSocketFail, ::WSAGetLastError());
			return;
		}
		socket = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
		if (socket == INVALID_SOCKET) {
			closesocket(socket);
			return;
		}
		ready = true;
		::setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&interval), sizeof(interval));
		id = qint16(::GetCurrentProcessId());
	}


	PingWorker::~PingWorker() {
		::closesocket(socket);
		::WSACleanup();
		timer->deleteLater();
	}


	void PingWorker::parseHost(const QString& host) {
		//wait host domain parse complete,then start timer to ping
		ready = false;
		if (isIPv4(host)) {
			ready = true;
			this->host = host;
			hostIP = host;
			if (running) {
				start(interval);
			}
		}
		else {
			QHostInfo::lookupHost(host, this, &PingWorker::hostLookedup);
		}
	}


	void PingWorker::doWork() {
		if (!ready) {
			return;
		}
		::SOCKADDR_IN destSockAddr;
		destSockAddr.sin_family = AF_INET;
		destSockAddr.sin_addr.S_un.S_addr = ::inet_addr(hostIP.toLatin1().constData());
		destSockAddr.sin_port = htons(0);
		ICMPHeader icmpHeader;
		int _icmpHeaderSize = sizeof(icmpHeader);
		icmpHeader.type = 8;
		icmpHeader.code = 0;
		icmpHeader.checksum = 0;
		icmpHeader.sequence = sequence++;
		icmpHeader.id = id;
		icmpHeader.timestamp = timestamp();
		icmpHeader.checksum = checksum(reinterpret_cast<quint16*>(&icmpHeader), _icmpHeaderSize);
		int ret = sendto(socket, reinterpret_cast<const char*>(&icmpHeader), _icmpHeaderSize, 0, reinterpret_cast<SOCKADDR*>(&destSockAddr), sizeof(destSockAddr));
		if (ret == SOCKET_ERROR) {
			emit ping->errorNotify(Ping::SendFail, ::WSAGetLastError());
			return;
		}
		int nLen = sizeof(SOCKADDR_IN);
		ret = recvfrom(socket, recvBuffer, PingRecvBufferSize, 0, reinterpret_cast<::SOCKADDR*>(&srcSockAddr), &nLen);
		if (ret == SOCKET_ERROR) {
			emit ping->errorNotify(Ping::RecvFail, ::WSAGetLastError());
			return;
		}
		auto pRcvHeader = reinterpret_cast<ICMPHeader*>(recvBuffer + IPHeaderSize);

		int elaspe = timestamp() - pRcvHeader->timestamp;
		Ping::NetworkTraffic nt;
		if (classifyFn) {
			nt = classifyFn(elaspe);
		}
		else {
			nt = defaultClassify(elaspe);
		}
		ping->networkTrafficNotify(nt, elaspe);
	}


	void PingWorker::start(int interval) {
		if (thread() != QThread::currentThread()) {
			QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection, Q_ARG(int, interval));
			return;
		}

		if (!ready) {
			return;
		}
		this->interval = interval;
		connect(this->timer, &QTimer::timeout, this, &PingWorker::doWork, Qt::UniqueConnection);
		this->timer->start(interval);
	}


	void PingWorker::stop() {
		if (thread() != QThread::currentThread()) {
			QMetaObject::invokeMethod(this, "stop", Qt::QueuedConnection);
			return;
		}
		timer->stop();
	}


	void PingWorker::setClassifyFn(std::function<Ping::NetworkTraffic(int)> fn) {
		classifyFn = fn;
	}


	bool PingWorker::isReady() {
		return ready;
	}


	void PingWorker::hostLookedup(const QHostInfo& hostInfo) {
		if (hostInfo.error() != QHostInfo::NoError) {
			qWarning() << "Lookup failed:" << hostInfo.errorString();
			return;
		}
		const auto addresses = hostInfo.addresses();
		if (!addresses.isEmpty()) {
			hostIP = addresses.first().toString();
			qDebug() << "Lookup HostAddress" << addresses.first().toString();
			ready = true;
			if (running) {
				start(interval);
			}
		}
	}


	Ping::NetworkTraffic PingWorker::defaultClassify(int delay) {
		if (delay < 1) {
			return Ping::Excellent;
		}
		else if (delay < 50) {
			return Ping::Good;
		}
		else if (delay < 200) {
			return Ping::Bad;
		}
		else {
			return Ping::Terrible;
		}
	}


	quint16 PingWorker::checksum(quint16* addr, int len) {
		int left = len;
		qint32 sum = 0;
		quint16* w = addr;
		qint32 answer = 0;
		while (left > 1) {
			sum += *w;
			left -= 2;
			w++;
		}
		if (left == 1) {
			*reinterpret_cast<unsigned char*>(&answer) = *reinterpret_cast<unsigned char*>(w);
			sum += answer;
		}
		sum = (sum >> 16) + (sum & 0xffff);
		sum += (sum >> 16);
		answer = ~sum;
		return answer;
	}


	quint32 PingWorker::timestamp() {
		return ::GetTickCount();
	}
		
} // Neo
