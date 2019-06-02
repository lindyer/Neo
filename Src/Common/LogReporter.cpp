#include "LogReporter.h"  

#include <qmetaobject.h>
#include <qdebug.h>
#include <qhostinfo.h>
#include "utilities/HttpClient.h"

class LogReporterPrivate { 
public:
	LogReporterPrivate(const QString& serverUrl, const QString& username);
	QString serverUrl;
	std::function<void(const QString&, LogReporter::Level level)> proxy;
	QString username;
	QString levelDescription(LogReporter::Level level);
};


LogReporterPrivate::LogReporterPrivate(const QString& serverUrl, const QString& username)
	:serverUrl(serverUrl),username(username) {

}


QString LogReporterPrivate::levelDescription(LogReporter::Level level) {
	QMetaEnum me = QMetaEnum::fromType<LogReporter::Level>();
	return me.key(level);
}


LogReporter::LogReporter(const QString& serverUrl, const QString& username, QObject *parent)
	: QObject(parent), d_ptr(new LogReporterPrivate(serverUrl,username))
{
}

LogReporter::~LogReporter()
{
	Q_D(LogReporter);
	delete d;
}

void LogReporter::report(const QString& log, Level level) {
	Q_D(LogReporter);
	if(d->proxy) {
		d->proxy(log, level);
	} else {
		HttpClient(d->serverUrl).param("uid", QSysInfo::machineUniqueId()).param("host",QHostInfo::localHostName()).param("account", d->username).param("level", QString::number(level)).param("log", log).post(nullptr);
	}
}

void LogReporter::setProxy(std::function<void(const QString &, Level level)> &&handler) {
	Q_D(LogReporter);
	d->proxy = std::move(handler);
}
