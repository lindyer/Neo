#include "LazyActor.h"

struct LazyActorPrivate {
	QHash<int, int> timerIdNType;
	QHash<int, std::function<void()> > typeNActor;
};

LazyActor::LazyActor(QObject* parent) : QObject(parent), _dptr(new LazyActorPrivate()) {

}


LazyActor& LazyActor::instance() {
	static LazyActor la;
	return la;
}


void LazyActor::exec(int type, std::function<void()> fn, int interval) {
	if (_dptr->typeNActor.contains(type)) {
		_dptr->typeNActor.insert(type, fn);  //replace it
		return;
	}
	int timeId = startTimer(interval, Qt::PreciseTimer);
	_dptr->timerIdNType.insert(timeId, type);
	_dptr->typeNActor.insert(type, fn);
}

void LazyActor::timerEvent(QTimerEvent* te) {
	 int timerId = te->timerId();
	 if (!_dptr->timerIdNType.contains(timerId)) {
		 qWarning() << "Can not find timerId";
		 return;
	 }
	 int type = _dptr->timerIdNType.take(timerId);
	 if (!_dptr->typeNActor.contains(type)) {
		 qWarning() << "Can not find type";
		 return;
	 }
	 killTimer(timerId);
	 auto fn = _dptr->typeNActor.take(type);
	 fn();
}
