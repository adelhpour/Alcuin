#ifndef __NEGUI_CENTROID_GRAPHICS_ITEM_H
#define __NEGUI_CENTROID_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"

class MyCentroidGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsEllipseItem {
public:
    
    MyCentroidGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getResizeHandledGraphicsItem() override;
    
    void setZValue(qreal z) override;
    
    void adjustOriginalPosition(const QPointF& originalPositionMovedDistance) override;
    
public slots:
    void updateExtents(const QRectF& extents) override;
};

#endif
