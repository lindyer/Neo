#include "Ping.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#endif

#include <QDateTime>
#include <functional>
#include <QThread>
#include <qtimer.h>
#include "Ping_p.h"

namespace Neo {
	namespace Net {

		constexpr int PingRecvBufferSize = 128;
		constexpr int IPHeaderSize = 20;
		QThread PingPrivate::pingThread;

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


		Ping::Ping(QObject* parent): QObject(parent), d_ptr(new PingPrivate(this)){
		}


		Ping::Ping(const QString& hostIP, QObject* parent)
			: QObject(parent), d_ptr(new PingPrivate(hostIP,this)) {
		}

		PingPrivate::PingPrivate(const QString& hostIP, Ping* parent)
			: q_ptr(parent) {
			pingWorker = new PingWorker(hostIP,parent);
			QObject::connect(&pingThread, &QThread::finished, pingWorker, &QObject::deleteLater);
		}


		PingPrivate::~PingPrivate() {
			pingThread.quit();
			pingThread.wait();
		}


		void PingPrivate::start(int interval) {
			pingWorker->start(interval);
		}


		Ping::~Ping() {
			Q_D(Ping);
			delete d;
		}


		void Ping::setClassifyFn(std::function<NetworkTraffic(int)> fn) {
			Q_D(Ping);
			d->pingWorker->setClassifyFn(fn);
		}


		bool Ping::isReady() {
			Q_D(Ping);
			return d->pingWorker->isReady();
		}


		void Ping::start(int interval) {
			Q_D(Ping);
			d->start();
		}


		void Ping::stop() {
			Q_D(Ping);
			d->pingWorker->stop();
		}


		class PingWorkerPrivate {
		public:
			PingWorkerPrivate(PingWorker *parent, Ping *ping) :ping(ping), q_ptr(parent)  { 
			}
			~PingWorkerPrivate() {
				::closesocket(socket);
				::WSACleanup();
			}
			bool ready = false;
			QString hostIP;
			int interval = 1000;
			int sequence = 0;
			quint16 id = 0;
			char recvBuffer[PingRecvBufferSize];
			SOCKET socket = 0;
			SOCKADDR_IN srcSockAddr;
			std::function<Ping::NetworkTraffic(int)> classifyFn;

			QTimer timer;

			Ping::NetworkTraffic defaultClassify(int delay) {
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

			quint16 checksum(quint16* addr, int len) {
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

			quint32 timestamp() {
				return ::GetTickCount();
			}
 
			Ping *const ping;
		private:
			Q_DECLARE_PUBLIC(PingWorker)
			PingWorker * const q_ptr;
		};

		PingWorker::PingWorker(const QString &hostIP, Ping *ping): d_ptr(new PingWorkerPrivate(this,ping)) {
			Q_D(PingWorker);
			::WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
				emit d->ping->errorNotify(Ping::InitSocketFail, ::WSAGetLastError());
				return;
			}
			d->socket = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
			if(d->socket == INVALID_SOCKET) {
				closesocket(d->socket);
				return;
			}
			d->ready = true;
			::setsockopt(d->socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&d->interval), sizeof(d->interval));
			d->id = qint16(::GetCurrentProcessId());
			d->hostIP = hostIP;

			this->moveToThread(&PingPrivate::pingThread);
			PingPrivate::pingThread.start();
		}


		PingWorker::~PingWorker() {
			Q_D(PingWorker);
			delete d; 
		}

		void PingWorker::doWork() {
			Q_D(PingWorker);
			::SOCKADDR_IN destSockAddr;
			destSockAddr.sin_family = AF_INET;
			destSockAddr.sin_addr.S_un.S_addr = ::inet_addr(d->hostIP.toLatin1().constData());
			destSockAddr.sin_port = htons(0);
			ICMPHeader icmpHeader;
			int _icmpHeaderSize = sizeof(icmpHeader);
			icmpHeader.type = 8;
			icmpHeader.code = 0;
			icmpHeader.checksum = 0;
			icmpHeader.sequence = d_ptr->sequence++;
			icmpHeader.id = d_ptr->id;
			icmpHeader.timestamp = d_ptr->timestamp();
			icmpHeader.checksum = d_ptr->checksum(reinterpret_cast<quint16*>(&icmpHeader), _icmpHeaderSize);
			int ret = sendto(d_ptr->socket, reinterpret_cast<const char*>(&icmpHeader), _icmpHeaderSize, 0, reinterpret_cast<SOCKADDR*>(&destSockAddr), sizeof(destSockAddr));
			if (ret == SOCKET_ERROR) {
				emit d_ptr->ping->errorNotify(Ping::SendFail, ::WSAGetLastError());
				return;
			}
			int nLen = sizeof(SOCKADDR_IN);
			ret = recvfrom(d_ptr->socket, d_ptr->recvBuffer, PingRecvBufferSize, 0, reinterpret_cast<::SOCKADDR*>(&d->srcSockAddr), &nLen);
			if (ret == SOCKET_ERROR) {
				emit d->ping->errorNotify(Ping::RecvFail, ::WSAGetLastError());
				return;
			}
			auto pRcvHeader = reinterpret_cast<ICMPHeader*>(d->recvBuffer + IPHeaderSize);

			int elaspe = d->timestamp() - pRcvHeader->timestamp;
			Ping::NetworkTraffic nt;
			if (d->classifyFn) {
				nt = d->classifyFn(elaspe);
			} else {
				nt = d->defaultClassify(elaspe);
			}
			d->ping->networkTrafficNotify(nt, elaspe);
		}


		void PingWorker::start(int interval) {
			Q_D(PingWorker);
			if(!d->ready) {
				qWarning("PingWorker is not ready,can not be started");
				return;
			}
			connect(&d->timer, &QTimer::timeout, this, &PingWorker::doWork,Qt::UniqueConnection);
 			d->timer.start(interval);
		}


		void PingWorker::stop() {
			Q_D(PingWorker);
			d->timer.stop();
		}


		void PingWorker::setClassifyFn(std::function<Ping::NetworkTraffic(int)> fn) {
			Q_D(PingWorker);
			d->classifyFn = fn;
		}


		bool PingWorker::isReady() {
			Q_D(PingWorker);
			return d->ready;
		}
	} // Net
} // Neo
