#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"
#include "negui_line_graphics_item.h"

class MyEdgeGraphicsItemBase: public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* defaultEllipseItem() override;
    
    MyShapeGraphicsItemBase* defaultRectItem() override;
    
    MyShapeGraphicsItemBase* defaultPolygonItem() override;
    
    MyShapeGraphicsItemBase* defaultLineItem() override;
    
    MyShapeGraphicsItemBase* defaultBezierItem() override;
    
    MyShapeGraphicsItemBase* defaultTextItem() override;
    
    void setLine(const QLineF &line);
    
    const qreal getEndSlope() const;

protected:
    QLineF _initialLine;
};

class MyEdgeSceneGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);
};

class MyEdgeIconGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeIconGraphicsItem(QGraphicsItem *parent = nullptr);
};

#endif
