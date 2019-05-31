#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include "MovableArea_Widget.h"

namespace Neo {

namespace Gui {

struct MovableAreaPrivate {
    QWidget *target = nullptr;
    QPoint cursorPos;
};

MovableArea::MovableArea(QWidget *parent) : QWidget(parent) {

}

QWidget *MovableArea::target()
{
    return dptr->target;
}

void MovableArea::setTarget(QWidget *target)
{
    if(target == dptr->target) {
        return;
    }
    dptr->target = target;
    targetChanged(target);
}

void MovableArea::mousePressEvent(QMouseEvent *)
{
    dptr->cursorPos = QCursor::pos();
}

void MovableArea::mouseMoveEvent(QMouseEvent *)
{
    auto cursorPos = QCursor::pos();
    auto deltaX = cursorPos.x() - dptr->cursorPos.x();
    auto deltaY = cursorPos.y() - dptr->cursorPos.y();
    dptr->target->move(dptr->target->x() + deltaX,dptr->target->y() + deltaY);
    dptr->cursorPos = cursorPos;
}

void MovableArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(event);
}

} // Gui

} // Neo

