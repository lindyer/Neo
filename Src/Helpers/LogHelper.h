#pragma once
#include <QString>
#include "Common/LogReporter.h"


namespace neo {
	
	void defaultLogReporter(const QString& log, LogReporter::Level level, const QString& serverUrl = "serverUrl", const QString& username = "");

}
