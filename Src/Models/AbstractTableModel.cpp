#include "AbstractTableModel.h"
#include "ModelHelper.h"
#include "TableHeaderModel.h"
#include <qqml.h> 
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

namespace neo {
	
	class AbstractTableModelPrivate {
	public:
		AbstractTableModelPrivate(const QString& jsonFilePath, const QString& keyPath);
		~AbstractTableModelPrivate();
		TableHeaderModel* headerModel = nullptr;
		std::function<int(QVariant)> contentMetricsFunctor;
	};


	AbstractTableModelPrivate::AbstractTableModelPrivate(const QString& jsonFilePath, const QString& keyPath) : headerModel(new TableHeaderModel(jsonFilePath, keyPath)) {

	}


	AbstractTableModelPrivate::~AbstractTableModelPrivate() {
	}


	AbstractTableModel::AbstractTableModel(QObject* parent)
		: QAbstractTableModel(parent) {
	}


	AbstractTableModel::AbstractTableModel(const QString& jsonFilePath, const QString& keyPath, QObject* parent)
		: QAbstractTableModel(parent), d_ptr(new AbstractTableModelPrivate(jsonFilePath, keyPath)) {
		Q_D(AbstractTableModel);
		connect(d->headerModel, &TableHeaderModel::columnVisibleChanged, this, [this](int column, bool visible) {
			if (visible) {
				beginInsertColumns(QModelIndex(), column, column);
				endInsertColumns();
			} else {
				beginRemoveColumns(QModelIndex(), column, column);
				endRemoveColumns();
			}
			});
	}


	AbstractTableModel::~AbstractTableModel() {
		Q_D(AbstractTableModel);
		delete d;
	}


	QHash<int, QByteArray> AbstractTableModel::roleNames() const {
		return defaultTableRoleNames();
	}


	int AbstractTableModel::columnCount(const QModelIndex& parent) const {
		const Q_D(AbstractTableModel);
		return d->headerModel->visibleHeaderItemCount();
	}


	int AbstractTableModel::resizeToContents(int column, const QFont& font) {
		if (column < 0 || column >= columnCount(QModelIndex())) {
			return 100;
		}
		int maxWidth = 0;
		QFontMetrics metrics(font);
		Q_D(AbstractTableModel);
		for (int row = 0; row < rowCount(); row++) {
			int itemWidth = 0;
			if (d->contentMetricsFunctor) {
				itemWidth = d->contentMetricsFunctor(data(createIndex(row, column)));
			}
			else {
				//default text
				QVariant value = data(createIndex(row, column));
				if (value.canConvert<QString>()) {
					itemWidth = metrics.horizontalAdvance(value.toString());
				}
				else {
					qWarning() << "resizeToContents(int,const QFont &) need to provide a contentMetricsFunctor";
				}
			}
			if (maxWidth < itemWidth) {
				maxWidth = itemWidth;
			}
		}
		return maxWidth;
	}


	TableHeaderModel* AbstractTableModel::headerModel() const {
		const Q_D(AbstractTableModel);
		return d->headerModel;
	}


	void AbstractTableModel::move(int from, int to) {
		Q_D(AbstractTableModel);
		d->headerModel->move(from, to);
		emit dataChanged(createIndex(0, qMin(from, to)), createIndex(rowCount(QModelIndex()), qMax(from, to)));
		//beginMoveColumns(QModelIndex(), from, from, QModelIndex(), to);
		//endMoveColumns();
	}


	void AbstractTableModel::refresh() {
		emit dataChanged(createIndex(0, 0), createIndex(rowCount(QModelIndex()), columnCount(QModelIndex())));
	}


	void AbstractTableModel::setContentMetrics(std::function<int(const QVariant&)>&& functor) {
		Q_D(AbstractTableModel);
		d->contentMetricsFunctor = std::move(functor);
	}


}

