#include "JsonHelper.h"

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include <QFile>
#include <QDebug>

namespace neo { 

	void removeJsonComment(QString& content) { 
//#ifndef JSON_FILE_PRINT_OFF
//		qDebug() << content;
//#endif
		std::function<void(QString&)> functor;
		int dobuleSlashSearchIndex = 0;
		int slashStarSearchIndex = 0;
		functor = [&functor, &dobuleSlashSearchIndex,&slashStarSearchIndex](QString& str) {
			int doubleSlashIndex = str.indexOf("//", dobuleSlashSearchIndex);
			if (doubleSlashIndex != -1) {
				dobuleSlashSearchIndex = doubleSlashIndex;
				int returnIndex = str.indexOf("\n", dobuleSlashSearchIndex + 2);
				str.remove(doubleSlashIndex, returnIndex - doubleSlashIndex);
				functor(str);
			}
			int slashStarIndex = str.indexOf("/*", slashStarSearchIndex);
			if (slashStarIndex != -1) {
				slashStarSearchIndex = slashStarIndex;
				int starSlashIndex = str.indexOf("*/", slashStarSearchIndex + 2);
				str.remove(slashStarIndex, starSlashIndex - slashStarIndex + 2);
				functor(str);
			}
		};
		functor(content);
	}


	QString readJsonFileWithRemoveComment(const QString& jsonFilePath) {
		QFile file(jsonFilePath);
		if (!file.open(QFile::ReadOnly)) {
			return QString();
		}
		QString content = file.readAll();  
		removeJsonComment(content);
		return content;
	}


	QSharedPointer<Json> createJsonSharedPtr(const QString& jsonFilePath, bool haveComment) {
		if(haveComment) {
			return QSharedPointer<Json>::create(readJsonFileWithRemoveComment(jsonFilePath), false);
		}
		return QSharedPointer<Json>::create(jsonFilePath, true);
	}

	Json* createJsonPtr(const QString& jsonFilePath, bool haveComment) {
		if (haveComment) {
			return new Json(readJsonFileWithRemoveComment(jsonFilePath), false);
		}
		return new Json(jsonFilePath, true);
	}


	QJsonValue getJsonValue(const QString& jsonFilePath, const QString& keyPath) {
		auto&& ptr = createJsonPtr(jsonFilePath, true);
		return ptr->getJsonValue(keyPath);
	}


	void sortJsonArray(QJsonArray& array, std::function<bool(const QJsonValue&, const QJsonValue&)> greaterFn) {
		for(int i = 0; i < array.size() - 1; i++) {
			for(int j = 0; j < array.size() - i - 1; j++) {
				if(!greaterFn(array[j],array[j+1])) {
					QJsonValue tmp = array[j];
					array.replace(j, array[j + 1]);
					array.replace(j + 1, tmp);
				}
			}
		}
	}
}

