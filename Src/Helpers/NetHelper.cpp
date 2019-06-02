#include "NetHelper.h"
#include "NetHelper.h"

#include <QStringList>

namespace Neo {
	
	namespace Net {
		
		bool isIPV4(const QString& ip) {
			QStringList sections = ip.split(".");
			if(sections.size() != 4) {
				return false;
			}
			for(auto section : sections) {
				bool isInteger = false;
				int num = section.toInt(&isInteger);
				if(!isInteger) {
					return false;
				}
				if(num < 0 || num > 255) {
					return false;
				}
			}
			return true;
		}

	}

}


