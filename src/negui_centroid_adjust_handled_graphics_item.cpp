#include "negui_centroid_adjust_handled_graphics_item.h"
#include "negui_bezier_adjust_customized_graphics_items.h"

#include <QPen>

MyCentroidAdjustHandledGraphicsItems::MyCentroidAdjustHandledGraphicsItems(const QRectF &rect, const QLineF &line, qreal zValue, QGraphicsItem *parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createCentroidFocusedGraphicsItem(rect);
    createBezierAdjustLineFocusedGraphicsItem(line);
    _isPressed = false;
}

void MyCentroidAdjustHandledGraphicsItems::createCentroidFocusedGraphicsItem(const QRectF &rect) {
    _centroidFocusedGraphicsItem = new QGraphicsEllipseItem(rect);
    ((QGraphicsEllipseItem*)_centroidFocusedGraphicsItem)->setPen(QPen(QColor("#0000CD")));
    ((QGraphicsEllipseItem*)_centroidFocusedGraphicsItem)->setBrush((QBrush(QColor("#4169e1"), Qt::SolidPattern)));
    addToGroup(_centroidFocusedGraphicsItem);
}

void MyCentroidAdjustHandledGraphicsItems::createBezierAdjustLineFocusedGraphicsItem(const QLineF &line) {
    _centroidBezierAdjustHandledGraphicsItem = new MyCentroidBezierAdjustHandledGraphicsItem(line.p1(), line.p2());
    connect((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem, &MyCentroidBezierAdjustHandledGraphicsItem::startPositionIsUpdated, this, [this] (const QPointF& startPosition) {
        ((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem)->updateStartPosition(startPosition);
        emit bezierAdjustLineIsUpdated(((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem)->line()); });
    connect((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem, &MyCentroidBezierAdjustHandledGraphicsItem::endPositionIsUpdated, this, [this] (const QPointF& endPosition) {
        ((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem)->updateEndPosition(endPosition);
        emit bezierAdjustLineIsUpdated(((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem)->line()); });
    connect((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem, &MyCentroidBezierAdjustHandledGraphicsItem::isPressed, this, [this] () { _isPressed = true; });
    connect((MyCentroidBezierAdjustHandledGraphicsItem*)_centroidBezierAdjustHandledGraphicsItem, &MyCentroidBezierAdjustHandledGraphicsItem::isReleased, this, [this] () { _isPressed = false; });
    addToGroup(_centroidBezierAdjustHandledGraphicsItem);
}

// MyCentroidBezierAdjustHandledGraphicsItem

MyCentroidBezierAdjustHandledGraphicsItem::MyCentroidBezierAdjustHandledGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    createLine(startPoint, endPoint);
    createStartHandle(startPoint);
    createEndHandle(endPoint);
}

void MyCentroidBezierAdjustHandledGraphicsItem::createLine(const QPointF& p1, const QPointF& p2) {
    _line = new MyBezierAdjustLineGraphicsItem(p1.x(), p1.y(), p2.x(), p2.y());
    addToGroup(_line);
}

const QLineF MyCentroidBezierAdjustHandledGraphicsItem::line() {
    return QLineF(startPosition().x(), startPosition().y(), endPosition().x(), endPosition().y());
}

void MyCentroidBezierAdjustHandledGraphicsItem::createStartHandle(const QPointF& center) {
    _startControlHandle = new MyBezierControlPointHandleGraphicsItem(center);
    connect((MyBezierControlPointHandleGraphicsItem*)_startControlHandle, SIGNAL(positionChanged(const QPointF&)), this, SIGNAL(startPositionIsUpdated(const QPointF&)));
    connect((MyBezierControlPointHandleGraphicsItem*)_startControlHandle, SIGNAL(isPressed()), this, SIGNAL(isPressed()));
    connect((MyBezierControlPointHandleGraphicsItem*)_startControlHandle, SIGNAL(isReleased()), this, SIGNAL(isReleased()));
    addToGroup(_startControlHandle);
}

void MyCentroidBezierAdjustHandledGraphicsItem::createEndHandle(const QPointF& center) {
    _endControlHandle = new MyBezierControlPointHandleGraphicsItem(center);
    connect((MyBezierControlPointHandleGraphicsItem*)_endControlHandle, SIGNAL(positionChanged(const QPointF&)), this, SIGNAL(endPositionIsUpdated(const QPointF&)));
    connect((MyBezierControlPointHandleGraphicsItem*)_endControlHandle, SIGNAL(isPressed()), this, SIGNAL(isPressed()));
    connect((MyBezierControlPointHandleGraphicsItem*)_endControlHandle, SIGNAL(isReleased()), this, SIGNAL(isReleased()));
    addToGroup(_endControlHandle);
}

const QPointF MyCentroidBezierAdjustHandledGraphicsItem::startPosition() {
    return ((MyBezierControlPointHandleGraphicsItem*)_startControlHandle)->position();
}

void MyCentroidBezierAdjustHandledGraphicsItem::updateStartPosition(const QPointF& position) {
    QPointF newEndPosition = startPosition() + endPosition() - position;
    ((MyBezierAdjustLineGraphicsItem*)_line)->setLine(position.x(), position.y(), newEndPosition.x(), newEndPosition.y());
    ((MyBezierControlPointHandleGraphicsItem*)_startControlHandle)->updatePosition(position);
    ((MyBezierControlPointHandleGraphicsItem*)_endControlHandle)->updatePosition(newEndPosition);
}

const QPointF MyCentroidBezierAdjustHandledGraphicsItem::endPosition() {
    return ((MyBezierControlPointHandleGraphicsItem*)_endControlHandle)->position();
}

void MyCentroidBezierAdjustHandledGraphicsItem::updateEndPosition(const QPointF& position) {
    QPointF newStartPosition = startPosition() + endPosition() - position;
    ((MyBezierAdjustLineGraphicsItem*)_line)->setLine(newStartPosition.x(), newStartPosition.y(), position.x(), position.y());
    ((MyBezierControlPointHandleGraphicsItem*)_startControlHandle)->updatePosition(newStartPosition);
    ((MyBezierControlPointHandleGraphicsItem*)_endControlHandle)->updatePosition(position);
}
