#include "negui_shape_graphics_item_base.h"

// MyShapeGraphicsItemBase

MyShapeGraphicsItemBase::MyShapeGraphicsItemBase() {
    _style = NULL;
    _isSetStyle = false;
}

MyShapeStyleBase* MyShapeGraphicsItemBase::style() {
    return _style;
}

void MyShapeGraphicsItemBase::setStyle(MyShapeStyleBase* style) {
    _style = style;
    _isSetStyle = true;
}

void MyShapeGraphicsItemBase::updateExtents(const QRectF& extents) {
    
}

void MyShapeGraphicsItemBase::setLine(const QLineF& line) {
    
}

const qreal MyShapeGraphicsItemBase::getEndSlope() const {
    return 0.0;
}
