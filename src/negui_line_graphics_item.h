#ifndef __NEGUI_LINE_GRAPHICS_ITEM_H
#define __NEGUI_LINE_GRAPHICS_ITEM_H

#include "negui_shape_graphics_item_base.h"
#include "negui_line_style.h"

class MyLinearGraphicsItemBase: public MyShapeGraphicsItemBase, public QGraphicsPathItem {
public:
    
    MyLinearGraphicsItemBase(QGraphicsItem *parent);
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    const qreal getEndSlope() const override;
    
    QRectF getExtents() const override;
    
protected:
    QLineF _line;
    qreal _endSlope;
};

class MyLineGraphicsItem: public MyLinearGraphicsItemBase {
public:
    
    MyLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setLine(const QLineF& line) override;
};

class MyBezierGraphicsItem: public MyLinearGraphicsItemBase {
public:
    
    MyBezierGraphicsItem(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setLine(const QLineF& line) override;
    
    const qreal calcualteBezierEndSlope(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3) const;
};

#endif
