/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "NeoTableHeaderModel.h"
#include "NeoTableHeaderItem.h"
#include "NeoModelHelper.h"
#include "NeoTableHeaderItem_p.h"

#include <QVariant>
#include <QQmlListProperty>
#include <QVector>
#include <QDebug>

class NeoTableHeaderModelPrivate {
public:
	NeoTableHeaderModelPrivate(NeoTableHeaderModel *parent): q_ptr (parent) {}

	static void appendHeaderItem(QQmlListProperty<NeoTableHeaderItem>* list, NeoTableHeaderItem* item);

	static int headerItemCount(QQmlListProperty<NeoTableHeaderItem>* list);

	static NeoTableHeaderItem* headerItemAt(QQmlListProperty<NeoTableHeaderItem>* list, int index);

	static void clearHeader(QQmlListProperty<NeoTableHeaderItem>* list);

	NeoTableHeaderModel* const q_ptr = nullptr;
	Q_DECLARE_PUBLIC(NeoTableHeaderModel)

	QVector<NeoTableHeaderItem*> headerItems;

	qreal headerWidth = 0;
	//Json配置路径
	QString jsonFilePath;
	//键路径
	QString keyPath;

	QScopedPointer<Json> jsonPtr;

	QJsonObject defaultObject;
	QJsonArray itemsArray;
	//若没有提供defaultObject，则用此
	QJsonObject builtInDefaultObject {
		{ "visible",true },
		{ "width",99 },
		{ "sortable",true },
		{ "toolTip", "" }
	};

	void reloadHeaderItems();

	//item获取字段对应值，不存在则找对应默认值，再不存在找内建值（getDefaultFieldValue）
	QJsonValue getFieldValue(const QJsonObject &item,const QString &key);
	QJsonValue getDefaultFieldValue(const QString &key);

	void onSaveChange(int index, const QString& key, const QJsonValue& value);
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


void NeoTableHeaderModelPrivate::reloadHeaderItems() {
	if(jsonFilePath.isEmpty() || keyPath.isEmpty()) {
		qWarning() << "either no init jsonFilePath or keyPath property";
		return;
	}
	jsonPtr.reset(neo::createJsonPtr(jsonFilePath));
	auto value = jsonPtr->getJsonValue(keyPath);
	if(!value.isObject()) {
		qWarning() << "keyPath is not a object";
		return;
	}
	QJsonObject jsonObject = value.toObject();
	if(!jsonObject.contains("default")) {
		qDebug() << "keyPath does not contain default property"; 
		return;
	}
	if (!jsonObject.contains("items")) {
		qDebug() << "keyPath does not contain items property";
		return;
	}
	defaultObject = jsonObject.value("default").toObject(builtInDefaultObject);
	itemsArray = jsonObject.value("items").toArray();
	Q_Q(NeoTableHeaderModel);
	q->beginResetModel();
	qDeleteAll(headerItems);
	headerItems.clear();
	//按索引排序
	neo::sortJsonArray(itemsArray, [](const QJsonValue &one, const QJsonValue& second) {
		return one.toObject().value("index").toInt() < second.toObject().value("index").toInt();
	});
	for(auto valueRef: itemsArray) {
		if(!valueRef.isObject()) {
			qWarning() << "Item Value is not a JsonObject";
			continue;
		}
		QJsonObject itemObject = valueRef.toObject();
		auto headerItem = new NeoTableHeaderItem();
		QObject::connect(headerItem, &NeoTableHeaderItem::saveChange, [this](int index,const QString& key, const QJsonValue& value) {
			this->onSaveChange(index,key, value);
		});
		auto ptr = headerItem->d_ptr;  //take care here, don't need to notify
		ptr->index = getFieldValue(itemObject, "index").toInt(); //need to first init
		headerItem->setItemWidth(getFieldValue(itemObject, "width").toInt());
		ptr->title = getFieldValue(itemObject, "title").toString();
		ptr->sortable = getFieldValue(itemObject, "sortable").toBool();
		ptr->toolTip = getFieldValue(itemObject, "toolTip").toString();
		ptr->titleFontSize = getFieldValue(itemObject, "titleFontSize").toInt();
		ptr->visible = getFieldValue(itemObject, "visible").toBool();
		ptr->movable = getFieldValue(itemObject, "movable").toBool();
		ptr->resizable = getFieldValue(itemObject, "resizable").toBool();
		ptr->minWidth = getFieldValue(itemObject, "minWidth").toInt();
		ptr->titleHorizontalAlignment = getFieldValue(itemObject, "titleHorizontalAlignment").toInt();
		ptr->extraData = getFieldValue(itemObject, "extraData");
		q->appendHeaderItem(headerItem);
	}
	q->endResetModel();
}


QJsonValue NeoTableHeaderModelPrivate::getFieldValue(const QJsonObject& item, const QString& key) {
	if (!item.contains(key)) {
		return getDefaultFieldValue(key);
	}
	return item.value(key);
}


QJsonValue NeoTableHeaderModelPrivate::getDefaultFieldValue(const QString& key) {
	if(!defaultObject.contains(key)) {
		return QJsonValue();
	}
	return defaultObject.value(key);
}


void NeoTableHeaderModelPrivate::onSaveChange(int index,const QString& key, const QJsonValue& value) {
	bool exist = false;
	for (int i = 0; i < itemsArray.size(); i++) {
		QJsonValueRef valueRef = itemsArray[i];
		if(!valueRef.isObject()) {
			continue;
		}
		QJsonObject itemObject = valueRef.toObject();
		if(!itemObject.contains("index")) {
			qWarning() << "not found Index";
			continue;
		}
		if(itemObject.value("index").isDouble() && itemObject.value("index").toInt() == index) {
			//跟默认不一样才更新
			if(getDefaultFieldValue(key) != value) {
				itemObject.insert(key, value);
				itemsArray.replace(i, itemObject);
				exist = true;
			}
			break;
		}
	}
	if(!exist) {
		return;
	}
	jsonPtr->set(QString("%1.items").arg(keyPath), itemsArray);
	jsonPtr->save(jsonFilePath);
}


NeoTableHeaderModel::NeoTableHeaderModel(QObject* parent) :QAbstractListModel(parent), d_ptr (new NeoTableHeaderModelPrivate(this)){
}


NeoTableHeaderModel::NeoTableHeaderModel(const QString& jsonFilePath, const QString& keyPath, QObject* parent) : QAbstractListModel(parent), d_ptr(new NeoTableHeaderModelPrivate(this)) {
	setJsonFilePath(jsonFilePath);
	setKeyPath(keyPath);
	d_ptr->reloadHeaderItems();
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


QString NeoTableHeaderModel::jsonFilePath() const {
	const Q_D(NeoTableHeaderModel);
	return d->jsonFilePath;
}


void NeoTableHeaderModel::setJsonFilePath(const QString& jsonFilePath) {
	Q_D(NeoTableHeaderModel);
	if(jsonFilePath == d->jsonFilePath) {
		return;
	}
	d->jsonFilePath = jsonFilePath;
	emit jsonFilePathChanged(jsonFilePath);
}


QString NeoTableHeaderModel::keyPath() const {
	const Q_D(NeoTableHeaderModel);
	return d->keyPath;
}


void NeoTableHeaderModel::setKeyPath(const QString& keyPath) {
	Q_D(NeoTableHeaderModel);
	if(keyPath == d->keyPath) {
		return;
	}
	d->keyPath = keyPath;
	emit keyPathChanged(keyPath);
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


void NeoTableHeaderModel::move(int from, int to) {
	if(from == to) {
		return;
	}
	Q_D(NeoTableHeaderModel);
	d->headerItems.move(from, to);
	dataChanged(createIndex(qMin(from,to), 0), createIndex(qMax(from, to), 0));
	emit columnMoved(from, to);
}


void NeoTableHeaderModel::classBegin() {

}


void NeoTableHeaderModel::componentComplete() {
	Q_D(NeoTableHeaderModel);
	d->reloadHeaderItems();
}
