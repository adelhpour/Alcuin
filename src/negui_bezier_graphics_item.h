#ifndef __NEGUI_BEZIER_GRAPHICS_ITEM_H
#define __NEGUI_BEZIER_GRAPHICS_ITEM_H

#include "negui_1d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyBezierGraphicsItem: public My1DShapeGraphicsItemBase, public QGraphicsPathItem {
public:
    
    MyBezierGraphicsItem(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    QRectF getExtents() const override;
    
    void setZValue(qreal z) override;
    
    void setLine(const QLineF& line) override;
    
    const qreal calcualteBezierEndSlope(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3) const;
};

#endif
