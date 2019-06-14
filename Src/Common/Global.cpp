#include "Global.h"


namespace neo {
	
	QList<std::function<void()>>& qmlTypeRegisterList() {
		static QList<std::function<void()>> list;
		return list;
	}


	void registerQmlTypeList() {
		for (auto functor : qmlTypeRegisterList()) {
			functor();
		}
	}

}
