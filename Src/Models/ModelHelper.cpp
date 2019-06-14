#include "ModelHelper.h"

namespace neo {


	QHash<int, QByteArray> defaultTableRoleNames() {
		static QHash<int, QByteArray> hash{
			{Qt::UserRole + 1, "modelData"}
		};
		return hash;
	}

	QHash<int, QByteArray> defaultListRoleNames() {
		static QHash<int, QByteArray> hash{
			{Qt::UserRole + 1, "modelData"}
		};
		return hash;
	}

}

