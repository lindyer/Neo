#include "MovableArea.h"
#include <QQuickWindow>

namespace Neo {

namespace Quick {

struct MovableAreaPrivate {
    QObject* target = nullptr;
    QQuickItem* item = nullptr;
    QQuickWindow* window = nullptr;
    QPoint cursorPos;
	bool hoverEnable = false;
    Qt::CursorShape cursorShape = Qt::ArrowCursor;
};


MovableArea::MovableArea(QQuickItem* parent)
    : QQuickItem(parent),
      dptr(new MovableAreaPrivate()) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}


MovableArea::~MovableArea() {
}


QObject* MovableArea::target() const {
    return dptr->target;
}


void MovableArea::setTarget(QObject* target) {
    if (target == dptr->target) {
        return;
    }
    dptr->target = target;
    emit targetChanged(target);

    dptr->item = qobject_cast<QQuickItem*>(dptr->target);
    if (dptr->item) {
        return;
    }
    dptr->window = qobject_cast<QQuickWindow*>(dptr->target);
    if (dptr->window) {
        return;
    }
}


bool MovableArea::hoverEnable() const {
	return dptr->hoverEnable;
}


void MovableArea::setHoverEnable(bool enable) {
	if(dptr->hoverEnable == enable) {
		return;
	}
	dptr->hoverEnable = enable;
	emit hoverEnableChanged(enable);
	setAcceptHoverEvents(enable);
}


int MovableArea::cursorShape() const
{
    return dptr->cursorShape;
}

void MovableArea::setCursorShape(int cursorShape)
{
	if(dptr->cursorShape == cursorShape) {
		return;
	}
	dptr->cursorShape = Qt::CursorShape(cursorShape);
	emit cursorShapeChanged(cursorShape);
}


void MovableArea::mousePressEvent(QMouseEvent*) {
    dptr->cursorPos = QCursor::pos();
}


void MovableArea::mouseMoveEvent(QMouseEvent*) {
    auto cursorPos = QCursor::pos();
    auto deltaX = cursorPos.x() - dptr->cursorPos.x();
    auto deltaY = cursorPos.y() - dptr->cursorPos.y();
    if(dptr->item) {
        dptr->item->setX(dptr->item->x() + deltaX);
        dptr->item->setY(dptr->item->y() + deltaY);
    } else if(dptr->window) {
        dptr->window->setX(dptr->window->x() + deltaX);
        dptr->window->setY(dptr->window->y() + deltaY);
    }
    dptr->cursorPos = cursorPos;
    emit positionChanged(cursorPos,deltaX,deltaY);
}


void MovableArea::mouseDoubleClickEvent(QMouseEvent*) {
    emit doubleClicked();
}


void MovableArea::hoverEnterEvent(QHoverEvent *) {
    setCursor(dptr->cursorShape);
}

} //Quick

} //Neo

