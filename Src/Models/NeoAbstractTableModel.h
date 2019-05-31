#pragma once
#include <QAbstractTableModel>
#include "NeoTableHeaderItem.h"
#include <QQmlListProperty>

struct NeoAbstractTableModelPrivate;
class NeoAbstractTableModel : public QAbstractTableModel {
	Q_OBJECT 

	Q_PROPERTY(qreal headerWidth READ headerWidth NOTIFY headerWidthChanged)
public:
	NeoAbstractTableModel(QObject* parent = nullptr);

	~NeoAbstractTableModel();
	QHash<int,QByteArray> roleNames() const override;
	//int columnCount(const QModelIndex& parent) const override final;
	qreal headerWidth();

signals:
	void headerWidthChanged(qreal headerWidth);

public slots:
	void initHiddenColumns(const QVector<int>& columns);

	void addHiddenColumn(int role);

	void removeHiddenColumn(int role);

	//�������ݴ�С����
	int resizeToContents(int column,const QFont &font);

public:
	/**
	 * \brief 
	 * \param functor QVariant: data(), ����ֵ�������ݿ��
	 */
	void setContentMetrics(std::function<int(const QVariant&)> &&functor);
private:
	Q_DECLARE_PRIVATE(NeoAbstractTableModel)
	NeoAbstractTableModelPrivate* const d_ptr = nullptr;
};
