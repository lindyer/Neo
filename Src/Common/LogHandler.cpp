/***
**  @Filename:  LogHandler.cpp
**  @Creator: linjianpeng
**  @CreateDate: 2019-1-9
**  @Brief:
**  @ReviseHistory:
**/

#include "LogHandler.h"

LogHandler &LogHandler::instance()
{
    static LogHandler handler;
    return handler;
}
