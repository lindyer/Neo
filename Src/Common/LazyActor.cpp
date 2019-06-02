#include "LazyActor.h"

struct LazyActorPrivate {
	QHash<int, int> timerIdNType;
	QHash<int, std::function<void()> > typeNActor;
};

LazyActor::LazyActor(QObject* parent) : QObject(parent), d_ptr(new LazyActorPrivate()) {

}


LazyActor& LazyActor::instance() {
	static LazyActor la;
	return la;
}


void LazyActor::exec(int type, std::function<void()> fn, int interval) {
	Q_D(LazyActor);
	if (d->typeNActor.contains(type)) {
		d->typeNActor.insert(type, fn);  //replace it
		return;
	}
	int timeId = startTimer(interval, Qt::PreciseTimer);
	d->timerIdNType.insert(timeId, type);
	d->typeNActor.insert(type, fn);
}

void LazyActor::timerEvent(QTimerEvent* te) {
	Q_D(LazyActor);
	 int timerId = te->timerId();
	 if (!d->timerIdNType.contains(timerId)) {
		 qWarning() << "Can not find timerId";
		 return;
	 }
	 int type = d->timerIdNType.take(timerId);
	 if (!d->typeNActor.contains(type)) {
		 qWarning() << "Can not find type";
		 return;
	 }
	 killTimer(timerId);
	 auto fn = d->typeNActor.take(type);
	 fn();
}
