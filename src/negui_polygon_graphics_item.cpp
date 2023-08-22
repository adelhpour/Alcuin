#include "negui_polygon_graphics_item.h"
#include "negui_polygon_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyPolygonGraphicsItem

MyPolygonGraphicsItem::MyPolygonGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x,y), QGraphicsPolygonItem(parent) {
}

void MyPolygonGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyPolygonStyleBase*)style())->pen());
        
        // brush
        setBrush(((MyPolygonStyleBase*)style())->brush());
        
        // polygon
        if (((MyPolygonStyleBase*)style())->points().size() > 2) {
            QPolygonF polygon;
            QList<QPointF> points = ((MyPolygonStyleBase*)style())->points();
            for (QPointF point : qAsConst(points))
                polygon << QPointF(_originalPosition.x() + point.x(), _originalPosition.y() + point.y());
            setPolygon(polygon);
        }
    }
}

void MyPolygonGraphicsItem::setSelectedWithBorderColor(const bool& selected) {
    if (selected)
        setPen(((MyPolygonStyleBase*)style())->selectedPen());
    else
        setPen(((MyPolygonStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyPolygonGraphicsItem::setSelectedWithFillColor(const bool& selected) {
    if (selected)
        setBrush(((MyPolygonStyleBase*)style())->selectedBrush());
    else
        setBrush(((MyPolygonStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyPolygonGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle())
        ((MyPolygonStyleBase*)style())->scaleToExtents(QRectF(extents.x() - (movedDistance().x() + boundingRect().x()), extents.y() - (movedDistance().y() + boundingRect().y()), extents.width(), extents.height()));
    
    updateStyle();
}

QRectF MyPolygonGraphicsItem::getExtents() {
    QRectF polygonBoundingRect = ((MyPolygonStyleBase*)style())->getShapeExtents();
    return QRectF((movedDistance().x() + boundingRect().x()), (movedDistance().y() + boundingRect().y()), polygonBoundingRect.width(), polygonBoundingRect.height());
}

void MyPolygonGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyPolygonStyleBase*)style())->moveBy(-originalPositionMovedDistance.x(), -originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyPolygonGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyResizeHandledGraphicsItem(focusedRect, zValue());
    connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), this, SLOT(updateExtents(const QRectF&)));
    
    return focusedGraphicsItem;
}

void MyPolygonGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
