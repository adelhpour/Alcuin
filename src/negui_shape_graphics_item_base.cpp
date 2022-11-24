#include "negui_shape_graphics_item_base.h"

// MyShapeGraphicsItemBase

MyShapeGraphicsItemBase::MyShapeGraphicsItemBase() {
    _style = NULL;
    _isSetStyle = false;
    _movedDistance = QPointF(0.0, 0.0);
}

MyShapeStyleBase* MyShapeGraphicsItemBase::style() {
    return _style;
}

void MyShapeGraphicsItemBase::setStyle(MyShapeStyleBase* style) {
    _style = style;
    _isSetStyle = true;
}

void MyShapeGraphicsItemBase::setMovedDistance(const QPointF& movedDistance) {
    _movedDistance = movedDistance;
}

const QPointF& MyShapeGraphicsItemBase::movedDistance() const {
    return _movedDistance;
}
