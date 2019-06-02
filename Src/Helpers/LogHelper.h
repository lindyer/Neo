#pragma once
#include <QString>
#include "Common/LogReporter.h"


namespace Neo {
	
	void defaultLogReporter(const QString& log, LogReporter::Level level, const QString& serverUrl = "serverUrl", const QString& username = "");

}
