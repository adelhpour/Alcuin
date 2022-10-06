#include "negui_rect_graphics_item.h"

// MyRectGraphicsItem

MyRectGraphicsItem::MyRectGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : QGraphicsPathItem(parent) {
    _originalPosition = QPointF(x, y);
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
    _originalPosition = QPointF(extents.x() + 0.5 * extents.width(), extents.y() + 0.5 * extents.height());
    
    if (isSetStyle()) {
        // x
        ((MyRectStyleBase*)style())->setX(0.0);
        
        // y
        ((MyRectStyleBase*)style())->setY(0.0);
        
        // width
        ((MyRectStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyRectStyleBase*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyRectGraphicsItem::getExtents() const {
    return boundingRect();
}
