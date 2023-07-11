#include "negui_bezier_adjust_customized_graphics_items.h"

#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

// MyBezierControlPointHandleGraphicsItem

MyBezierControlPointHandleGraphicsItem::MyBezierControlPointHandleGraphicsItem(const QPointF& center, QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    _handleRadius = 2.0;
    setPen();
    setBrush();

    updatePosition(center);
}

void MyBezierControlPointHandleGraphicsItem::setPen() {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    QAbstractGraphicsShapeItem::setPen(pen);
}

void MyBezierControlPointHandleGraphicsItem::setBrush() {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    QAbstractGraphicsShapeItem::setBrush(brush);
}

void MyBezierControlPointHandleGraphicsItem::updatePosition(const QPointF& center) {
    setRect(QRectF(center.x() - _handleRadius, center.y() - _handleRadius, 2 * _handleRadius, 2 * _handleRadius));
}

const QPointF MyBezierControlPointHandleGraphicsItem::position() {
    return rect().center();
}

void MyBezierControlPointHandleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    emit isPressed();
}

void MyBezierControlPointHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
    emit positionChanged(event->pos());
}

void MyBezierControlPointHandleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    emit isReleased();
}

//MyBezierAdjustLineGraphicsItem

MyBezierAdjustLineGraphicsItem::MyBezierAdjustLineGraphicsItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) : QGraphicsLineItem(x1, y1, x2, y2, parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    pen.setStyle(Qt::DashLine);
    setPen(pen);
}
