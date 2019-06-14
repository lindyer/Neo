#pragma once
#pragma execution_character_set("utf-8")
#include "AbstractTableModel.h"
#include "Common/Global.h"

class TableModelTest: public neo::AbstractTableModel {
	Q_OBJECT
		
	NEO_QML_TYPE_REGISTER_DECLARE(TableModelTest)
public:
	TableModelTest(QObject* parent = nullptr){}
	TableModelTest(const QString & jsonFilePath,const QString &keyPath,QObject* parent = nullptr);

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent) const override;

public slots:

private:
	QVariant fieldProvider(const QString& field) const override;
};
