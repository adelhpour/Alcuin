#ifndef __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H
#define __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H

#include "negui_shape_style_base.h"

class MyShapeGraphicsItemBase {
public:
    
    MyShapeGraphicsItemBase();
    
    MyShapeStyleBase* style();
    
    void setStyle(MyShapeStyleBase* style);
    
    const bool isSetStyle() const { return _isSetStyle; }
    
    virtual void updateStyle() = 0;
    
    virtual void setSelectedWithStroke(const bool& selected) = 0;
    
    virtual void setSelectedWithFill(const bool& selected) = 0;
    
    virtual QRectF getExtents() const = 0;
    
    virtual void setZValue(qreal z) = 0;

protected:
    MyShapeStyleBase* _style;
    QPointF _originalPosition;
    bool _isSetStyle;
};

#endif
