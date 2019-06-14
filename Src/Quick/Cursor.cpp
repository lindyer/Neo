#include "Cursor.h"

#include <QCursor>

namespace neo {
	
	namespace quick {
		
		NEO_QML_SINGLETON_TYPE_REGISTER_DEFINE("Neo.Quick",Cursor)

		Cursor::Cursor(QObject* parent) : QObject(parent) {
		}


		QPoint Cursor::pos() const {
			return QCursor::pos();
		}


		void Cursor::setPos(int x, int y) {
			QCursor::setPos(x, y);
		}


	}

}


