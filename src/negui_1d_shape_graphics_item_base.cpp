#include "negui_1d_shape_graphics_item_base.h"

// My1DShapeGraphicsItemBase

My1DShapeGraphicsItemBase::My1DShapeGraphicsItemBase() {
    _endSlope = 0.0;
}

const qreal My1DShapeGraphicsItemBase::getEndSlope() const {
    return _endSlope;
}
