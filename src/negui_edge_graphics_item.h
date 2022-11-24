#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"

class MyEdgeGraphicsItemBase: public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
    
    void setLine(const QLineF &line);
    
    const qreal getEndSlope() const;
    
    void enableNormalMode() override;
    
    QList<QGraphicsItem*> createResizeHandleBaredGraphicsItems() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void enableRemoveMode() override;

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
