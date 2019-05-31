#include "LogReporter.h"
#include "HttpClient.h"
#include "AppConfig.h"

#include <qmetaobject.h>
#include <qdebug.h>
#include <qhostinfo.h>

QString LogReporter::_username;
std::function<void(const QString &, LogReporter::Level level)> LogReporter::_proxy;
LogReporter::LogReporter(QObject *parent)
	: QObject(parent)
{
}

LogReporter::~LogReporter()
{
}

void LogReporter::report(const QString& log, Level level) {
	if(_proxy) {
		_proxy(log, level);
	} else {
		static QString defaultUrl = AppConfig::instance().getString("LogUrl");
		HttpClient(defaultUrl).param("uid", QSysInfo::machineUniqueId()).param("host",QHostInfo::localHostName()).param("account", _username).param("level", QString::number(level)).param("log", log).post(nullptr);
	}
}

void LogReporter::setProxy(std::function<void(const QString &, Level level)> handler) {
	_proxy = handler;
}

QString LogReporter::_levelDescription(Level level) {

	QMetaEnum me = QMetaEnum::fromType<Level>();
	return me.key(level);
	//switch (level) {
	//case Debug: return "Debug";
	//case Info: return "Info";
	//case Warn: return "Warn";
	//case Critical: return "Critical";
	//case Fatal: return "Fatal";
	//}
	//return QString();
}
