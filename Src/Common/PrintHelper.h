#pragma once

#include "ConvertHelper.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>


namespace neo {
	class PrintHelper : public QObject {
	public:
		enum Level {
			Debug,
			Info,
			Warning,
			Critical,
			Fatal
		};
		Q_ENUM(Level)

	public:
		static QDebug levelSelector(PrintHelper::Level);
	};


	inline void prettyPrintStringList(const QStringList& strList,PrintHelper::Level level = PrintHelper::Debug) {
		PrintHelper::levelSelector(level) << strListToPrettyStr(strList).data();
	}

}


