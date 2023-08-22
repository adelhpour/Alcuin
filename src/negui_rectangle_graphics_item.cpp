#include "negui_rectangle_graphics_item.h"
#include "negui_rectangle_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyRectangleGraphicsItem

MyRectangleGraphicsItem::MyRectangleGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsPathItem(parent) {
    
}

void MyRectangleGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyRectangleStyleBase*)style())->pen());
        
        // brush
        setBrush(((MyRectangleStyleBase*)style())->brush());
        
        // position and dimensions
        QRectF rect(_originalPosition.x() + ((MyRectangleStyleBase*)style())->x(), _originalPosition.y() + ((MyRectangleStyleBase*)style())->y(), ((MyRectangleStyleBase*)style())->width(), ((MyRectangleStyleBase*)style())->height());
        
        // rx
        qreal rx = ((MyRectangleStyleBase*)style())->rx();
        
        // ry
        qreal ry = ((MyRectangleStyleBase*)style())->ry();
        
        QPainterPath _path;
        _path.addRoundedRect(rect, rx, ry);
        QGraphicsPathItem::setPath(_path);
    }
}

void MyRectangleGraphicsItem::setSelectedWithBorderColor(const bool& selected) {
    if (selected)
        setPen(((MyRectangleStyleBase*)style())->selectedPen());
    else
        setPen(((MyRectangleStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyRectangleGraphicsItem::setSelectedWithFillColor(const bool& selected) {
    if (selected)
        setBrush(((MyRectangleStyleBase*)style())->selectedBrush());
    else
        setBrush(((MyRectangleStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyRectangleGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // x
        ((MyRectangleStyleBase*)style())->setX(extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // y
        ((MyRectangleStyleBase*)style())->setY(extents.y() - (movedDistance().y() + _originalPosition.y()));
        
        // rx
        ((MyRectangleStyleBase*)style())->setRx((extents.width()/ ((MyRectangleStyleBase*)style())->width()) * ((MyRectangleStyleBase*)style())->rx());
        
        // ry
        ((MyRectangleStyleBase*)style())->setRy((extents.height()/ ((MyRectangleStyleBase*)style())->height()) * ((MyRectangleStyleBase*)style())->ry());
        
        // width
        ((MyRectangleStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyRectangleStyleBase*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyRectangleGraphicsItem::getExtents() {
    return QRectF(((MyRectangleStyleBase*)style())->x() + (movedDistance().x() + _originalPosition.x()), ((MyRectangleStyleBase*)style())->y() + (movedDistance().y() + _originalPosition.y()), ((MyRectangleStyleBase*)style())->width(), ((MyRectangleStyleBase*)style())->height());
}

void MyRectangleGraphicsItem::updateCurvatureRadii(const qreal& radiusX, const qreal& radiusY) {
    if (isSetStyle()) {
        // rx
        ((MyRectangleStyleBase*)style())->setRx(radiusX);
        
        // ry
        ((MyRectangleStyleBase*)style())->setRy(radiusY);
    }
    
    updateStyle();
}

void MyRectangleGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyRectangleStyleBase*)style())->setX(((MyRectangleStyleBase*)style())->x() - originalPositionMovedDistance.x());
    ((MyRectangleStyleBase*)style())->setY(((MyRectangleStyleBase*)style())->y() - originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyRectangleGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyRoundedRectangleResizeHandledGraphicsItem(focusedRect, ((MyRectangleStyleBase*)style())->rx(), ((MyRectangleStyleBase*)style())->ry(), zValue());
    connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    connect(focusedGraphicsItem, SIGNAL(curvatureRadiiAreUpdated(const qreal&, const qreal&)), this, SLOT(updateCurvatureRadii(const qreal&, const qreal&)));

    return focusedGraphicsItem;
}

void MyRectangleGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}
