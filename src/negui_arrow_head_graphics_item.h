#ifndef __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H
#define __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"
#include "negui_ellipse_graphics_item.h"
#include "negui_rect_graphics_item.h"
#include "negui_polygon_graphics_item.h"

class MyArrowHeadGraphicsItemBase : public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* defaultEllipseItem() override;
    
    MyShapeGraphicsItemBase* defaultRectItem() override;
    
    MyShapeGraphicsItemBase* defaultPolygonItem() override;
    
    MyShapeGraphicsItemBase* defaultLineItem() override;
    
    MyShapeGraphicsItemBase* defaultBezierItem() override;
    
    MyShapeGraphicsItemBase* defaultTextItem() override;
    
protected:
    QPointF _initialPosition;
    qreal _padding;
};

class MyArrowHeadSceneGraphicsItem : public MyArrowHeadGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadSceneGraphicsItem(QGraphicsItem *parent = nullptr);
    
    void update(const QPointF& position, const qreal& rotation);
};

class MyArrowHeadIconGraphicsItem : public MyArrowHeadGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadIconGraphicsItem(QGraphicsItem *parent = nullptr);
};

#endif
