#pragma once 

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#endif


#include <QObject>
#include <QThread>
#include <functional>
#include <QTimer>
#include "Ping.h"
class QHostInfo;


namespace neo {

	class Ping;
	class PingWorker;

	constexpr int PingRecvBufferSize = 256;
	constexpr int IPHeaderSize = 20;

	class PingPrivate {
		Q_DECLARE_PUBLIC(Ping);
	public:
		explicit PingPrivate(Ping* parent);
		explicit PingPrivate(const QString& hostIP, int interval, bool running, Ping* parent);
		~PingPrivate();

		PingWorker* pingWorker = nullptr;

		Ping* const q_ptr;

		QThread pingThread;  //all ping share one same thread
	};

	class PingWorker : public QObject {
		Q_OBJECT
			Q_DISABLE_COPY(PingWorker)
	public:
		PingWorker(Ping* parent);
		~PingWorker();

	public slots:
		void parseHost(const QString& host);
		void doWork();
		void start(int interval = 2000);
		void stop();
		void setClassifyFn(std::function<Ping::NetworkTraffic(int)>);
		bool isReady();

	private slots:
		void hostLookedup(const QHostInfo&);

	public:
		bool ready = false;
		//domain or ip
		QString host;
		QString hostIP;
		int interval = 1000;
		bool running = false;
		int sequence = 0;
		quint16 id = 0;
		char recvBuffer[PingRecvBufferSize]{};
		SOCKET socket = 0;
		SOCKADDR_IN srcSockAddr{};
		std::function<Ping::NetworkTraffic(int)> classifyFn;

		Ping* const ping;
		QTimer* timer = nullptr;

		Ping::NetworkTraffic defaultClassify(int delay);
		quint16 checksum(quint16* addr, int len);
		quint32 timestamp();
	};
}


