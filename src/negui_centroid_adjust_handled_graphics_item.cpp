#include "negui_centroid_adjust_handled_graphics_item.h"
#include "negui_bezier_control_point_handle_graphics_item.h"

#include <QPen>

MyCentroidAdjustHandledGraphicsItems::MyCentroidAdjustHandledGraphicsItems(const QRectF &rect, const QLineF &line, qreal zValue, QGraphicsItem *parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createCentroidFocusedGraphicsItem(rect);
    createBezierAdjustLineFocusedGraphicsItem(line);
    createBezierControlPointHandleGraphicsItem(line.p1());
    createBezierControlPointHandleGraphicsItem(line.p2());
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

void MyCentroidAdjustHandledGraphicsItems::createBezierControlPointHandleGraphicsItem(const QPointF& point) {
    _controlHandle = new MyBezierControlPointHandleGraphicsItem(point);
    //connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(positionChanged(const QPointF&)), this, SIGNAL(adjustHandledGraphicsItemIsUpdated(const QPointF&)));
    //connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(isPressed()), this, SIGNAL(isPressed()));
    //connect((MyBezierControlPointHandleGraphicsItem*)_controlHandle, SIGNAL(isReleased()), this, SIGNAL(isReleased()));
    addToGroup(_controlHandle);
}
