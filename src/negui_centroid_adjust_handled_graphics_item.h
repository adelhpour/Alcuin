#ifndef __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>

class MyCentroidAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MyCentroidAdjustHandledGraphicsItems(const QRectF &rect, const QLineF &line, qreal zValue, QGraphicsItem *parent = nullptr);

    void createCentroidFocusedGraphicsItem(const QRectF &rect);

    void createBezierAdjustLineFocusedGraphicsItem(const QLineF &line);

    void createBezierControlPointHandleGraphicsItem(const QPointF& point);

protected:
    QGraphicsItem* _centroidFocusedGraphicsItem;
    QGraphicsItem* _bezierAdjustLineFocusedGraphicsItem;
    QGraphicsItem* _controlHandle;
    bool _isPressed;
};
#endif
