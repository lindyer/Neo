#pragma once


#include "Noncopyable.h"
#include "Global.h"

#include <functional>

//auto select base application type
#ifdef QT_WIDGETS_LIB
#include <QApplication>
#else
#include <QGuiApplication>
#endif

namespace neo {
	
#ifdef QT_WIDGETS_LIB
	using AppType = QApplication;
#else
	using AppType = QGuiApplication;
#endif

	class App: public Noncopyable {
	public:
		static int run(int argc, char* argv[], std::function<bool(AppType&, QQmlApplicationEngine &engine)> init);
	private:
		App() = default;
	};

}
