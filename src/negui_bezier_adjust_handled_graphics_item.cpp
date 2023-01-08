#include "negui_bezier_adjust_handled_graphics_item.h"
#include <QPen>
#include <QCursor>

// MyBezierAdjustHandledGraphicsItems

MyBezierAdjustHandledGraphicsItems::MyBezierAdjustHandledGraphicsItems(const QPointF& startPoint, const QPointF& controlPoint1, const QPointF& controlPoint2, const QPointF& endPoint, qreal zValue, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createBezierStartAdjustHandledGraphicsItem(startPoint, controlPoint1);
    createBezierEndAdjustHandledGraphicsItem(endPoint, controlPoint2);
}

void MyBezierAdjustHandledGraphicsItems::createBezierStartAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint) {
    _startAdjustHandledGraphicsItem = new MyBezierAdjustHandledGraphicsItem(point, controlPoint);
    connect((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem, SIGNAL(adjustHandledGraphicsItemIsUpdated(const QPointF&)), this, SLOT(updateStartAdjustHandledGraphicsItem(const QPointF&)));
    addToGroup(_startAdjustHandledGraphicsItem);
}

void MyBezierAdjustHandledGraphicsItems::createBezierEndAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint) {
    _endAdjustHandledGraphicsItem = new MyBezierAdjustHandledGraphicsItem(point, controlPoint);
    connect((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem, SIGNAL(adjustHandledGraphicsItemIsUpdated(const QPointF&)), this, SLOT(updateEndAdjustHandledGraphicsItem(const QPointF&)));
    addToGroup(_endAdjustHandledGraphicsItem);
}

void MyBezierAdjustHandledGraphicsItems::updateStartAdjustHandledGraphicsItem(const QPointF& startControlPoint) {
    ((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem)->updatePostion(startControlPoint);
    emit controlPointsAreUpdated(startControlPoint, ((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem)->position());
}

void MyBezierAdjustHandledGraphicsItems::updateEndAdjustHandledGraphicsItem(const QPointF& endControlPoint) {
    ((MyBezierAdjustHandledGraphicsItem*)_endAdjustHandledGraphicsItem)->updatePostion(endControlPoint);
    emit controlPointsAreUpdated(((MyBezierAdjustHandledGraphicsItem*)_startAdjustHandledGraphicsItem)->position(), endControlPoint);
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
    connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(positionChanged(const QPointF&)), this, SIGNAL(adjustHandledGraphicsItemIsUpdated(const QPointF&)));
    addToGroup(_controlHandle);
}

void MyBezierAdjustHandledGraphicsItem::updatePostion(const QPointF& controlPoint) {
    ((MyBezierAdjustLineGraphicsItem*)_line)->setLine(((MyBezierAdjustLineGraphicsItem*)_line)->line().p1().x(), ((MyBezierAdjustLineGraphicsItem*)_line)->line().p1().y(), controlPoint.x(), controlPoint.y());
    ((MyBezierControlPointHandleGraphicsItem*)_controlHandle)->updatePostion(controlPoint);
}

const QPointF MyBezierAdjustHandledGraphicsItem::position() {
    return ((MyBezierControlPointHandleGraphicsItem*)_controlHandle)->position();
}

//MyBezierAdjustLineGraphicsItem

MyBezierAdjustLineGraphicsItem::MyBezierAdjustLineGraphicsItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) : QGraphicsLineItem(x1, y1, x2, y2, parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    pen.setStyle(Qt::DashLine);
    setPen(pen);
}

// MyBezierControlPointHandleGraphicsItem

MyBezierControlPointHandleGraphicsItem::MyBezierControlPointHandleGraphicsItem(const QPointF& center, QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    _handleRadius = 2.0;
    
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    setPen(pen);
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    setBrush(brush);
    
    updatePostion(center);
}

void MyBezierControlPointHandleGraphicsItem::updatePostion(const QPointF& center) {
    setRect(QRectF(center.x() - _handleRadius, center.y() - _handleRadius, 2 * _handleRadius, 2 * _handleRadius));
}

const QPointF MyBezierControlPointHandleGraphicsItem::position() {
    return rect().center();
}

void MyBezierControlPointHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
    emit positionChanged(event->pos());
}