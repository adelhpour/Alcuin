#ifndef __NEGUI_1D_SHAPE_GRAPHICS_ITEM_H
#define __NEGUI_1D_SHAPE_GRAPHICS_ITEM_H

#include "negui_shape_graphics_item_base.h"

class My1DShapeGraphicsItemBase: public MyShapeGraphicsItemBase {
public:
    
    My1DShapeGraphicsItemBase();
    
    const qreal getEndSlope() const;
    
    virtual void setLine(const QLineF& line) = 0;
    
    QGraphicsItem* getResizeHandlebaredGraphicsItem() override;
    
public slots:
    virtual void updateExtents(const QRectF& extents) override;
    
protected:
    QLineF _line;
    qreal _endSlope;
};

#endif