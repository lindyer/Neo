#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "NeoTableHeaderItem.h"

#include <QAbstractListModel>
#include <QQmlListProperty>

class NeoTableHeaderModelPrivate;

class NeoTableHeaderModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(qreal headerWidth READ headerWidth WRITE setHeaderWidth NOTIFY headerWidthChanged)
	Q_PROPERTY(QQmlListProperty<NeoTableHeaderItem> headerItems READ headerItems NOTIFY headerItemsChanged)

public:
	NeoTableHeaderModel(QObject* parent = nullptr);
	~NeoTableHeaderModel();

	int rowCount(const QModelIndex& parent) const override;

	QVariant data(const QModelIndex& index, int role) const override;

	QHash<int,QByteArray> roleNames() const override;

	qreal headerWidth() const;

	void setHeaderWidth(qreal headerWidth);

	QQmlListProperty<NeoTableHeaderItem> headerItems();

	void appendHeaderItem(NeoTableHeaderItem* item);

	int headerItemCount() const;

	NeoTableHeaderItem* headerItemAt(int index);

	void clearHeader();


public slots:
	void setItemVisibleAt(int index,bool visible);

	void setItemWidthAt(int index, qreal width);

signals:
	void headerWidthChanged(qreal headerWidth);
	void headerItemsChanged();

private:
	Q_DECLARE_PRIVATE(NeoTableHeaderModel)
	NeoTableHeaderModelPrivate* const d_ptr = nullptr;
};

