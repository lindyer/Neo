#include "NeoTableHeaderItem.h"


class NeoTableHeaderItemPrivate {
public:
	QString title;
	qreal itemWidth = 80;
	bool sortable = false;
	QString toolTip;
	qreal titleFontSize = 14;
	bool visible = true;
	bool movable = true;
	bool resizable = true;
	int minWidth = 40;
	int titleHorizontalAlignment = Qt::AlignCenter;
	QVariant extraData;
};

NeoTableHeaderItem::NeoTableHeaderItem(QObject* parent): QObject(parent), d_ptr(new NeoTableHeaderItemPrivate()){
}


NeoTableHeaderItem::NeoTableHeaderItem(const QString& title, qreal itemWidth, qreal titleFontSize, QObject* parent): QObject(parent), d_ptr(new NeoTableHeaderItemPrivate()) {
	setTitle(title);
	setItemWidth(itemWidth);
	setTitleFontSize(titleFontSize);
}


NeoTableHeaderItem::~NeoTableHeaderItem() {
	Q_D(NeoTableHeaderItem);
	delete d;
}


QString NeoTableHeaderItem::title()  const {
	const Q_D(NeoTableHeaderItem);
	return d->title;
}


void NeoTableHeaderItem::setTitle(const QString& title) {
	Q_D(NeoTableHeaderItem);
	if(title == d->title) {
		return;
	}
	d->title = title;
	emit titleChanged(title);
}


qreal NeoTableHeaderItem::itemWidth() const {
	const Q_D(NeoTableHeaderItem);
	return d->itemWidth;
}


void NeoTableHeaderItem::setItemWidth(qreal itemWidth) {
	Q_D(NeoTableHeaderItem);
	if(itemWidth == d->itemWidth) {
		return;
	}
	d->itemWidth = itemWidth;
	emit itemWidthChanged(itemWidth);
}


bool NeoTableHeaderItem::sortable() const {
	const Q_D(NeoTableHeaderItem);
	return d->sortable;
}


void NeoTableHeaderItem::setSortable(bool sortable) {
	Q_D(NeoTableHeaderItem);
	if(sortable == d->sortable) {
		return;
	}
	d->sortable = sortable;
	emit sortableChanged(sortable);
}


QString NeoTableHeaderItem::toolTip() const {
	const Q_D(NeoTableHeaderItem);
	return d->toolTip;
}


void NeoTableHeaderItem::setToolTip(const QString& toolTip) {
	Q_D(NeoTableHeaderItem);
	if (toolTip == d->toolTip) {
		return;
	}
	d->toolTip = toolTip;
	emit toolTipChanged(toolTip);
}


qreal NeoTableHeaderItem::titleFontSize() const {
	const Q_D(NeoTableHeaderItem);
	return d->titleFontSize;
}


void NeoTableHeaderItem::setTitleFontSize(qreal titleFontSize) {
	Q_D(NeoTableHeaderItem);
	if(titleFontSize == d->titleFontSize) {
		return;
	}
	d->titleFontSize = titleFontSize;
	emit titleFontSizeChanged(titleFontSize);
}


bool NeoTableHeaderItem::visible() const {
	const Q_D(NeoTableHeaderItem);
	return d->visible;
}


void NeoTableHeaderItem::setVisible(bool visible) {
	Q_D(NeoTableHeaderItem);
	if(visible == d->visible) {
		return;
	}
	d->visible = visible;
	emit visibleChanged(visible);
}


bool NeoTableHeaderItem::movable() const {
	const Q_D(NeoTableHeaderItem);
	return d->movable;
}


void NeoTableHeaderItem::setMovable(bool movable) {
	Q_D(NeoTableHeaderItem);
	if(d->movable == movable) {
		return;
	}
	d->movable = movable;
	emit movableChanged(movable);
}


bool NeoTableHeaderItem::resizable() const {
	const Q_D(NeoTableHeaderItem);
	return d->resizable;
}


void NeoTableHeaderItem::setResizable(bool resizable) {
	Q_D(NeoTableHeaderItem);
	if(d->resizable == resizable) {
		return;
	}
	d->resizable = resizable;
	emit resizableChanged(resizable);
}


int NeoTableHeaderItem::minWidth() const {
	const Q_D(NeoTableHeaderItem);
	return d->minWidth;
}


void NeoTableHeaderItem::setMinWidth(int minWidth) {
	Q_D(NeoTableHeaderItem);
	if(minWidth == d->minWidth) {
		return;
	}
	d->minWidth = minWidth;
	emit minWidthChanged(minWidth);
}


int NeoTableHeaderItem::titleHorizontalAlignment() const {
	const Q_D(NeoTableHeaderItem);
	return d->titleHorizontalAlignment;
}


void NeoTableHeaderItem::setTitleHorizontalAlignment(int titleHorizontalAlignment) {
	Q_D(NeoTableHeaderItem);
	if(titleHorizontalAlignment == d->titleHorizontalAlignment) {
		return;
	}
	d->titleHorizontalAlignment = titleHorizontalAlignment;
	emit titleHorizontalAlignmentChanged(titleHorizontalAlignment);
}


QVariant NeoTableHeaderItem::extraData() const {
	const Q_D(NeoTableHeaderItem);
	return d->extraData;
}


void NeoTableHeaderItem::setExtraData(const QVariant& extraData) {
	Q_D(NeoTableHeaderItem);
	if(extraData == d->extraData) {
		return;
	}
	d->extraData = extraData;
	emit extraDataChanged(extraData);
}
