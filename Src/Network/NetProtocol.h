#pragma once

#include <functional>

#include <QtGlobal>
#include <QString>
#include <QByteArray>

namespace neo {
	
	class NetProtocol {
	public:
		enum Version {
			Version01
		};

		enum Command {
			MsgText = 1,
			SevenZipFiles
		};

		struct Header {
			quint16 version = 0;               //版本号
			quint16 cmd = 0;                   //命令码
			quint32 msgSize = 0;               //消息体大小
			QString toString() const;
		};

		constexpr static int ProtocolHeaderSize = sizeof(Header);

		struct Message {
			Header header;
			QByteArray body;
			QByteArray serialize() const;
		};

		QByteArray pack(const QByteArray& body, quint16 cmd = MsgText, quint16 version = Version01);

		Message unpack(const QByteArray& data);

		Header parseHeader(const QByteArray& header);

		void parseData(QByteArray& data, const std::function<void(const Message&)>& msgHandler);

	};

	template<typename T>
	struct ConnectionItem {
		T* socket = nullptr;
		QByteArray buffer;
		~ConnectionItem() {
			socket->close();
			socket->deleteLater();
			buffer.clear();
		}
	};


	template<typename T>
	struct ConnectionList : public QList<ConnectionItem<T>*> {
		using QList<ConnectionItem<T>*>::size;
		using QList<ConnectionItem<T>*>::at;
		using QList<ConnectionItem<T>*>::removeAt;
		ConnectionItem<T>* findConnection(T* socket) {
			for (int i = 0; i < size(); i++) {
				auto& item = at(i);
				if (socket == item->socket) {
					return item;
				}
			}
			return nullptr;
		}
		bool remove(T* socket) {
			for (int i = 0; i < size(); i++) {
				auto& item = at(i);
				if (socket == item->socket) {
					removeAt(i);
					return true;
				}
			}
			return false;
		}
		void add(T* socket, QByteArray buffer = QByteArray()) {
			auto conn = new ConnectionItem<T>();
			conn->socket = socket;
			conn->buffer = buffer;
			append(conn);
		}
	};


	
}


