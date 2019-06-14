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
		
}
