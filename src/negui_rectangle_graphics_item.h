#ifndef __NEGUI_RECTANGLE_GRAPHICS_ITEM_H
#define __NEGUI_RECTANGLE_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"

class MyRectangleGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsPathItem {
    Q_OBJECT
    
public:

    MyRectangleGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithBorderColor(const bool& selected) override;
    
    void setSelectedWithFillColor(const bool& selected) override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getFocusedGraphicsItem() override;
    
    void setZValue(qreal z) override;

    void updateOriginalPosition(const QPointF originalPosition) override;

    void moveOriginalPosition(const qreal& dx, const qreal& dy) override;
    
public slots:
    void updateExtents(const QRectF& extents) override;
    
    void updateBorderRadii(const qreal& borderRadiusX, const qreal& borderRadiusY);
};

#endif
