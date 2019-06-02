#pragma once

#include <QObject>
#include <functional>

class LogReporterPrivate;
class LogReporter : public QObject
{
	Q_OBJECT

public:
	LogReporter(const QString &serverUrl, const QString& username,QObject *parent = nullptr);
	~LogReporter();	
public:
	enum Level {
		PanicLevel,
		FatalLevel,
		ErrorLevel,
		WarnLevel,
		InfoLevel,
		DebugLevel,
		TraceLevel,
	};

	Q_ENUM(Level)
	void setProxy(std::function<void(const QString &, Level level)> && handler);

public slots:
	void report(const QString &log, Level level = InfoLevel);

private:
	Q_DECLARE_PRIVATE(LogReporter)
	LogReporterPrivate* d_ptr = nullptr;
};
