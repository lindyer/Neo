#include "PrintHelper.h"
#include <QHash>

namespace neo {
	QDebug PrintHelper::levelSelector(PrintHelper::Level level) {
		switch (level) {
		case Debug: return qDebug();
		case Info: return qInfo();
		case Warning: return qWarning();
		case Critical: return qCritical();
		case Fatal: return qCritical(); //qFatal() -> void, use qCritical replace
		default: return qDebug();
		}
	}


}