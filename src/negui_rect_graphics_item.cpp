#include "negui_rect_graphics_item.h"
#include "negui_rect_style.h"
#include "negui_resize_handlebared_graphics_item.h"

// MyRectGraphicsItem

MyRectGraphicsItem::MyRectGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsPathItem(parent) {
    
}

void MyRectGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyRectStyleBase*)style())->pen());
        
        // brush
        setBrush(((MyRectStyleBase*)style())->brush());
        
        // position and dimensions
        QRectF rect(_originalPosition.x() + ((MyRectStyleBase*)style())->x() - 0.5 * ((MyRectStyleBase*)style())->width(), _originalPosition.y() + ((MyRectStyleBase*)style())->y() - 0.5 * ((MyRectStyleBase*)style())->height(), ((MyRectStyleBase*)style())->width(), ((MyRectStyleBase*)style())->height());
        
        // rx
        qreal rx = ((MyRectStyleBase*)style())->rx();
        
        // ry
        qreal ry = ((MyRectStyleBase*)style())->ry();
        
        QPainterPath _path;
        _path.addRoundedRect(rect, rx, ry);
        QGraphicsPathItem::setPath(_path);
    }
}

void MyRectGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(style()->selectedPen());
    else
        setPen(((MyRectStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyRectGraphicsItem::setSelectedWithFill(const bool& selected) {
    if (selected)
        setBrush(style()->selectedBrush());
    else
        setBrush(((MyRectStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyRectGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // x
        ((MyRectStyleBase*)style())->setX(extents.x() - (movedDistance().x() + _originalPosition.x() - 0.5 * ((MyRectStyleBase*)style())->width()));
        
        // y
        ((MyRectStyleBase*)style())->setY(extents.y() - (movedDistance().y() + _originalPosition.y() - 0.5 * ((MyRectStyleBase*)style())->height()));
        
        // rx
        ((MyRectStyleBase*)style())->setRx((extents.width()/ ((MyRectStyleBase*)style())->width()) * ((MyRectStyleBase*)style())->rx());
        
        // ry
        ((MyRectStyleBase*)style())->setRy((extents.height()/ ((MyRectStyleBase*)style())->height()) * ((MyRectStyleBase*)style())->ry());
        
        // width
        ((MyRectStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyRectStyleBase*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyRectGraphicsItem::getExtents() {
    return boundingRect();
}

void MyRectGraphicsItem::updateCurvatureRadii(const qreal& radiusX, const qreal& radiusY) {
    if (isSetStyle()) {
        // rx
        ((MyRectStyleBase*)style())->setRx(radiusX);
        
        // ry
        ((MyRectStyleBase*)style())->setRy(radiusY);
    }
    
    updateStyle();
}

QGraphicsItem* MyRectGraphicsItem::getResizeHandlebaredGraphicsItem() {
    QRectF resizeRect = QRectF(boundingRect().x() + movedDistance().x(), boundingRect().y() + movedDistance().y(), boundingRect().width(), boundingRect().height());
    MyResizeHandlebaredGraphicsItemBase* resizeHandlebaredGraphicsItem = new MyRoundedRectangleResizeHandlebaredGraphicsItem(resizeRect, ((MyRectStyleBase*)style())->rx(), ((MyRectStyleBase*)style())->ry(), zValue());
    connect(resizeHandlebaredGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    connect(resizeHandlebaredGraphicsItem, SIGNAL(curvatureRadiiAreUpdated(const qreal&, const qreal&)), this, SLOT(updateCurvatureRadii(const qreal&, const qreal&)));
    
    return resizeHandlebaredGraphicsItem;
}

void MyRectGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
