#include "negui_selection_area_graphics_item.h"

// MySelectionAreaGraphicsItem

MySelectionAreaGraphicsItem::MySelectionAreaGraphicsItem(const QPointF& initialPosition, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    _initialPosition = initialPosition;
    setRect(_initialPosition.x(), _initialPosition.y(), 0.0, 0.0);
    setPen(defaultPen());
    setBrush(defaultBrush());
}

void MySelectionAreaGraphicsItem::updateExtents(const QPointF& position) {
    setRect(qMin(_initialPosition.x(), position.x()), qMin(_initialPosition.y(), position.y()), qAbs(position.x() - _initialPosition.x()), qAbs(position.y() - _initialPosition.y()));
}

const QPen MySelectionAreaGraphicsItem::defaultPen() {
    return QPen(QColor("#4169e1"));
}

const QBrush MySelectionAreaGraphicsItem::defaultBrush() {
    return QBrush(QColor("#87CEEB"), Qt::SolidPattern);
}