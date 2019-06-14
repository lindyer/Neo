#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "TableHeaderItem.h"
#include "Common/JsonHelper.h"

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QQmlParserStatus>
#include "Common/Global.h"

namespace neo {

	class TableHeaderModelPrivate;

	class TableHeaderModel : public QAbstractListModel, public QQmlParserStatus
	{
		Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		Q_PROPERTY(qreal headerWidth READ headerWidth WRITE setHeaderWidth NOTIFY headerWidthChanged)
		Q_PROPERTY(QQmlListProperty<neo::TableHeaderItem> headerItems READ headerItems NOTIFY headerItemsChanged)
		Q_PROPERTY(QString jsonFilePath READ jsonFilePath WRITE setJsonFilePath NOTIFY jsonFilePathChanged)
		Q_PROPERTY(QString keyPath READ keyPath WRITE setKeyPath NOTIFY keyPathChanged)

		NEO_QML_TYPE_REGISTER_DECLARE(TableHeaderModel)
	public:
		TableHeaderModel(QObject* parent = nullptr);
		TableHeaderModel(const QString& jsonFilePath, const QString& keyPath, QObject* parent = nullptr);
		~TableHeaderModel();

		int rowCount(const QModelIndex& parent) const override;

		QVariant data(const QModelIndex& index, int role) const override;

		QHash<int, QByteArray> roleNames() const override;

		qreal headerWidth() const;

		void setHeaderWidth(qreal headerWidth);

		QQmlListProperty<neo::TableHeaderItem> headerItems();

		void appendHeaderItem(neo::TableHeaderItem* item);

		int headerItemCount() const;

		int visibleHeaderItemCount() const;

		neo::TableHeaderItem* headerItemAt(int index);

		neo::TableHeaderItem* visibleHeaderItemAt(int index);

		void clearHeader();

		QString jsonFilePath() const;
		void setJsonFilePath(const QString& jsonFilePath);

		QString keyPath() const;
		void setKeyPath(const QString& keyPath);



	public slots:

		void setItemVisibleAt(const QString& text, bool visible);

		void setItemWidthAt(int index, qreal width);

		void move(int from, int to);

	signals:
		void beginInit();
		void headerWidthChanged(qreal headerWidth);
		void headerItemsChanged();
		void columnMoved(int from, int to);
		void columnVisibleChanged(int column, bool visible);
		void jsonFilePathChanged(const QString& jsonFilePath);
		void keyPathChanged(const QString& keyPath);

	public:
		void classBegin() override;
		void componentComplete() override;

	private:
		Q_DECLARE_PRIVATE(TableHeaderModel)
		TableHeaderModelPrivate* const d_ptr = nullptr;
	};


}
