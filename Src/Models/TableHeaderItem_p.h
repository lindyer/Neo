#pragma once
#include <QString>
#include <QJsonValue>

namespace neo {

	class TableHeaderModel;

	class TableHeaderItemPrivate {
	public:
		int index = -1;
		QString title;
		int itemWidth = 80;
		bool sortable = false;
		QString toolTip;
		int titleFontSize = 14;
		bool visible = true;
		bool movable = true;
		bool resizable = true;
		int minWidth = 40;
		int titleHorizontalAlignment = Qt::AlignCenter;
		QJsonValue extraData;
		QString field;
	};


}
