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
#include <QQmlParserStatus>


namespace neo {

	/*
				QML qmlRegisterType<neo::net::Ping>("Neo.Net", 1, 0, "NeoPing");					NeoPing {
						host: "taobao.com"
						interval: 1000
						running: true
						onNetworkTrafficNotify: {
							print("taobao",nt,delayMs)
						}
					}
				 C++
					Neo::net::Ping ping("220.181.38.149");
					QObject::connect(&ping, &neo::net::Ping::networkTrafficNotify, [](neo::net::Ping::NetworkTraffic nt, int delayMs) {
						qDebug() << nt << delayMs;
					});
				 */
	
	class PingPrivate;
	class Ping : public QObject, public QQmlParserStatus
	{
		Q_OBJECT
			Q_INTERFACES(QQmlParserStatus)
			Q_DISABLE_COPY(Ping)

			Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
			Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
			Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
	public:
		explicit Ping(QObject* parent = nullptr);
		explicit Ping(const QString& host, int interval = 1000, bool running = true, QObject * parent = nullptr);
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

		void classBegin() override;

		void componentComplete() override;

	signals:
		void hostChanged(const QString& host);
		void intervalChanged(int interval);
		void runningChanged(bool running);

	public:
		QString host() const;
		void setHost(const QString& host);
		int interval() const;
		void setInterval(int interval);
		bool running() const;
		void setRunning(bool running);

	public slots:
		bool isReady();

		void restart(int interval = 1000);

		void stop();

	signals:
		void errorNotify(Error error, int errCode);
		void networkTrafficNotify(NetworkTraffic nt, int delayMs);

	private:
		Q_DECLARE_PRIVATE(Ping)
			PingPrivate* const d_ptr;
	};

} // Neo


