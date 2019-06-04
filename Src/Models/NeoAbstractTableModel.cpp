#include "NeoAbstractTableModel.h"
#include "NeoModelHelper.h"
#include "NeoTableHeaderModel.h"
#include <qqml.h> 
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

class NeoAbstractTableModelPrivate {
public:
	QVector<NeoTableHeaderItem*> headerItems;
	QVector<int> hiddenColumns;
	std::function<int(QVariant)> contentMetricsFunctor;
};

NeoAbstractTableModel::NeoAbstractTableModel(QObject* parent)
	: QAbstractTableModel(parent),d_ptr(new NeoAbstractTableModelPrivate()) {
	qmlRegisterType<NeoTableHeaderItem>("Neo.Quick", 1, 0, "NeoTableHeaderItem");
}


NeoAbstractTableModel::~NeoAbstractTableModel() {
}


QHash<int, QByteArray> NeoAbstractTableModel::roleNames() const {
	return defaultTableRoleNames();
}


void NeoAbstractTableModel::initHiddenColumns(const QVector<int>& columns) {
	Q_D(NeoAbstractTableModel);
	d->hiddenColumns = columns;
}


void NeoAbstractTableModel::addHiddenColumn(int role) {
	Q_D(NeoAbstractTableModel);
	d->hiddenColumns.append(role);
}


void NeoAbstractTableModel::removeHiddenColumn(int role) {
	Q_D(NeoAbstractTableModel);
	d->hiddenColumns.removeOne(role);
}


int NeoAbstractTableModel::resizeToContents(int column, const QFont& font) {
	if(column < 0 || column >= columnCount()) {
		return 100;
	}
	int maxWidth = 0;
	QFontMetrics metrics(font);
	Q_D(NeoAbstractTableModel);
	for(int row = 0; row < rowCount(); row++) {
		int itemWidth = 0;
		if(d->contentMetricsFunctor) {
			itemWidth = d->contentMetricsFunctor(data(createIndex(row, column)));
		} else { 
			//default text
			QVariant value = data(createIndex(row, column));
			if (value.canConvert<QString>()) {
				itemWidth = metrics.horizontalAdvance(value.toString());
			} else {
				qWarning() << "resizeToContents(int,const QFont &) need to provide a contentMetricsFunctor";
			}
		}
		if (maxWidth < itemWidth) {
			maxWidth = itemWidth;
		}
	}
	return maxWidth;
}


void NeoAbstractTableModel::setContentMetrics(std::function<int(const QVariant &)> &&functor) {
	Q_D(NeoAbstractTableModel);
	d->contentMetricsFunctor = std::move(functor);
}


qreal NeoAbstractTableModel::headerWidth() {
	Q_D(NeoAbstractTableModel);
	auto &list = d->headerItems;
	qreal sum = 0;
	for(int i = 0;i < list.size(); i++) {
		
		sum += list[i]->itemWidth();
	}
	return sum;
}



//int NeoAbstractTableModel::columnCount(const QModelIndex& parent) const {
//	return headerItemCount();
//}
