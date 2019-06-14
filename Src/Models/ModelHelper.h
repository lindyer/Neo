#pragma once

#include <QHash>
#include <QByteArray>
#include <QMetaEnum>

namespace neo {
	
	//for model,transform Q_ENUM to rolenames
	template<typename _Enum> 
	QHash<int, QByteArray> makeEnumToRoleNames() {
		QMetaEnum me = QMetaEnum::fromType<_Enum>();
		QHash<int, QByteArray> retHash;
		for (int i = 0; i < me.keyCount(); i++) {
			retHash.insert(me.value(i), me.key(i));
		}
		return retHash;
	}

	QHash<int, QByteArray> defaultTableRoleNames();

	QHash<int, QByteArray> defaultListRoleNames();

}
