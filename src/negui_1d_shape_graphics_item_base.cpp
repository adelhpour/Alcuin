#include "negui_1d_shape_graphics_item_base.h"

// My1DShapeGraphicsItemBase

My1DShapeGraphicsItemBase::My1DShapeGraphicsItemBase() {
    _endSlope = 0.0;
}

const qreal My1DShapeGraphicsItemBase::getEndSlope() const {
    return _endSlope;
}

QGraphicsItem* My1DShapeGraphicsItemBase::getResizeHandlebaredGraphicsItem() {
    return NULL;
}

void My1DShapeGraphicsItemBase::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    
}

void My1DShapeGraphicsItemBase::updateExtents(const QRectF& extents) {
    
}
