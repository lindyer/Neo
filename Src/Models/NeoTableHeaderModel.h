#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "NeoTableHeaderItem.h"
#include "Helpers/JsonHelper.h"

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QQmlParserStatus>
class NeoTableHeaderModelPrivate;

class NeoTableHeaderModel : public QAbstractListModel, public QQmlParserStatus
{
	Q_OBJECT
	Q_INTERFACES(QQmlParserStatus)
	Q_PROPERTY(qreal headerWidth READ headerWidth WRITE setHeaderWidth NOTIFY headerWidthChanged)
	Q_PROPERTY(QQmlListProperty<NeoTableHeaderItem> headerItems READ headerItems NOTIFY headerItemsChanged)
	Q_PROPERTY(QString jsonFilePath READ jsonFilePath WRITE setJsonFilePath NOTIFY jsonFilePathChanged)
	Q_PROPERTY(QString keyPath READ keyPath WRITE setKeyPath NOTIFY keyPathChanged)

public:
	NeoTableHeaderModel(QObject* parent = nullptr);
	NeoTableHeaderModel(const QString &jsonFilePath,const QString &keyPath,QObject* parent = nullptr);
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

	QString jsonFilePath() const;
	void setJsonFilePath(const QString& jsonFilePath);

	QString keyPath() const;
	void setKeyPath(const QString& keyPath);


public slots:

	void setItemVisibleAt(int index,bool visible);

	void setItemWidthAt(int index, qreal width);

	void move(int from, int to);

signals:
	void headerWidthChanged(qreal headerWidth); 
	void headerItemsChanged();
	void columnMoved(int from, int to);
	void jsonFilePathChanged(const QString& jsonFilePath);
	void keyPathChanged(const QString& keyPath);

public:
	void classBegin() override;
	void componentComplete() override;

private:
	Q_DECLARE_PRIVATE(NeoTableHeaderModel)
	NeoTableHeaderModelPrivate* const d_ptr = nullptr;
};

