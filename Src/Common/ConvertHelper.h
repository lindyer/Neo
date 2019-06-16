#pragma once
#include <QtGlobal>
#include <QByteArray>
class QStringList;

namespace neo {
	
	/**
	 * \brief 
	 * \param ba
	 * \return
	 */
	quint16 byteArrayToUint16(const QByteArray& ba);
	/**
	 * \brief
	 * \param ba
	 * \return
	 */
	quint32 byteArrayToUint32(const QByteArray& ba);
	/**
	 * \brief
	 * \return
	 */
	QByteArray uint16ToByteArray(quint16 i);
	/**
	 * \brief
	 * \param num
	 * \return
	 */
	QByteArray uint32ToByteArray(quint32 num);

	/**
	 * \brief
	 * \param data
	 * \param len
	 * \return
	 */
	quint16 checkSumISO3309(const char* data, int len);

	QByteArray strListToPrettyStr(const QStringList& list);
}

