#ifndef __NEGUI_2D_SHAPE_GRAPHICS_ITEM_H
#define __NEGUI_2D_SHAPE_GRAPHICS_ITEM_H

#include "negui_shape_graphics_item_base.h"

class My2DShapeGraphicsItemBase: public MyShapeGraphicsItemBase {
public:
    
    My2DShapeGraphicsItemBase(qreal x, qreal y);
    
    virtual void updateExtents(const QRectF& extents) = 0;
};

#endif
