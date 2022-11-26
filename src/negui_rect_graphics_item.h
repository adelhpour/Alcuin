#ifndef __NEGUI_RECT_GRAPHICS_ITEM_H
#define __NEGUI_RECT_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"

class MyRectGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsPathItem {
    Q_OBJECT
    
public:
    
    MyRectGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getResizeHandlebaredGraphicsItem() override;
    
    void setZValue(qreal z) override;
    
public slots:
    void updateExtents(const QRectF& extents) override;
    
    void updateCurvatureRadii(const qreal& radiusX, const qreal& radiusY);
};

#endif