#include "NetHelper.h"
#include "NetHelper.h"

#include <QStringList>
#include <QObject>


namespace neo {

	bool isIPv4(const QString& ip) {
		QStringList sections = ip.split(".");
		if (sections.size() != 4) {
			return false;
		}
		for (auto section : sections) {
			bool isInteger = false;
			int num = section.toInt(&isInteger);
			if (!isInteger) {
				return false;
			}
			if (num < 0 || num > 255) {
				return false;
			}
		}
		return true;
	}


	bool NetHelper::isIPv4(const QString& str) const {
		return neo::isIPv4(str);
	}
		
}


