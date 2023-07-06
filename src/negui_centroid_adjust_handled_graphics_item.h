#ifndef __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyCentroidAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MyCentroidAdjustHandledGraphicsItems(const QRectF &rect, const QLineF &line, qreal zValue, QGraphicsItem *parent = nullptr);

    void createCentroidFocusedGraphicsItem(const QRectF &rect);

protected:
    QGraphicsItem* _centroidFocusedGraphicsItem;
    bool _isPressed;
};
#endif
