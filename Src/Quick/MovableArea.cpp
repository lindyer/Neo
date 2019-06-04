#include "MovableArea.h"
#include <QQuickWindow>

namespace neo {

namespace Quick {

class MovableAreaPrivate {
public:
    QObject* target = nullptr;
    QQuickItem* item = nullptr;
    QQuickWindow* window = nullptr;
    QPoint cursorPos;
	bool hoverEnable = false;
    Qt::CursorShape cursorShape = Qt::ArrowCursor;
};


MovableArea::MovableArea(QQuickItem* parent)
    : QQuickItem(parent),
      d_ptr(new MovableAreaPrivate()) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}


MovableArea::~MovableArea() {
}


QObject* MovableArea::target() const {
	const Q_D(MovableArea);
    return d->target;
}


void MovableArea::setTarget(QObject* target) {
	Q_D(MovableArea);
    if (target == d->target) {
        return;
    }
    d->target = target;
    emit targetChanged(target);

    d->item = qobject_cast<QQuickItem*>(d->target);
    if (d->item) {
        return;
    }
    d->window = qobject_cast<QQuickWindow*>(d->target);
    if (d->window) {
        return;
    }
}


bool MovableArea::hoverEnable() const {
	const Q_D(MovableArea);
	return d->hoverEnable;
}


void MovableArea::setHoverEnable(bool enable) {
	Q_D(MovableArea);
	if(d->hoverEnable == enable) {
		return;
	}
	d->hoverEnable = enable;
	emit hoverEnableChanged(enable);
	setAcceptHoverEvents(enable);
}


int MovableArea::cursorShape() const
{
	const Q_D(MovableArea);
    return d->cursorShape;
}

void MovableArea::setCursorShape(int cursorShape)
{
	Q_D(MovableArea);
	if(d->cursorShape == cursorShape) {
		return;
	}
	d->cursorShape = Qt::CursorShape(cursorShape);
	emit cursorShapeChanged(cursorShape);
}


void MovableArea::mousePressEvent(QMouseEvent*) {
	Q_D(MovableArea);
    d->cursorPos = QCursor::pos();
}


void MovableArea::mouseMoveEvent(QMouseEvent*) {
	Q_D(MovableArea);
    auto cursorPos = QCursor::pos();
    auto deltaX = cursorPos.x() - d->cursorPos.x();
    auto deltaY = cursorPos.y() - d->cursorPos.y();
    if(d->item) {
        d->item->setX(d->item->x() + deltaX);
        d->item->setY(d->item->y() + deltaY);
    } else if(d->window) {
        d->window->setX(d->window->x() + deltaX);
        d->window->setY(d->window->y() + deltaY);
    }
    d->cursorPos = cursorPos;
    emit positionChanged(cursorPos,deltaX,deltaY);
}


void MovableArea::mouseDoubleClickEvent(QMouseEvent*) {
    emit doubleClicked();
}


void MovableArea::hoverEnterEvent(QHoverEvent *) {
	Q_D(MovableArea);
    setCursor(d->cursorShape);
}

} //Quick

} //Neo

