#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QObject>
#include <functional>

#include <QDebug>

namespace Neo {

	namespace Net {

		/*
		 Neo::Net::Ping ping("220.181.38.149");
		qDebug() << ping.isReady(); 
		QObject::connect(&ping, &Neo::Net::Ping::errorNotify, [](Neo::Net::Ping::Error err,int errCode) {
			qDebug() << err << errCode;
		});
		QObject::connect(&ping, &Neo::Net::Ping::networkTrafficNotify, [](Neo::Net::Ping::NetworkTraffic nt, int delayMs) {
			qDebug() << nt << delayMs;
		});
		ping.start();
		 */
		class PingPrivate; 
		class Ping final: public QObject
		{
			Q_OBJECT
			Q_DECLARE_PRIVATE(Ping)
			Q_DISABLE_COPY(Ping)
		public:
			explicit Ping(QObject* parent = nullptr);
			explicit Ping(const QString& hostIP, QObject* parent = nullptr); 
			~Ping();
			enum Error {
				InitSocketFail,
				SendFail,
				RecvFail
			};
			Q_ENUM(Error)

			enum NetworkTraffic {
				Terrible,
				Bad,
				Good,
				Excellent,
			};
			Q_ENUM(NetworkTraffic)
				 
			/*
			 * if no set,default <1:Excellent,<50:Good,<200:Bad,<500:Terrible
			 */
			void setClassifyFn(std::function<NetworkTraffic(int)>);
			
			bool isReady();

			void start(int interval = 2000);

			void stop();

		signals:
			void errorNotify(Error error,int errCode);
			void networkTrafficNotify(NetworkTraffic nt, int delayMs);

		private:
			PingPrivate* const d_ptr;
		};


	} // Network
} // Neo


