#include <QList>
#include <functional>
#include <QtQml> 

#pragma once
namespace neo {

	void registerQmlTypeList();

	QList<std::function<void()>>& qmlTypeRegisterList();

	template <typename T>
	struct QmlTypeRegister {
		QmlTypeRegister() {
			auto initializer = []() {
				qmlRegisterType<T>();
			};
			qmlTypeRegisterList().append(initializer);
		}
		QmlTypeRegister(const char *uri,const char *qmlName,int majorVersion = 1,int minorVersion = 0) {
			auto initializer = [uri,qmlName,majorVersion,minorVersion]() {
				qmlRegisterType<T>(uri,majorVersion,minorVersion,qmlName);
			};
			qmlTypeRegisterList().append(initializer);
		}
	};

	template <typename T,typename ...Args>
	struct QmlSingletonTypeRegister {
		QmlSingletonTypeRegister(const char* uri, const char* qmlName, int majorVersion = 1, int minorVersion = 0, Args... args) {
			auto initializer = [uri, qmlName, majorVersion, minorVersion,args...]() {
				qmlRegisterSingletonType<T>(uri, majorVersion, minorVersion, qmlName, [](QQmlEngine* , QJSEngine*) -> QObject* {
					return new T(args...);
				});
			};
			qmlTypeRegisterList().append(initializer);
		}
	};
};

#define NEO_TYPE_STR(Type) #Type

//concat name and line
#define _NEO_NAME_LINE_CAT(Name,Line) Name##Line

#define NEO_NAME_LINE_CAT(Name,Line) _NEO_NAME_LINE_CAT(Name,Line)

#define NEO_QML_TYPE_ID NEO_NAME_LINE_CAT(qmlTypeId,__LINE__)

#define NEO_QML_REGISTER(Uri, Type) static int NEO_QML_TYPE_ID = qmlRegisterType<Type>(Uri, 1, 0, #Type);
#define NEO_QML_DEFAULT_REGISTER(Type) static int NEO_QML_TYPE_ID = qmlRegisterType<Type>("Neo",1,0,#Type);
#define NEO_QML_QUICK_REGISTER(Type) static int NEO_QML_TYPE_ID = qmlRegisterType<Type>("Neo.Quick",1,0,#Type);
#define NEO_QML_MODEL_REGISTER(Type) static int NEO_QML_TYPE_ID = qmlRegisterType<Type>("Neo.Model",1,0,#Type);

/*
 * Note: if you want to import your classes to QML,under macros you will need for easy use. 
 * You must call neo::registerQmlTypeList() method,this method is called in App::run() automatic.
 */

//for header file declare,normal class
#define NEO_QML_TYPE_REGISTER_DECLARE(Type) static neo::QmlTypeRegister<Type> _;
//for cpp file define
#define NEO_QML_TYPE_REGISTER_DEFINE(Uri,Type) neo::QmlTypeRegister<Type> Type::_{Uri,#Type};

//for header file declare,singleton class
#define NEO_QML_SINGLETON_TYPE_REGISTER_DECLARE(Type) static neo::QmlSingletonTypeRegister<Type> _;
//for cpp file define
#define NEO_QML_SINGLETON_TYPE_REGISTER_DEFINE(Uri,Type) neo::QmlSingletonTypeRegister<Type> Type::_{Uri,#Type};


