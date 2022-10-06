#ifndef __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H
#define __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H

#include "negui_base.h"
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
    
    virtual void updateExtents(const QRectF& extents);
    
    virtual QRectF getExtents() const = 0;
    
    virtual void setLine(const QLineF& line);
    
    virtual const qreal getEndSlope() const;

protected:
    MyShapeStyleBase* _style;
    QPointF _originalPosition;
    bool _isSetStyle;
};

#endif
