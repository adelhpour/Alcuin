#include "negui_bezier_adjust_handled_graphics_item.h"
#include "negui_bezier_adjust_customized_graphics_items.h"

#include <QPen>
#include <QCursor>

// MyBezierAdjustHandledGraphicsItemsBase

MyBezierAdjustHandledGraphicsItemsBase::MyBezierAdjustHandledGraphicsItemsBase(qreal zValue, QGraphicsItem *parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    _isPressed = false;
}

// MySinglePointBezierAdjustHandledGraphicsItems

MySinglePointBezierAdjustHandledGraphicsItems::MySinglePointBezierAdjustHandledGraphicsItems(const QPointF& point, const QPointF& controlPoint, qreal zValue, QGraphicsItem *parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createBezierAdjustHandledGraphicsItem(point, controlPoint);
    _isPressed = false;
}

void MySinglePointBezierAdjustHandledGraphicsItems::createBezierAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint) {
    _adjustHandledGraphicsItem = new MyBezierAdjustHandledGraphicsItem(point, controlPoint);
    connect((MyBezierAdjustHandledGraphicsItem*)_adjustHandledGraphicsItem, SIGNAL(handlePositionIsUpdated(const QPointF&)), this, SLOT(updateAdjustHandledGraphicsItem(const QPointF&)));
    connect((MyBezierAdjustHandledGraphicsItem*)_adjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isPressed, this, [this] () { _isPressed = true; });
    connect((MyBezierAdjustHandledGraphicsItem*)_adjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isReleased, this, [this] () { _isPressed = false; });

    addToGroup(_adjustHandledGraphicsItem);
}

void MySinglePointBezierAdjustHandledGraphicsItems::adjust(const QPointF& controlPoint) {
    if (!_isPressed)
        ((MyBezierAdjustHandledGraphicsItem*)_adjustHandledGraphicsItem)->updatePosition(controlPoint);
}

void MySinglePointBezierAdjustHandledGraphicsItems::updateAdjustHandledGraphicsItem(const QPointF& controlPoint) {
    ((MyBezierAdjustHandledGraphicsItem*)_adjustHandledGraphicsItem)->updatePosition(controlPoint);
    emit controlPointIsUpdated(controlPoint);
}

// MyDoublePointBezierAdjustHandledGraphicsItems

MyDoublePointBezierAdjustHandledGraphicsItems::MyDoublePointBezierAdjustHandledGraphicsItems(const QPointF& startPoint, const QPointF& controlPoint1, const QPointF& controlPoint2, const QPointF& endPoint, qreal zValue, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createBezierStartAdjustHandledGraphicsItem(startPoint, controlPoint1);
    createBezierEndAdjustHandledGraphicsItem(endPoint, controlPoint2);
    _isPressed = false;
}

void MyDoublePointBezierAdjustHandledGraphicsItems::createBezierStartAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint) {
    _startAdjustHandledGraphicsItem = new MyBezierAdjustHandledGraphicsItem(point, controlPoint);
    connect((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem, SIGNAL(handlePositionIsUpdated(const QPointF&)), this, SLOT(updateStartAdjustHandledGraphicsItem(const QPointF&)));
    connect((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isPressed, this, [this] () { _isPressed = true; });
    connect((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isReleased, this, [this] () { _isPressed = false; });

    addToGroup(_startAdjustHandledGraphicsItem);
}

void MyDoublePointBezierAdjustHandledGraphicsItems::createBezierEndAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint) {
    _endAdjustHandledGraphicsItem = new MyBezierAdjustHandledGraphicsItem(point, controlPoint);
    connect((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem, SIGNAL(handlePositionIsUpdated(const QPointF&)), this, SLOT(updateEndAdjustHandledGraphicsItem(const QPointF&)));
    connect((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isPressed, this, [this] () { _isPressed = true; });
    connect((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem, &MyBezierAdjustHandledGraphicsItem::isReleased, this, [this] () { _isPressed = false; });

    addToGroup(_endAdjustHandledGraphicsItem);
}

void MyDoublePointBezierAdjustHandledGraphicsItems::adjust(const QPointF& controlPoint1, const QPointF& controlPoint2) {
    if (!_isPressed) {
        ((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem)->updatePosition(controlPoint1);
        ((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem)->updatePosition(controlPoint2);
    }
}

void MyDoublePointBezierAdjustHandledGraphicsItems::updateStartAdjustHandledGraphicsItem(const QPointF& startControlPoint) {
    ((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem)->updatePosition(startControlPoint);
    emit startControlPointIsUpdated(startControlPoint);
}

void MyDoublePointBezierAdjustHandledGraphicsItems::updateEndAdjustHandledGraphicsItem(const QPointF& endControlPoint) {
    ((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem)->updatePosition(endControlPoint);
    emit endControlPointIsUpdated(endControlPoint);
}

// MyBezierAdjustHandledGraphicsItem

MyBezierAdjustHandledGraphicsItem::MyBezierAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    createLine(point, controlPoint);
    createHandle(controlPoint);
}

void MyBezierAdjustHandledGraphicsItem::createLine(const QPointF& p1, const QPointF& p2) {
    _line = new MyBezierAdjustLineGraphicsItem(p1.x(), p1.y(), p2.x(), p2.y());
    addToGroup(_line);
}

void MyBezierAdjustHandledGraphicsItem::createHandle(const QPointF& center) {
    _controlHandle = new MyBezierControlPointHandleGraphicsItem(center);
    connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(positionChanged(const QPointF&)), this, SIGNAL(handlePositionIsUpdated(const QPointF&)));
    connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(isPressed()), this, SIGNAL(isPressed()));
    connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(isReleased()), this, SIGNAL(isReleased()));
    addToGroup(_controlHandle);
}

void MyBezierAdjustHandledGraphicsItem::updatePosition(const QPointF& controlPoint) {
    ((MyBezierAdjustLineGraphicsItem*)_line)->setLine(((MyBezierAdjustLineGraphicsItem*)_line)->line().p1().x(), ((MyBezierAdjustLineGraphicsItem*)_line)->line().p1().y(), controlPoint.x(), controlPoint.y());
    ((MyBezierControlPointHandleGraphicsItem*)_controlHandle)->updatePosition(controlPoint);
}

const QPointF MyBezierAdjustHandledGraphicsItem::position() {
    return ((MyBezierControlPointHandleGraphicsItem*)_controlHandle)->position();
}