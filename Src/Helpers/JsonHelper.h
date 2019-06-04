#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "3rd/utilities/Json.h"

#include <QJsonDocument>
#include <QPair>
#include <QSharedPointer>


namespace neo {
	/**
	 * \brief 移除JSON字符串中注释
	 * \param content JSON字符串
	 */
	void removeJsonComment(QString& content);

	/**
	 * \brief 读取JSON文件并移除其中的注释 // \/**\/
	 * \param jsonFilePath JSON文件路径
	 * \return bool:是否解析成功，Json
	 */
	QString readJsonFileWithRemoveComment(const QString& jsonFilePath);

	/**
	 * \brief 生成Json操作对象
	 * \param jsonFilePath json文件路径
	 * \param haveComment 是否包含注释
	 * \return Json共享指针对象
	 */
	QSharedPointer<Json> createJsonPtr(const QString& jsonFilePath,bool haveComment = true);

}
