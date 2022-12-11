#include "negui_ellipse_graphics_item.h"
#include "negui_ellipse_style.h"
#include "negui_resize_handlebared_graphics_item.h"

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
        setRect(_originalPosition.x() + ((MyEllipseStyleBase*)style())->cx() - ((MyEllipseStyleBase*)style())->rx(), _originalPosition.y() + ((MyEllipseStyleBase*)style())->cy() - ((MyEllipseStyleBase*)style())->ry(), 2 * ((MyEllipseStyleBase*)style())->rx(), 2 * ((MyEllipseStyleBase*)style())->ry());
    }
}

void MyEllipseGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(style()->selectedPen());
    else
        setPen(((MyEllipseStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyEllipseGraphicsItem::setSelectedWithFill(const bool& selected) {
    if (selected)
        setBrush(style()->selectedBrush());
    else
        setBrush(((MyEllipseStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyEllipseGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // cx
        ((MyEllipseStyleBase*)style())->setCx(0.5 * extents.width() + extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // cy
        ((MyEllipseStyleBase*)style())->setCy(0.5 * extents.height() + extents.y() - (movedDistance().y() + _originalPosition.y()));
        
        // rx
        ((MyEllipseStyleBase*)style())->setRx(0.5 * extents.width());
        
        // ry
        ((MyEllipseStyleBase*)style())->setRy(0.5 * extents.height());
    }
    
    updateStyle();
}

QRectF MyEllipseGraphicsItem::getExtents() {
    return QRectF(((MyEllipseStyleBase*)style())->cx() - ((MyEllipseStyleBase*)style())->rx() + (movedDistance().x() + _originalPosition.x()), ((MyEllipseStyleBase*)style())->cy() - ((MyEllipseStyleBase*)style())->ry() + (movedDistance().y() + _originalPosition.y()), 2 * ((MyEllipseStyleBase*)style())->rx(), 2 * ((MyEllipseStyleBase*)style())->ry());
}

void MyEllipseGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyEllipseStyleBase*)style())->setCx(((MyEllipseStyleBase*)style())->cx() - originalPositionMovedDistance.x());
    ((MyEllipseStyleBase*)style())->setCy(((MyEllipseStyleBase*)style())->cy() - originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyEllipseGraphicsItem::getResizeHandlebaredGraphicsItem() {
    QRectF resizeRect = getExtents();
    MyResizeHandlebaredGraphicsItemBase* resizeHandlebaredGraphicsItem = new MyResizeHandlebaredGraphicsItem(resizeRect, zValue());
    connect(resizeHandlebaredGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    
    return resizeHandlebaredGraphicsItem;
}

void MyEllipseGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
