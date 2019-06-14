#include "TableModelTest.h"
#include "TableHeaderModel.h"
#include <QDebug>

NEO_QML_TYPE_REGISTER_DEFINE("Neo.Tests",TableModelTest)
//neo::QmlTypeRegister<TableModelTest> TableModelTest::_("Neo.Tests","TableModelTest");

QVariant TableModelTest::data(const QModelIndex& index, int role) const {
	int row = index.row();
	int col = index.column();
	const auto headerItem = headerModel()->visibleHeaderItemAt(col);
	if(!headerItem) {
		return QVariant("");
	}
	return fieldProvider(headerItem->field());
}


TableModelTest::TableModelTest(const QString& jsonFilePath, const QString& keyPath,QObject* parent): AbstractTableModel(jsonFilePath,keyPath,parent) {

}

int TableModelTest::rowCount(const QModelIndex& parent) const {
	return 1;
}


QVariant TableModelTest::fieldProvider(const QString& field) const {
	if(field == "Date") {
		return "Date";
	} else if(field == "StockCode") {
		return "StockCode";
	} else if(field == "StockName") {
		return "StockName";
	}
	return QVariant("undefined");
}
