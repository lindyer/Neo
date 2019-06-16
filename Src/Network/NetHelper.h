#pragma once
#include <QString>
#include <QObject>


namespace neo { 

	//for C++
	bool isIPv4(const QString& ip);

	//for qml
	class NetHelper : public QObject {
		Q_OBJECT
	public:
		explicit NetHelper(QObject* parent = nullptr);

	public slots:
		bool isIPv4(const QString& str) const;
	};

	inline quint64 operator"" _KB(quint64 n) {
		return n * 1024;
	}


	inline quint64 operator"" _MB(quint64 n) {
		return n * 1024_KB;
	}


	inline quint64 operator"" _GB(quint64 n) {
		return n * 1024_MB;
	}
}
