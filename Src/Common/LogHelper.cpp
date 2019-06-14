#include "LogHelper.h"
#include "Common/LogReporter.h"


namespace neo {

	void defaultLogReporter(const QString& log,LogReporter::Level level, const QString& serverUrl, const QString& username) {
		static LogReporter logReporter(serverUrl,username);
		logReporter.report(log,level);
	}

}


