#pragma once

#include <QObject>
#include <functional>

class LogReporter : public QObject
{
	Q_OBJECT


private:
	LogReporter(QObject *parent = nullptr);
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
	static void setProxy(std::function<void(const QString &, Level level)> handler);

public slots:
	static void report(const QString &log, Level level = InfoLevel);

private:
	static QString _levelDescription(Level level) ;

private:
	static std::function<void(const QString &, Level level)> _proxy;
	static QString _username;
};
