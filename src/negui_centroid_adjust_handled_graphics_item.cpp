#include "negui_centroid_adjust_handled_graphics_item.h"

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
    _bezierAdjustLineFocusedGraphicsItem = new QGraphicsLineItem(line);
    ((QGraphicsLineItem*)_bezierAdjustLineFocusedGraphicsItem)->setPen(QPen(QColor("#0000CD")));
    addToGroup(_bezierAdjustLineFocusedGraphicsItem);
}
