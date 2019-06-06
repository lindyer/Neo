#include "TableModelTest.h"
#include <QDebug>


QVariant TableModelTest::data(const QModelIndex& index, int role) const {
	int row = index.row();
	int col = index.column();
	if(col == 0) {
		return "你好啊，Table";
	} else if(col == 1) {
		return "Goods,Friends";
	}
	return "AAAN";
}


//QQmlListProperty<NeoTableHeaderItem> TableModelTest::headerList() {
//	appendHeaderItem(new NeoTableHeaderItem("L1",100,14));
//	appendHeaderItem(new NeoTableHeaderItem("L2", 200, 14));
//	appendHeaderItem(new NeoTableHeaderItem("L3", 150, 14));
//	appendHeaderItem(new NeoTableHeaderItem("L4", 50, 14));
//
//}


TableModelTest::TableModelTest(QObject* parent) {

}

int TableModelTest::rowCount(const QModelIndex& parent) const {
	return 10;
}


//int TableModelTest::columnCount(const QModelIndex& parent) const {
//	return  ColumnCount;
//}
