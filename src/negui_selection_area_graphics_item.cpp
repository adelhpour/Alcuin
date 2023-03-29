#include "negui_selection_area_graphics_item.h"

// MySelectionAreaGraphicsItem

MySelectionAreaGraphicsItem::MySelectionAreaGraphicsItem(const QPointF& initialPosition, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    _initialPosition = initialPosition;
    setRect(_initialPosition.x(), _initialPosition.y(), 0.0, 0.0);
}

void MySelectionAreaGraphicsItem::updateExtents(const QPointF& position) {
    setRect(qMin(_initialPosition.x(), position.x()), qMin(_initialPosition.y(), position.y()), qAbs(position.x() - _initialPosition.x()), qAbs(position.y() - _initialPosition.y()));
}