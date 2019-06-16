#include "NetProtocol.h"
#include "Common/ConvertHelper.h"
#include <winsock2.h>
#include <QDebug>


namespace neo {
	
	QString NetProtocol::Header::toString() const {
		return QString("Version: %1,Command: %2,MsgSize: %3").arg(version).arg(cmd).arg(msgSize);
	}


	QByteArray NetProtocol::Message::serialize() const {
		QByteArray ba;
		ba.append(uint16ToByteArray(header.version));
		ba.append(uint16ToByteArray(header.cmd));
		ba.append(uint32ToByteArray(header.msgSize));
		ba.append(body);
		return ba;
	}


	QByteArray NetProtocol::pack(const QByteArray& body, quint16 cmd, quint16 version) {
		QByteArray ba;
		ba.append(uint16ToByteArray(version));
		ba.append(uint16ToByteArray(cmd));
		ba.append(uint32ToByteArray(ProtocolHeaderSize + quint32(body.size())));
		ba.append(body);
		return ba;
	}


	NetProtocol::Message NetProtocol::unpack(const QByteArray & data) {
		Message msg;
		msg.header = parseHeader(data.mid(0, ProtocolHeaderSize));
		msg.body = data.mid(ProtocolHeaderSize);
		return msg;
	}


	NetProtocol::Header NetProtocol::parseHeader(const QByteArray & header) {
		Header h;
		h.version = byteArrayToUint16(header.mid(0, 2));
		h.cmd = byteArrayToUint16(header.mid(2, 2));
		h.msgSize = byteArrayToUint32(header.mid(4, 4));
		return h;
	}


	void NetProtocol::parseData(QByteArray & data, const std::function<void(const NetProtocol::Message&)> & msgHandler) {
		int versionSize = sizeof(quint16);
		if (data.size() >= versionSize) {
			auto version = data.mid(0, versionSize);
			if (uint16ToByteArray(NetProtocol::Version01) != version) {
				qWarning() << "Version doesn't match,drop it";
				data.clear();
				return;
			}
		}
		if (data.size() < NetProtocol::ProtocolHeaderSize) {
			return;
		}
		auto header = parseHeader(data.mid(0, NetProtocol::ProtocolHeaderSize));
		if (header.msgSize <= data.size()) {
			auto item = data.mid(0, header.msgSize);  //取出一个完整的信息包
			data.remove(0, header.msgSize);
			auto msg = unpack(item);
			if (msgHandler) {
				msgHandler(msg);
			}
			return;
		}
	}
}
