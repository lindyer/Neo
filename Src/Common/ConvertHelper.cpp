#include "ConvertHelper.h"
#include <QJsonDocument>
#include <QJsonArray>

namespace neo {
	
	QByteArray uint16ToByteArray(quint16 num) {
		QByteArray ba;
		ba.resize(2);
		ba[0] = uchar((0xFF00 & num) >> 8);
		ba[1] = uchar(0x00FF & num);
		return ba;
	}


	QByteArray uint32ToByteArray(quint32 num) {
		QByteArray ba;
		ba.resize(4);
		ba[0] = uchar((0xFF000000 & num) >> 24);
		ba[1] = uchar((0x00FF0000 & num) >> 16);
		ba[2] = uchar((0x0000FF00 & num) >> 8);
		ba[3] = uchar((0x000000FF & num) >> 0);
		return ba;
	}


	quint32 byteArrayToUint32(const QByteArray & ba) {
		if (ba.size() != 4) {
			return 0;
		}
		quint32 num = ba[3] & 0x000000FF;
		num |= ((ba[2] << 8) & 0x0000FF00);
		num |= ((ba[1] << 16) & 0x00FF0000);
		num |= ((ba[0] << 24) & 0xFF000000);
		return num;
	}


	quint16 byteArrayToUint16(const QByteArray & ba) {
		if (ba.size() != 2) {
			return 0;
		}
		quint16 num = ba[1] & 0x00FF;
		num |= (ba[0] << 8) & 0xFF00;
		return num;
	}

	/*
	ISO 3309
	*/
	static const quint16 crc_tbl[16] = {
		0x0000, 0x1081, 0x2102, 0x3183,
		0x4204, 0x5285, 0x6306, 0x7387,
		0x8408, 0x9489, 0xa50a, 0xb58b,
		0xc60c, 0xd68d, 0xe70e, 0xf78f
	};


	quint16 checkSumISO3309(const char* data, int len) {
		quint16 crc = 0xffff;
		uchar c = 0;
		const auto* p = reinterpret_cast<const uchar*>(data);
		while (len--) {
			c = *p++;
			crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
			c >>= 4;
			crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
		}
		crc = ~crc;
		return crc & 0xffff;
	}


	QByteArray strListToPrettyStr(const QStringList& list) {
		return QJsonDocument(QJsonArray::fromStringList(list)).toJson(QJsonDocument::Indented);
	}

	QByteArray jsonArrayToPrettyStr(const QJsonArray& arr) {
		return QJsonDocument(arr).toJson(QJsonDocument::Indented);
	}
}
