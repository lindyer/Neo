/***
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QGuiApplication>

#include <Quick/Quick.h>
#include <Quick/MovableArea.h>
#include "Network/Ping.h"
#include "Common/LogHandler.h"
#include "TableModelTest.h"
#include "Common/FileHelper.h"
#include "Common/App.h"

using namespace neo;

int main(int argc, char *argv[])
{
	App::run(argc, argv, [](AppType &app,QQmlApplicationEngine &engine)-> bool {
		engine.addImportPath("../../Src/QML");
		auto test = new TableModelTest("app.settings", "tables.tableName.header");
		engine.rootContext()->setContextProperty("tableModelTest",test);
		engine.load(urlByRelativePath("main.qml"));
		return !engine.rootObjects().isEmpty();
	});
}
