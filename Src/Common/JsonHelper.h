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
	 * \brief �Ƴ�JSON�ַ�����ע��
	 * \param content JSON�ַ���
	 */
	void removeJsonComment(QString& content);

	/**
	 * \brief ��ȡJSON�ļ����Ƴ����е�ע�� // \/**\/
	 * \param jsonFilePath JSON�ļ�·��
	 * \return bool:�Ƿ�����ɹ���Json
	 */
	QString readJsonFileWithRemoveComment(const QString& jsonFilePath);

	/**
	 * \brief ����Json��������
	 * \param jsonFilePath json�ļ�·��
	 * \param haveComment �Ƿ����ע��
	 * \return Json����ָ�����
	 */
	QSharedPointer<Json> createJsonSharedPtr(const QString& jsonFilePath,bool haveComment = true);

	Json* createJsonPtr(const QString& jsonFilePath, bool haveComment = true);
	/**
	 * \brief ��ȡJson��ֵ
	 * \param jsonFilePath json�ļ�·��
	 * \param keyPath ��·�� 
	 * \return 
	 */
	QJsonValue getJsonValue(const QString &jsonFilePath,const QString &keyPath);

	void sortJsonArray(QJsonArray& array,std::function<bool(const QJsonValue &,const QJsonValue &)> greaterFn);

}