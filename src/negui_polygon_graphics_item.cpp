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
        ((MyPolygonStyleBase*)style())->scaleToExtents(QRectF(extents.x() - (_originalPosition.x() + _movedOriginalPosition.x()),
                                                       extents.y() - (_originalPosition.y() + _movedOriginalPosition.y()),
                                                       extents.width(), extents.height()));
    
    updateStyle();
}

QRectF MyPolygonGraphicsItem::getExtents() {
    QRectF polygonBoundingRect = ((MyPolygonStyleBase*)style())->getShapeExtents();
    return QRectF(polygonBoundingRect.x() + (_originalPosition.x() + _movedOriginalPosition.x()),
                  polygonBoundingRect.y() + (_originalPosition.y() + _movedOriginalPosition.y()),
                  polygonBoundingRect.width(), polygonBoundingRect.height());
}

void MyPolygonGraphicsItem::updateOriginalPosition(const QPointF originalPosition) {
    QRectF polygonBoundingRect = ((MyPolygonStyleBase*)style())->getShapeExtents();
    ((MyPolygonStyleBase*)style())->scaleToExtents(QRectF(polygonBoundingRect.x() - (originalPosition - (_originalPosition + _movedOriginalPosition)).x(),
                                                          polygonBoundingRect.y() - (originalPosition - (_originalPosition + _movedOriginalPosition)).y(),
                                                          polygonBoundingRect.width(), polygonBoundingRect.height()));
    _originalPosition = originalPosition - _movedOriginalPosition;
}

void MyPolygonGraphicsItem::moveOriginalPosition(const qreal& dx, const qreal& dy) {
    _movedOriginalPosition += QPointF(dx, dy);
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
