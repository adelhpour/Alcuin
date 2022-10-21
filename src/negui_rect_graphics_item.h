#ifndef __NEGUI_RECT_GRAPHICS_ITEM_H
#define __NEGUI_RECT_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyRectGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsPathItem {
public:
    
    MyRectGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    void updateExtents(const QRectF& extents) override;
    
    QRectF getExtents() const override;
    
    void setZValue(qreal z) override;
};

#endif
