#include "JsonHelper.h"

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include <QFile>
#include <QDebug>

namespace Neo { 

	void removeJsonComment(QString& content) { 
#ifndef JSON_FILE_PRINT_OFF
		qDebug() << content;
#endif
		std::function<void(QString&)> functor;
		int searchIndex = 0;
		functor = [&functor, &searchIndex](QString& str) {
			int doubleSlashIndex = str.indexOf("//", searchIndex);
			if (doubleSlashIndex != -1) {
				searchIndex = doubleSlashIndex;
				int returnIndex = str.indexOf("\n", searchIndex + 2);
				str.remove(doubleSlashIndex, returnIndex - doubleSlashIndex);
				functor(str);
			}
			int slashStarIndex = str.indexOf("/*", searchIndex);
			if (slashStarIndex != -1) {
				searchIndex = slashStarIndex;
				int starSlashIndex = str.indexOf("*/", searchIndex + 2);
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


	QSharedPointer<Json> createJsonPtr(const QString& jsonFilePath, bool haveComment) {
		if(haveComment) {
			return QSharedPointer<Json>::create(readJsonFileWithRemoveComment(jsonFilePath), false);
		}
		return QSharedPointer<Json>::create(jsonFilePath, true);
	}

}

