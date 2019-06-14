#include "App.h"


namespace neo {
	
	int App::run(int argc, char* argv[], std::function<bool(AppType&, QQmlApplicationEngine&)> init) {

		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
		AppType app(argc, argv);
		registerQmlTypeList();
		QQmlApplicationEngine engine;
		if(!init(app,engine)) {
			return -1;
		}
		return app.exec();
	}

}

