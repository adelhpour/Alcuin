#ifndef __NEGUI_POLYGON_GRAPHICS_ITEM_H
#define __NEGUI_POLYGON_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyPolygonGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsPolygonItem {
public:
    
    MyPolygonGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    void updateExtents(const QRectF& extents) override;
    
    QRectF getExtents() const override;
    
    void setZValue(qreal z) override;
};

#endif
