#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:
**  @Brief:
**/

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace Neo {

namespace Gui {

struct MovableAreaPrivate;
class MovableArea : public QWidget {
    Q_OBJECT
public:
    explicit MovableArea(QWidget *parent = nullptr);
    QWidget* target();
    void setTarget(QWidget* target);

signals:
    void targetChanged(QObject* target);
    void doubleClicked(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    MovableAreaPrivate *dptr = nullptr;
};

} // Gui

} // Neo
