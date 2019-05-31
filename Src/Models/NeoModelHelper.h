#pragma once

#include <QHash>
#include <QByteArray>
#include <QMetaEnum>


template<typename _Enum>
static QHash<int, QByteArray> makeEnumToRoleNames() {
	QMetaEnum me = QMetaEnum::fromType<_Enum>();
	QHash<int, QByteArray> retHash;
	for (int i = 0; i < me.keyCount(); i++) {
		retHash.insert(me.value(i), me.key(i));
	}
	return retHash;
}

static QHash<int, QByteArray> defaultTableRoleNames() {
	static QHash<int, QByteArray> hash{
		{Qt::UserRole + 1, "modelData"}
	};
	return hash;
}

static QHash<int, QByteArray> defaultListRoleNames() {
	static QHash<int, QByteArray> hash{
		{Qt::UserRole + 1, "modelData"}
	};
	return hash;
}