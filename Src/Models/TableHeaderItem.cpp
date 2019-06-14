#include "TableHeaderItem.h"
#include "TableHeaderItem_p.h"
#include "Common/LazyActor.h"

namespace neo {
	
	NEO_QML_TYPE_REGISTER_DEFINE("Neo.Quick",TableHeaderItem)
	TableHeaderItem::TableHeaderItem(QObject* parent) : QObject(parent), d_ptr(new TableHeaderItemPrivate()) {
	}


	TableHeaderItem::TableHeaderItem(const QString& title, qreal itemWidth, qreal titleFontSize, QObject* parent) : QObject(parent), d_ptr(new TableHeaderItemPrivate()) {
		setTitle(title);
		setItemWidth(itemWidth); 
		setTitleFontSize(titleFontSize);
	}


	TableHeaderItem::~TableHeaderItem() {
		Q_D(TableHeaderItem);
		delete d;
	}


	int TableHeaderItem::index() const {
		const Q_D(TableHeaderItem);
		return d->index;
	}


	void TableHeaderItem::setIndex(int index) {
		Q_D(TableHeaderItem);
		if (index == d->index) {
			return;
		}
		d->index = index;
		indexChanged(index);
	}


	QString TableHeaderItem::title()  const {
		const Q_D(TableHeaderItem);
		return d->title;
	}


	void TableHeaderItem::setTitle(const QString& title) {
		Q_D(TableHeaderItem);
		if (title == d->title) {
			return;
		}
		d->title = title;
		emit titleChanged(title);

		emit saveChange(d->index, "title", title);
	}


	int TableHeaderItem::itemWidth() const {
		const Q_D(TableHeaderItem);
		return d->itemWidth;
	}


	void TableHeaderItem::setItemWidth(int itemWidth) {
		Q_D(TableHeaderItem);
		if (itemWidth == d->itemWidth) {
			return;
		}
		d->itemWidth = itemWidth;
		emit itemWidthChanged(itemWidth);

		int index = d->index;
		LazyActor::instance().exec(int(this), [this, index, itemWidth]() {
			emit saveChange(index, "width", itemWidth);
			});
	}


	bool TableHeaderItem::sortable() const {
		const Q_D(TableHeaderItem);
		return d->sortable;
	}


	void TableHeaderItem::setSortable(bool sortable) {
		Q_D(TableHeaderItem);
		if (sortable == d->sortable) {
			return;
		}
		d->sortable = sortable;
		emit sortableChanged(sortable);
		emit saveChange(d->index, "sortable", sortable);
	}


	QString TableHeaderItem::toolTip() const {
		const Q_D(TableHeaderItem);
		return d->toolTip;
	}


	void TableHeaderItem::setToolTip(const QString& toolTip) {
		Q_D(TableHeaderItem);
		if (toolTip == d->toolTip) {
			return;
		}
		d->toolTip = toolTip;
		emit toolTipChanged(toolTip);
		emit saveChange(d->index, "toolTip", toolTip);
	}


	int TableHeaderItem::titleFontSize() const {
		const Q_D(TableHeaderItem);
		return d->titleFontSize;
	}


	void TableHeaderItem::setTitleFontSize(int titleFontSize) {
		Q_D(TableHeaderItem);
		if (titleFontSize == d->titleFontSize) {
			return;
		}
		d->titleFontSize = titleFontSize;
		emit titleFontSizeChanged(titleFontSize);
		emit saveChange(d->index, "titleFontSize", titleFontSize);
	}


	bool TableHeaderItem::visible() const {
		const Q_D(TableHeaderItem);
		return d->visible;
	}


	void TableHeaderItem::setVisible(bool visible) {
		Q_D(TableHeaderItem);
		if (visible == d->visible) {
			return;
		}
		d->visible = visible;
		emit visibleChanged(visible);
		emit saveChange(d->index, "visible", visible);
	}


	bool TableHeaderItem::movable() const {
		const Q_D(TableHeaderItem);
		return d->movable;
	}


	void TableHeaderItem::setMovable(bool movable) {
		Q_D(TableHeaderItem);
		if (d->movable == movable) {
			return;
		}
		d->movable = movable;
		emit movableChanged(movable);
		emit saveChange(d->index, "movable", movable);
	}


	bool TableHeaderItem::resizable() const {
		const Q_D(TableHeaderItem);
		return d->resizable;
	}


	void TableHeaderItem::setResizable(bool resizable) {
		Q_D(TableHeaderItem);
		if (d->resizable == resizable) {
			return;
		}
		d->resizable = resizable;
		emit resizableChanged(resizable);
		emit saveChange(d->index, "resizable", resizable);
	}


	int TableHeaderItem::minWidth() const {
		const Q_D(TableHeaderItem);
		return d->minWidth;
	}


	void TableHeaderItem::setMinWidth(int minWidth) {
		Q_D(TableHeaderItem);
		if (minWidth == d->minWidth) {
			return;
		}
		d->minWidth = minWidth;
		emit minWidthChanged(minWidth);
		emit saveChange(d->index, "minWidth", minWidth);
	}


	int TableHeaderItem::titleHorizontalAlignment() const {
		const Q_D(TableHeaderItem);
		return d->titleHorizontalAlignment;
	}


	void TableHeaderItem::setTitleHorizontalAlignment(int titleHorizontalAlignment) {
		Q_D(TableHeaderItem);
		if (titleHorizontalAlignment == d->titleHorizontalAlignment) {
			return;
		}
		d->titleHorizontalAlignment = titleHorizontalAlignment;
		emit titleHorizontalAlignmentChanged(titleHorizontalAlignment);
		emit saveChange(d->index, "titleHorizontalAlignment", titleHorizontalAlignment);
	}


	QJsonValue TableHeaderItem::extraData() const {
		const Q_D(TableHeaderItem);
		return d->extraData;
	}


	void TableHeaderItem::setExtraData(const QJsonValue& extraData) {
		Q_D(TableHeaderItem);
		if (extraData == d->extraData) {
			return;
		}
		d->extraData = extraData;
		emit extraDataChanged(extraData);
		emit saveChange(d->index, "extraData", extraData);
	}


	QString TableHeaderItem::field() const {
		const Q_D(TableHeaderItem);
		return d->field;
	}


	void TableHeaderItem::setField(const QString& field) {
		Q_D(TableHeaderItem);
		if (field == d->field) {
			return;
		}
		d->field = field;
	}


}
