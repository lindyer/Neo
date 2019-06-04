#pragma once

/**
**  This file is a part of Neo
**  @Creator:       linjianpeng
**  @Github:        github/com/lindyer
**  @Date:          2019-5-22
**  @Note:    QML使用前先注册: qmlRegisterType<MovableArea>("Neo.Quick", 1, 0, "MovableArea");
**          target指定为parent时需要注意parent是否有设置default属性
**/


#include <QQuickItem>

namespace neo {

namespace Quick {

class MovableAreaPrivate;

class MovableArea : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject* target READ target WRITE setTarget NOTIFY targetChanged)
	Q_PROPERTY(bool hoverEnable READ hoverEnable WRITE setHoverEnable NOTIFY hoverEnableChanged)
    Q_PROPERTY(int cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged)
public:
    MovableArea(QQuickItem* parent = nullptr);
    ~MovableArea() override;
    QObject* target() const;
    void setTarget(QObject* target);
	bool hoverEnable() const;
	void setHoverEnable(bool enable);
    int cursorShape() const;
    void setCursorShape(int cursorShape);

signals:
    void targetChanged(QObject* target);
    void positionChanged(QPoint cursorPos,int deltaX,int deltaY);
	void hoverEnableChanged(bool hoverEnable);
    void cursorShapeChanged(int cursorShape);
    void doubleClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void hoverEnterEvent(QHoverEvent *event) override;

private:
	Q_DECLARE_PRIVATE(MovableArea)
    MovableAreaPrivate * const d_ptr = nullptr;
};

} //Quick

} //Neo

