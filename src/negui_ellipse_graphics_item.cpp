#include "negui_ellipse_graphics_item.h"

// MyEllipseGraphicsItem

MyEllipseGraphicsItem::MyEllipseGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    _originalPosition = QPointF(x, y);
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
    _originalPosition = QPointF(extents.x() + 0.5 * extents.width(), extents.y() + 0.5 * extents.height());
    
    if (isSetStyle()) {
        // cx
        ((MyEllipseStyleBase*)style())->setCx(0.5 * extents.width());
        
        // cy
        ((MyEllipseStyleBase*)style())->setCy(0.5 * extents.height());
        
        // rx
        ((MyEllipseStyleBase*)style())->setRx(0.5 * extents.width());
        
        // ry
        ((MyEllipseStyleBase*)style())->setRy(0.5 * extents.height());
    }
    
    updateStyle();
}

QRectF MyEllipseGraphicsItem::getExtents() const {
    return boundingRect();
}
