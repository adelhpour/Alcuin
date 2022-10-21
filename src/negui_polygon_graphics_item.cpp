#include "negui_polygon_graphics_item.h"
#include "negui_polygon_style.h"

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

void MyPolygonGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(style()->selectedPen());
    else
        setPen(((MyPolygonStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyPolygonGraphicsItem::setSelectedWithFill(const bool& selected) {
    if (selected)
        setBrush(style()->selectedBrush());
    else
        setBrush(((MyPolygonStyleBase*)style())->brush());
    QGraphicsItem::setSelected(selected);
}

void MyPolygonGraphicsItem::updateExtents(const QRectF& extents) {
    _originalPosition = QPointF(extents.x() + 0.5 * extents.width(), extents.y() + 0.5 * extents.height());
    
    if (isSetStyle())
        ((MyPolygonStyleBase*)style())->scaleToExtents(extents);
    
    updateStyle();
}

QRectF MyPolygonGraphicsItem::getExtents() const {
    return boundingRect();
}

void MyPolygonGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
