#include "negui_rect_graphics_item.h"
#include "negui_rect_style.h"
#include "negui_resize_handled_graphics_item.h"

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
        QRectF rect(_originalPosition.x() + ((MyRectStyleBase*)style())->x(), _originalPosition.y() + ((MyRectStyleBase*)style())->y(), ((MyRectStyleBase*)style())->width(), ((MyRectStyleBase*)style())->height());
        
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
        ((MyRectStyleBase*)style())->setX(extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // y
        ((MyRectStyleBase*)style())->setY(extents.y() - (movedDistance().y() + _originalPosition.y()));
        
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
    return QRectF(((MyRectStyleBase*)style())->x() + (movedDistance().x() + _originalPosition.x()), ((MyRectStyleBase*)style())->y() + (movedDistance().y() + _originalPosition.y()), ((MyRectStyleBase*)style())->width(), ((MyRectStyleBase*)style())->height());
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

void MyRectGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyRectStyleBase*)style())->setX(((MyRectStyleBase*)style())->x() - originalPositionMovedDistance.x());
    ((MyRectStyleBase*)style())->setY(((MyRectStyleBase*)style())->y() - originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyRectGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyRoundedRectangleResizeHandledGraphicsItem(focusedRect, ((MyRectStyleBase*)style())->rx(), ((MyRectStyleBase*)style())->ry(), zValue());
    connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    connect(focusedGraphicsItem, SIGNAL(curvatureRadiiAreUpdated(const qreal&, const qreal&)), this, SLOT(updateCurvatureRadii(const qreal&, const qreal&)));

    return focusedGraphicsItem;
}

void MyRectGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}
