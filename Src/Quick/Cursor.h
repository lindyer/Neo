#pragma once
#include <QObject>
#include <QPoint>
#include "Common/Global.h"

namespace neo {
	
	namespace quick {
		
		class Cursor : public QObject {
			Q_OBJECT

			NEO_QML_SINGLETON_TYPE_REGISTER_DECLARE(Cursor)
		public:
			explicit Cursor(QObject* parent = nullptr);

		public slots:
			QPoint pos() const;

			void setPos(int x, int y);
		};

	}

}


