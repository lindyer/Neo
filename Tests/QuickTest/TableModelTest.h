#pragma once
#pragma execution_character_set("utf-8")
#include "NeoAbstractTableModel.h"

class TableModelTest: public NeoAbstractTableModel {
	Q_OBJECT

public: 
	TableModelTest(QObject* parent = nullptr);
	enum Columns {
		C1,
		C2,
		C3,
		C4,
		ColumnCount
	};

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;

public slots:
	//QQmlListProperty<NeoTableHeaderItem> headerList();


private: 
};
