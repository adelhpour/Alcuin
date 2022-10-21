#ifndef __NEGUI_LINE_GRAPHICS_ITEM_H
#define __NEGUI_LINE_GRAPHICS_ITEM_H

#include "negui_1d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyLineGraphicsItem: public My1DShapeGraphicsItemBase, public QGraphicsPathItem {
public:
    
    MyLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    QRectF getExtents() const override;
    
    void setZValue(qreal z) override;
    
    void setLine(const QLineF& line) override;
};

#endif
