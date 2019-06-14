/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "TableHeaderModel.h"
#include "TableHeaderItem.h"
#include "TableHeaderItem_p.h"
#include "ModelHelper.h"

#include <QVariant>
#include <QQmlListProperty>
#include <QVector>
#include <QDebug>


namespace neo {
	
	//TypeRegister<TableHeaderModel> TableHeaderModel::_;

	NEO_QML_TYPE_REGISTER_DEFINE("Neo.Quick",TableHeaderModel)

	class TableHeaderModelPrivate {
	public:
		TableHeaderModelPrivate(TableHeaderModel* parent) : q_ptr(parent) {}

		static void appendHeaderItem(QQmlListProperty<TableHeaderItem>* list, TableHeaderItem* item);

		static int headerItemCount(QQmlListProperty<TableHeaderItem>* list);

		static TableHeaderItem* headerItemAt(QQmlListProperty<TableHeaderItem>* list, int index);

		static void clearHeader(QQmlListProperty<TableHeaderItem>* list);

		TableHeaderModel* const q_ptr = nullptr;
		Q_DECLARE_PUBLIC(TableHeaderModel)

		//all header items,contain hidden and visible items
		QVector<neo::TableHeaderItem*> headerItems;
		//only for easy access
		QVector<neo::TableHeaderItem*> visibleHeaderItems;
		//calc visibleHeaderItems total width
		qreal headerWidth = 0;

		//Json配置路径
		QString jsonFilePath;
		//键路径
		QString keyPath;

		QScopedPointer<Json> jsonPtr;

		QJsonObject defaultObject;
		QJsonArray itemsArray;
		//若没有提供defaultObject，则用此
		QJsonObject builtInDefaultObject{
			{ "visible",true },
			{ "width",99 },
			{ "sortable",true },
			{ "toolTip", "" }
		};

		void reloadHeaderItems();

		//item获取字段对应值，不存在则找对应默认值，再不存在找内建值（getDefaultFieldValue）
		QJsonValue getFieldValue(const QJsonObject& item, const QString& key);
		QJsonValue getDefaultFieldValue(const QString& key);

		void onSaveChange(int index, const QString& key, const QJsonValue& value);
	};

	void TableHeaderModelPrivate::appendHeaderItem(QQmlListProperty<TableHeaderItem>* list, TableHeaderItem* item) {
		reinterpret_cast<TableHeaderModel*>(list->data)->appendHeaderItem(item); 
	}  


	int TableHeaderModelPrivate::headerItemCount(QQmlListProperty<TableHeaderItem>* list) {
		return reinterpret_cast<TableHeaderModel*>(list->data)->headerItemCount();
	}


	TableHeaderItem* TableHeaderModelPrivate::headerItemAt(QQmlListProperty<TableHeaderItem>* list, int index) {
		return reinterpret_cast<TableHeaderModel*>(list->data)->headerItemAt(index);
	}


	void TableHeaderModelPrivate::clearHeader(QQmlListProperty<TableHeaderItem>* list) {
		return reinterpret_cast<TableHeaderModel*>(list->data)->clearHeader();
	}


	void TableHeaderModelPrivate::reloadHeaderItems() {
		if (jsonFilePath.isEmpty() || keyPath.isEmpty()) {
			qWarning() << "either no init jsonFilePath or keyPath property";
			return;
		}
		jsonPtr.reset(neo::createJsonPtr(jsonFilePath));
		auto value = jsonPtr->getJsonValue(keyPath);
		if (!value.isObject()) {
			qWarning() << "keyPath is not a object";
			return;
		}
		QJsonObject jsonObject = value.toObject();
		if (!jsonObject.contains("default")) {
			qDebug() << "keyPath does not contain default property";
			return;
		}
		if (!jsonObject.contains("items")) {
			qDebug() << "keyPath does not contain items property";
			return;
		}
		defaultObject = jsonObject.value("default").toObject(builtInDefaultObject);
		itemsArray = jsonObject.value("items").toArray();
		Q_Q(TableHeaderModel);
		q->beginResetModel();
		qDeleteAll(headerItems);
		headerItems.clear();
		//按索引排序
		neo::sortJsonArray(itemsArray, [](const QJsonValue& one, const QJsonValue& second) {
			return one.toObject().value("index").toInt() < second.toObject().value("index").toInt();
			});
		for (auto valueRef : itemsArray) {
			if (!valueRef.isObject()) {
				qWarning() << "Item Value is not a JsonObject";
				continue;
			}
			QJsonObject itemObject = valueRef.toObject();
			auto headerItem = new TableHeaderItem();
			QObject::connect(headerItem, &TableHeaderItem::saveChange, [this](int index, const QString& key, const QJsonValue& value) {
				this->onSaveChange(index, key, value);
				});
			auto ptr = headerItem->d_ptr;  //take care here, don't need to notify
			ptr->index = getFieldValue(itemObject, "index").toInt(); //need to first init
			ptr->itemWidth = getFieldValue(itemObject, "width").toInt();
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
			ptr->field = getFieldValue(itemObject, "field").toString();
			q->appendHeaderItem(headerItem);
		}
		q->endResetModel();
	}


	QJsonValue TableHeaderModelPrivate::getFieldValue(const QJsonObject& item, const QString& key) {
		if (!item.contains(key)) {
			return getDefaultFieldValue(key);
		}
		return item.value(key);
	}


	QJsonValue TableHeaderModelPrivate::getDefaultFieldValue(const QString& key) {
		if (!defaultObject.contains(key)) {
			return QJsonValue();
		}
		return defaultObject.value(key);
	}


	void TableHeaderModelPrivate::onSaveChange(int index, const QString& key, const QJsonValue& value) {
		bool exist = false;
		for (int i = 0; i < itemsArray.size(); i++) {
			QJsonValueRef valueRef = itemsArray[i];
			if (!valueRef.isObject()) {
				continue;
			}
			QJsonObject itemObject = valueRef.toObject();
			if (!itemObject.contains("index")) {
				qWarning() << "not found Index";
				continue;
			}
			if (itemObject.value("index").isDouble() && itemObject.value("index").toInt() == index) {
				//跟默认不一样才更新
				if (getDefaultFieldValue(key) != value) {
					itemObject.insert(key, value);
				}
				else { //移除默认值
					itemObject.remove(key);
				}
				exist = true;
				itemsArray.replace(i, itemObject);
				break;
			}
		}
		if (!exist) {
			return;
		}
		jsonPtr->set(QString("%1.items").arg(keyPath), itemsArray);
		jsonPtr->save(jsonFilePath);
	}


	TableHeaderModel::TableHeaderModel(QObject* parent) :QAbstractListModel(parent), d_ptr(new TableHeaderModelPrivate(this)) {
	}


	TableHeaderModel::TableHeaderModel(const QString& jsonFilePath, const QString& keyPath, QObject* parent) : QAbstractListModel(parent), d_ptr(new TableHeaderModelPrivate(this)) {
		setJsonFilePath(jsonFilePath);
		setKeyPath(keyPath);
		d_ptr->reloadHeaderItems();
	}


	TableHeaderModel::~TableHeaderModel() {
		Q_D(TableHeaderModel);
		delete d;
	}


	int TableHeaderModel::rowCount(const QModelIndex& parent) const {
		const Q_D(TableHeaderModel);
		return d->headerItems.size();
	}


	QVariant TableHeaderModel::data(const QModelIndex& index, int role) const {
		const Q_D(TableHeaderModel);
		int row = index.row();
		if (row < 0 || row >= d->headerItems.size()) {
			return QVariant();
		}
		return QVariant::fromValue(d->headerItems[row]);
	}


	QHash<int, QByteArray> TableHeaderModel::roleNames() const {
		return defaultListRoleNames();
	}


	qreal TableHeaderModel::headerWidth() const {
		const Q_D(TableHeaderModel);
		return d->headerWidth;
	}


	void TableHeaderModel::setHeaderWidth(qreal headerWidth) {
		Q_D(TableHeaderModel);
		if (headerWidth == d->headerWidth) {
			return;
		}
		d->headerWidth = headerWidth;
		emit headerWidthChanged(headerWidth);
	}

	QQmlListProperty<TableHeaderItem> TableHeaderModel::headerItems() {
		return QQmlListProperty<TableHeaderItem> {this, this,
			& TableHeaderModelPrivate::appendHeaderItem,
			& TableHeaderModelPrivate::headerItemCount,
			& TableHeaderModelPrivate::headerItemAt,
			& TableHeaderModelPrivate::clearHeader
		};
	}


	void TableHeaderModel::appendHeaderItem(TableHeaderItem* item) {
		Q_D(TableHeaderModel);
		if (item->visible()) {
			d->visibleHeaderItems.append(item);
			setHeaderWidth(headerWidth() + item->itemWidth());
		}
		d->headerItems.append(item);
		emit headerItemsChanged();
	}


	int TableHeaderModel::headerItemCount() const {
		const Q_D(TableHeaderModel);
		return d->headerItems.size();
	}

	int TableHeaderModel::visibleHeaderItemCount() const {
		const Q_D(TableHeaderModel);
		return d->visibleHeaderItems.size();
	}


	TableHeaderItem* TableHeaderModel::headerItemAt(int index) {
		Q_D(TableHeaderModel);
		if (index < 0 || index >= d->headerItems.size()) {
			return nullptr;
		}
		return d->headerItems[index];
	}


	TableHeaderItem* TableHeaderModel::visibleHeaderItemAt(int index) {
		Q_D(TableHeaderModel);
		if (index < 0 || index >= d->visibleHeaderItems.size()) {
			return nullptr;
		}
		return d->visibleHeaderItems[index];
	}


	void TableHeaderModel::clearHeader() {
		Q_D(TableHeaderModel);
		qDeleteAll(d->headerItems);
		d->visibleHeaderItems.clear();
		d->headerItems.clear();
	}


	QString TableHeaderModel::jsonFilePath() const {
		const Q_D(TableHeaderModel);
		return d->jsonFilePath;
	}


	void TableHeaderModel::setJsonFilePath(const QString& jsonFilePath) {
		Q_D(TableHeaderModel);
		if (jsonFilePath == d->jsonFilePath) {
			return;
		}
		d->jsonFilePath = jsonFilePath;
		emit jsonFilePathChanged(jsonFilePath);
	}


	QString TableHeaderModel::keyPath() const {
		const Q_D(TableHeaderModel);
		return d->keyPath;
	}


	void TableHeaderModel::setKeyPath(const QString& keyPath) {
		Q_D(TableHeaderModel);
		if (keyPath == d->keyPath) {
			return;
		}
		d->keyPath = keyPath;
		emit keyPathChanged(keyPath);
	}


	void TableHeaderModel::setItemVisibleAt(const QString& text, bool visible) {
		Q_D(TableHeaderModel);

		TableHeaderItem* existItem = nullptr;
		int index = 0;
		for (auto item : d->headerItems) {
			if (item->title() == text) {
				existItem = item;
				break;
			}
			if (item->visible()) {
				index++;
			}
		}


		if (!existItem || existItem->visible() == visible) {
			return;
		}

		//由于TableModel列依赖HeaderModel（columnCount），先更新TableModel
		emit columnVisibleChanged(index, visible);

		if (visible) {
			auto findResult = std::find(d->visibleHeaderItems.begin(), d->visibleHeaderItems.end(), existItem);
			if (findResult == d->visibleHeaderItems.end()) {
				d->visibleHeaderItems.append(existItem);
				std::sort(d->visibleHeaderItems.begin(), d->visibleHeaderItems.end(), [](const TableHeaderItem& first, const TableHeaderItem& second) {
					return first.index() < second.index();
					});
			}
		}
		else {
			d->visibleHeaderItems.erase(std::remove(d->visibleHeaderItems.begin(), d->visibleHeaderItems.end(), existItem));
		}

		existItem->setVisible(visible);

		if (visible) {
			setHeaderWidth(headerWidth() + existItem->itemWidth());
		}
		else {
			setHeaderWidth(headerWidth() - existItem->itemWidth());
		}

	}


	void TableHeaderModel::setItemWidthAt(int index, qreal width) {
		auto item = headerItemAt(index);
		if (!item) {
			return;
		}
		if (item->minWidth() > width) {
			width = item->minWidth();
		}
		auto delta = width - item->itemWidth();
		item->setItemWidth(width);
		setHeaderWidth(headerWidth() + delta);
	}


	void TableHeaderModel::move(int from, int to) {
		Q_D(TableHeaderModel);
		if (from == to || from < 0 || from >= d->visibleHeaderItems.size() || to < 0 || to >= d->visibleHeaderItems.size()) {
			return;
		}
		auto fromItem = d->visibleHeaderItems[from];
		auto toItem = d->visibleHeaderItems[to];
		fromItem->setIndex(to);
		toItem->setIndex(from);
		d->visibleHeaderItems.move(from, to);
		int srcFromIndex = d->headerItems.indexOf(fromItem);
		int srcToIndex = d->headerItems.indexOf(toItem);
		if (srcFromIndex < 0 || srcToIndex < 0) {
			return;
		}
		d->headerItems.move(srcFromIndex, srcToIndex);
		dataChanged(createIndex(qMin(from, to), 0), createIndex(qMax(from, to), 0));
		emit columnMoved(from, to);
	}


	void TableHeaderModel::classBegin() {
		emit beginInit();
	}


	void TableHeaderModel::componentComplete() {
		Q_D(TableHeaderModel);
		d->reloadHeaderItems();
	}


}
