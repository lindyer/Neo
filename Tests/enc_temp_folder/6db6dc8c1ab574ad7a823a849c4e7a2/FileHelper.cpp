#include "FileHelper.h"

#include <QFileInfo>

namespace neo {
	
	

	QUrl urlByRelativePath(QString &&path) {
		QFileInfo fi(std::move(path));
		return QString("file:///%1").arg(fi.absoluteFilePath());
	}
}
