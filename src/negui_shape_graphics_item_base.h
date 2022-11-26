#ifndef __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H
#define __NEGUI_SHAPE_GRAPHICS_ITEM_BASE_H

#include "negui_shape_style_base.h"
#include <QGraphicsItem>

class MyShapeGraphicsItemBase : public QObject {
    Q_OBJECT
    
public:
    
    MyShapeGraphicsItemBase();
    
    MyShapeStyleBase* style();
    
    void setStyle(MyShapeStyleBase* style);
    
    const bool isSetStyle() const { return _isSetStyle; }
    
    void setMovedDistance(const QPointF& movedDistance);
    
    const QPointF& movedDistance() const;
    
    virtual void updateStyle() = 0;
    
    virtual void setSelectedWithStroke(const bool& selected) = 0;
    
    virtual void setSelectedWithFill(const bool& selected) = 0;
    
    virtual QRectF getExtents() = 0;
    
    virtual QGraphicsItem* getResizeHandlebaredGraphicsItem() = 0;
    
    virtual void setZValue(qreal z) = 0;
    
public slots:
    virtual void updateExtents(const QRectF& extents) = 0;

protected:
    MyShapeStyleBase* _style;
    QPointF _originalPosition;
    QPointF _movedDistance;
    bool _isSetStyle;
};

#endif