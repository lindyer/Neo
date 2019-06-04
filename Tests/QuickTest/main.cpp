/***
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDir>

#include <Quick/Quick>
#include <Quick/MovableArea.h>
#include "Network/Ping.h"
#include "Common/LogHandler.h"
#include "TableModelTest.h"
#include "Models/NeoTableHeaderModel.h"
#include "Common/ScopeGuard.h"
#include "Common/Common.h"
#include "Helpers/Helpers.h"


using namespace neo;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    registerNeoQuickType();
	qmlRegisterType<TableModelTest>("Neo.Tests", 1, 0, "TableModelTest");
	qmlRegisterType<NeoTableHeaderItem>("Neo.Quick", 1, 0, "NeoTableHeaderItem");
	qmlRegisterType<NeoTableHeaderModel>("Neo.Quick", 1, 0, "NeoTableHeaderModel"); 

    QQmlApplicationEngine engine;
    engine.addImportPath("../../Src/QML");
    engine.load(urlByRelativePath("main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
