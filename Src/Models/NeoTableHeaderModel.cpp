/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "NeoTableHeaderModel.h"
#include "NeoTableHeaderItem.h"
#include "NeoModelHelper.h"

class NeoTableHeaderModelPrivate {
public:
	static void appendHeaderItem(QQmlListProperty<NeoTableHeaderItem>* list, NeoTableHeaderItem* item);

	static int headerItemCount(QQmlListProperty<NeoTableHeaderItem>* list);

	static NeoTableHeaderItem* headerItemAt(QQmlListProperty<NeoTableHeaderItem>* list, int index);

	static void clearHeader(QQmlListProperty<NeoTableHeaderItem>* list);

	QVector<NeoTableHeaderItem*> headerItems;

	qreal headerWidth = 0;
};

void NeoTableHeaderModelPrivate::appendHeaderItem(QQmlListProperty<NeoTableHeaderItem>* list, NeoTableHeaderItem* item) {
	reinterpret_cast<NeoTableHeaderModel*>(list->data)->appendHeaderItem(item);
}


int NeoTableHeaderModelPrivate::headerItemCount(QQmlListProperty<NeoTableHeaderItem>* list) {
	return reinterpret_cast<NeoTableHeaderModel*>(list->data)->headerItemCount();
}


NeoTableHeaderItem* NeoTableHeaderModelPrivate::headerItemAt(QQmlListProperty<NeoTableHeaderItem>* list, int index) {
	return reinterpret_cast<NeoTableHeaderModel*>(list->data)->headerItemAt(index);
}


void NeoTableHeaderModelPrivate::clearHeader(QQmlListProperty<NeoTableHeaderItem>* list) {
	return reinterpret_cast<NeoTableHeaderModel*>(list->data)->clearHeader();
}


NeoTableHeaderModel::NeoTableHeaderModel(QObject* parent) :QAbstractListModel(parent), d_ptr (new NeoTableHeaderModelPrivate()){
}


NeoTableHeaderModel::~NeoTableHeaderModel() {
	Q_D(NeoTableHeaderModel);
	delete d;
}


int NeoTableHeaderModel::rowCount(const QModelIndex& parent) const {
	const Q_D(NeoTableHeaderModel); 
	return d->headerItems.size();
}


QVariant NeoTableHeaderModel::data(const QModelIndex& index, int role) const {
	const Q_D(NeoTableHeaderModel);
	int row = index.row();
	if(row < 0 || row >= d->headerItems.size()) {
		return QVariant();
	}
	return QVariant::fromValue(d->headerItems[row]);
}


QHash<int, QByteArray> NeoTableHeaderModel::roleNames() const {
	return defaultListRoleNames();
}


qreal NeoTableHeaderModel::headerWidth() const {
	const Q_D(NeoTableHeaderModel);
	return d->headerWidth;
}


void NeoTableHeaderModel::setHeaderWidth(qreal headerWidth) {
	Q_D(NeoTableHeaderModel);
	if(headerWidth == d->headerWidth) {
		return; 
	}
	d->headerWidth = headerWidth;
	emit headerWidthChanged(headerWidth);
}

QQmlListProperty<NeoTableHeaderItem> NeoTableHeaderModel::headerItems() {
	return QQmlListProperty<NeoTableHeaderItem> {this, this,
		& NeoTableHeaderModelPrivate::appendHeaderItem,
		& NeoTableHeaderModelPrivate::headerItemCount,
		& NeoTableHeaderModelPrivate::headerItemAt,
		& NeoTableHeaderModelPrivate::clearHeader
	};
}


void NeoTableHeaderModel::appendHeaderItem(NeoTableHeaderItem* item) {
	Q_D(NeoTableHeaderModel);
	d->headerItems.append(item); 
	setHeaderWidth(headerWidth() + item->itemWidth());
	emit headerItemsChanged();
}


int NeoTableHeaderModel::headerItemCount() const {
	const Q_D(NeoTableHeaderModel);
	return d->headerItems.size();
}


NeoTableHeaderItem* NeoTableHeaderModel::headerItemAt(int index) {
	Q_D(NeoTableHeaderModel);
	if (index < 0 || index >= d->headerItems.size()) {
		return nullptr;
	}
	return d->headerItems[index];
}


void NeoTableHeaderModel::clearHeader() {
	Q_D(NeoTableHeaderModel);
	d->headerItems.clear();
}


void NeoTableHeaderModel::setItemVisibleAt(int index, bool visible) {
	Q_D(NeoTableHeaderModel);
	if(index < 0 || index >= d->headerItems.size()) {
		return;
	}
	auto &item = d->headerItems[index];
	if(item->visible() != visible) {
		if(visible) {
			setHeaderWidth(headerWidth() + item->itemWidth());
		} else {
			setHeaderWidth(headerWidth() - item->itemWidth());
		}
	}
	item->setVisible(visible);
}


void NeoTableHeaderModel::setItemWidthAt(int index, qreal width) {
	auto item = headerItemAt(index);
	if(!item) {
		return;
	}
	if(item->minWidth() > width) {
		width = item->minWidth();
	}
	auto delta = width - item->itemWidth();
	item->setItemWidth(width);
	setHeaderWidth(headerWidth() + delta);
}
