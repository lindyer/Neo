#pragma once 

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QObject>
#include <QThread>
#include <functional>
#include "Ping.h"

namespace Neo {
	namespace Net {
		class PingWorkerPrivate;
		class Ping;
		class PingWorker;

		class PingPrivate {
		public:
			explicit PingPrivate(Ping *parent) : q_ptr(parent) { }
			explicit PingPrivate(const QString& hostIP, Ping *parent);
			~PingPrivate();
			void start(int interval = 2000);
			PingWorker *pingWorker = nullptr;
			static QThread pingThread;
			Ping *const q_ptr;
			Q_DECLARE_PUBLIC(Ping);
		};

		class PingWorker : public QObject {
			Q_OBJECT
			Q_DECLARE_PRIVATE(PingWorker)
			Q_DISABLE_COPY(PingWorker)
		public:
			PingWorker(const QString &hostIP,Ping *parent);
			~PingWorker();
		public slots:
			void doWork();
			void start(int interval = 2000);
			void stop();
			void setClassifyFn(std::function<Ping::NetworkTraffic(int)>);
			bool isReady();
		private:
			PingWorkerPrivate * const d_ptr;
		};
	}
}


