#ifndef __NEGUI_SELECTION_AREA_GRAPHICS_ITEM_H
#define __NEGUI_SELECTION_AREA_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>

class MySelectionAreaGraphicsItem : public QGraphicsRectItem {

public:
    MySelectionAreaGraphicsItem(const QPointF& initialPosition, QGraphicsItem *parent=nullptr);

    void updateExtents(const QPointF& position);

    const QPen defaultPen();

    const QBrush defaultBrush();

protected:
    QPointF _initialPosition;
};

#endif
