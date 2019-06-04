#pragma once

#pragma execution_character_set("utf-8")

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/


#include <QObject>

#define LOG_LOCATION QString("%2:%3 -> ").arg(__FUNCTION__).arg(__LINE__)

#ifdef USE_LOG4QT
#include "log4qt/logger.h"
#include <QFileInfo>
#include "log4qt/propertyconfigurator.h"

/**
 * @brief The LogHandler class
 * eg.
    xdebug("test",1);
    DEBUG("test",1);
    DEBUG_LOCATION();
 */

class LogHandler: public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    static LogHandler &instance();

public slots:
    virtual void debug(const QString &log) {
		logger()->debug(log);
	}
    virtual void info(const QString &log) {
		logger()->info(log);
	}
    virtual void warn(const QString &log) {
		logger()->warn(log);
	}
    virtual void error(const QString &log) {
		logger()->error(log);
	}
    virtual void fatal(const QString &log) {
		logger()->fatal(log);
	}

private:
    LogHandler(QObject *parent = nullptr) : QObject(parent) {
		QString configPath = QtCommon::workPath(AppConfig::instance().getString("Log4qt.Path"));
		Log4Qt::PropertyConfigurator::configure(configPath);
		logger()->info("################################################################");
		logger()->info("##############			Program Started		   #################");
		logger()->info("################################################################");
	}
    ~LogHandler() {
		logger()->info("****************************************************************");
		logger()->info("**************			Program Ended		   *****************");
		logger()->info("****************************************************************");
	}
};

#else

#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h" 

class LogHandler : public QObject
{
	Q_OBJECT
public:
	static LogHandler &instance();
	std::shared_ptr<spdlog::logger> logger_;
public slots:
	virtual void debug(const QString &log) {
		logger_->debug(log.toStdString());
	}
	virtual void info(const QString &log) {
		logger_->info(log.toStdString());
	}
	virtual void warn(const QString &log) {
		logger_->warn(log.toStdString());
	}
	virtual void error(const QString &log) {
		logger_->error(log.toStdString());
	}
	 

private:
	LogHandler(QObject *parent = nullptr): QObject(parent) {
		//https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		spdlog::flush_on(spdlog::level::info);
		spdlog::flush_every(std::chrono::seconds(3));
		spdlog::set_pattern("%L %m/%d %T.%e (%P:%t) - %v");
		logger_ = spdlog::daily_logger_mt("daily","logs/log",0,0);

		logger_->info("##############################################################");
		logger_->info("##############       Program  Started        #################");
		logger_->info("##############################################################");
	}
	~LogHandler() {
		logger_->info("**************************************************************");
		logger_->info("*************       Program   Ended		        *************");
		logger_->info("**************************************************************");
	}
};

//直接走spd
//spdI("Format Date: {}/{}/{}",2019,5,20);
#define spdD(fmt,...) LogHandler::instance().logger_->debug(fmt,##__VA_ARGS__);
#define spdI(fmt,...) LogHandler::instance().logger_->info(fmt,##__VA_ARGS__);
#define spdW(fmt,...) LogHandler::instance().logger_->warn(fmt,##__VA_ARGS__);
#define spdE(fmt,...) LogHandler::instance().logger_->error(fmt,##__VA_ARGS__);
#define spdF(fmt,...) LogHandler::instance().logger_->fatal(fmt,##__VA_ARGS__);

#endif


inline QString LogHandlerTemplateParameter() {
	return QString();
}

template <typename T, typename ... Args>
QString LogHandlerTemplateParameter(T head, Args... args) { return QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...); }

template <typename T, typename ... Args>
void xdebug(T head, Args ... args) {
	QString log = QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...);
	LogHandler::instance().debug(log);
}

template <typename T, typename... Args>
void xinfo(T head, Args... args) {
	QString log = QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...);
	LogHandler::instance().info(log);
}

template <typename T, typename... Args>
void xwarn(T head, Args... args) {
	QString log = QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...);
	LogHandler::instance().warn(log);
}

template <typename T, typename... Args>
void xerror(T head, Args... args) {
	QString log = QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...);
	LogHandler::instance().error(log);
}

template <typename T, typename... Args>
void xfatal(T head, Args... args) {
	QString log = QString("%1 ").arg(head) + LogHandlerTemplateParameter(args...);
	LogHandler::instance().fatal(log);
}

#define DEBUG_LOCATION() xdebug(LOG_LOCATION);
#define INFO_LOCATION() xinfo(LOG_LOCATION);
#define WARN_LOCATION() xwarn(LOG_LOCATION);
#define ERROR_LOCATION() xerror(LOG_LOCATION);
#define FATAL_LOCATION() xfatal(LOG_LOCATION);

//Qt通用
#define DEBUG(...) xdebug(LOG_LOCATION,##__VA_ARGS__);
#define INFO(...) xinfo(LOG_LOCATION,##__VA_ARGS__);
#define WARN(...) xwarn(LOG_LOCATION,##__VA_ARGS__);
#define ERROR(...) xerror(LOG_LOCATION,##__VA_ARGS__);
#define FATAL(...) xfatal(LOG_LOCATION,##__VA_ARGS__);

