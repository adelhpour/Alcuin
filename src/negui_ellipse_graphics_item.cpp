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
        setRect(_originalPosition.x() + ((MyEllipseStyleBase*)style())->cx() - 2 * ((MyEllipseStyleBase*)style())->rx(), _originalPosition.y() + ((MyEllipseStyleBase*)style())->cy() - 2 * ((MyEllipseStyleBase*)style())->ry(), 2 * ((MyEllipseStyleBase*)style())->rx(), 2 * ((MyEllipseStyleBase*)style())->ry());
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
        ((MyEllipseStyleBase*)style())->setCx(extents.width() + extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // cy
        ((MyEllipseStyleBase*)style())->setCy(extents.height() + extents.y() - (movedDistance().y() + _originalPosition.y()));
        
        // rx
        ((MyEllipseStyleBase*)style())->setRx(0.5 * extents.width());
        
        // ry
        ((MyEllipseStyleBase*)style())->setRy(0.5 * extents.height());
    }
    
    updateStyle();
}

QRectF MyEllipseGraphicsItem::getExtents() {
    return boundingRect();
}

QGraphicsItem* MyEllipseGraphicsItem::getResizeHandlebaredGraphicsItem() {
    QRectF resizeRect = QRectF(boundingRect().x() + movedDistance().x(), boundingRect().y() + movedDistance().y(), boundingRect().width(), boundingRect().height());
    MyResizeHandlebaredGraphicsItemBase* resizeHandlebaredGraphicsItem = new MyResizeHandlebaredGraphicsItem(resizeRect, zValue());
    connect(resizeHandlebaredGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    
    return resizeHandlebaredGraphicsItem;
}

void MyEllipseGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
