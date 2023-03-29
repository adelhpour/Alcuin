#include "negui_selection_area_graphics_item.h"

// MySelectionAreaGraphicsItem

MySelectionAreaGraphicsItem::MySelectionAreaGraphicsItem(const QPointF& initialPosition, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    _initialPosition = initialPosition;
    setRect(_initialPosition.x(), _initialPosition.y(), 0.0, 0.0);
}

void MySelectionAreaGraphicsItem::updateExtents(const QPointF& position) {
    //set(_initialPosition, position);
}