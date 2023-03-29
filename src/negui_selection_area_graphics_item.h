#ifndef __NEGUI_SELECTION_AREA_GRAPHICS_ITEM_H
#define __NEGUI_SELECTION_AREA_GRAPHICS_ITEM_H

#include <QGraphicsRectItem>

class MySelectionAreaGraphicsItem : public QGraphicsRectItem {

public:
    MySelectionAreaGraphicsItem(const QPointF& initialPosition, QGraphicsItem *parent=nullptr);

    void updateExtents(const QPointF& position);

protected:
    QPointF _initialPosition;
};

#endif
