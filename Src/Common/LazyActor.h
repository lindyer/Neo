#pragma once

#include <functional>

#include <QObject>
#include <QHash>
#include <QTimerEvent>
#include <QDebug>

/*
* 延迟执行，相同类型的操作合并为一个执行 
*/

struct LazyActorPrivate;
class LazyActor: public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(LazyActor)

public:
	static LazyActor& instance();

	void exec(int type, std::function<void()> fn, int interval = 500);

protected:
	virtual void timerEvent(QTimerEvent* te) override;

private:
	explicit LazyActor(QObject *parent = nullptr);

private:
	Q_DECLARE_PRIVATE(LazyActor)
	LazyActorPrivate * const d_ptr = nullptr;
};