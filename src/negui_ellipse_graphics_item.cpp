#include "negui_ellipse_graphics_item.h"
#include "negui_ellipse_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyEllipseGraphicsItem

MyEllipseGraphicsItem::MyEllipseGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsEllipseItem(parent) {
}

void MyEllipseGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyEllipseStyleBase*)style())->pen());
        
        // brush
        setBrush(((MyEllipseStyleBase*)style())->brush());
        
        // position and dimensions
        setRect(_originalPosition.x() + ((MyEllipseStyleBase*)style())->centerX() - ((MyEllipseStyleBase*)style())->radiusX(),
                _originalPosition.y() + ((MyEllipseStyleBase*)style())->centerY() - ((MyEllipseStyleBase*)style())->radiusY(),
                2 * ((MyEllipseStyleBase*)style())->radiusX(), 2 * ((MyEllipseStyleBase*)style())->radiusY());
    }
}

void MyEllipseGraphicsItem::setSelectedWithBorderColor(const bool& selected) {
    if (selected)
        setPen(((MyEllipseStyleBase*)style())->selectedPen());
    else
        setPen(((MyEllipseStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyEllipseGraphicsItem::setSelectedWithFillColor(const bool& selected) {
    if (selected)
        setBrush(((MyEllipseStyleBase*)style())->selectedBrush());
    else
        setBrush(((MyEllipseStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyEllipseGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // center-x
        ((MyEllipseStyleBase*)style())->setCenterX(0.5 * extents.width() + extents.x() - _originalPosition.x() + _movedOriginalPosition.x());
        
        // center-y
        ((MyEllipseStyleBase*)style())->setCenterY(0.5 * extents.height() + extents.y() - _originalPosition.y() + _movedOriginalPosition.y());
        
        // radius-x
        ((MyEllipseStyleBase*)style())->setRadiusX(0.5 * extents.width());
        
        // radius-y
        ((MyEllipseStyleBase*)style())->setRadiusY(0.5 * extents.height());
    }
    
    updateStyle();
}

QRectF MyEllipseGraphicsItem::getExtents() {
    return QRectF(((MyEllipseStyleBase*)style())->centerX() - ((MyEllipseStyleBase*)style())->radiusX() + _originalPosition.x() + _movedOriginalPosition.x(),
                  ((MyEllipseStyleBase*)style())->centerY() - ((MyEllipseStyleBase*)style())->radiusY() + _originalPosition.y() + _movedOriginalPosition.y(),
                  2 * ((MyEllipseStyleBase*)style())->radiusX(), 2 * ((MyEllipseStyleBase*)style())->radiusY());
}

void MyEllipseGraphicsItem::moveOriginalPosition(const qreal& dx, const qreal& dy) {
    _movedOriginalPosition += QPointF(dx, dy);
}

QGraphicsItem* MyEllipseGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyResizeHandledGraphicsItem(focusedRect, zValue());
    connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));

    return focusedGraphicsItem;
}

void MyEllipseGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
