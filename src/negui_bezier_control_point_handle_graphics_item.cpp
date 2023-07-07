#include "negui_bezier_control_point_handle_graphics_item.h"

#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

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

    updatePosition(center);
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
