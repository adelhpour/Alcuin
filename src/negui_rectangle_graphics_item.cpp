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
        
        // border-radius-x
        qreal borderRadiusX = ((MyRectangleStyleBase*)style())->borderRadiusX();
        
        // border-radius-y
        qreal borderRadiusY = ((MyRectangleStyleBase*)style())->borderRadiusY();
        
        QPainterPath _path;
        _path.addRoundedRect(rect, borderRadiusX, borderRadiusY);
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
        ((MyRectangleStyleBase*)style())->setX(extents.x() - (_originalPosition.x() + _movedOriginalPosition.x()));
        
        // y
        ((MyRectangleStyleBase*)style())->setY(extents.y() - (_originalPosition.y() + _movedOriginalPosition.y()));
        
        // border-radius-x
        ((MyRectangleStyleBase*)style())->setBorderRadiusX((extents.width()/ ((MyRectangleStyleBase*)style())->width()) * ((MyRectangleStyleBase*)style())->borderRadiusX());
        
        // border-radius-y
        ((MyRectangleStyleBase*)style())->setBorderRadiusY((extents.height()/ ((MyRectangleStyleBase*)style())->height()) * ((MyRectangleStyleBase*)style())->borderRadiusY());
        
        // width
        ((MyRectangleStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyRectangleStyleBase*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyRectangleGraphicsItem::getExtents() {
    return  QRectF(((MyRectangleStyleBase*)style())->x() + _originalPosition.x() + _movedOriginalPosition.x(),
                   ((MyRectangleStyleBase*)style())->y() + _originalPosition.y() + _movedOriginalPosition.y(),
                   ((MyRectangleStyleBase*)style())->width(), ((MyRectangleStyleBase*)style())->height());
}

void MyRectangleGraphicsItem::updateBorderRadii(const qreal& borderRadiusX, const qreal& borderRadiusY) {
    if (isSetStyle()) {
        // border-radius-x
        ((MyRectangleStyleBase*)style())->setBorderRadiusX(borderRadiusX);
        
        // border-radius-y
        ((MyRectangleStyleBase*)style())->setBorderRadiusY(borderRadiusY);
    }
    
    updateStyle();
}

void MyRectangleGraphicsItem::moveOriginalPosition(const qreal& dx, const qreal& dy) {
    _movedOriginalPosition += QPointF(dx, dy);
}

QGraphicsItem* MyRectangleGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyRoundedRectangleResizeHandledGraphicsItem(focusedRect, ((MyRectangleStyleBase*)style())->borderRadiusX(), ((MyRectangleStyleBase*)style())->borderRadiusY(), zValue());
    connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    connect(focusedGraphicsItem, SIGNAL(borderRadiiAreUpdated(const qreal&, const qreal&)), this, SLOT(updateBorderRadii(const qreal&, const qreal&)));

    return focusedGraphicsItem;
}

void MyRectangleGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}
