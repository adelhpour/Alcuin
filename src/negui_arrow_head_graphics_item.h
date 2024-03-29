#ifndef __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H
#define __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H

#include "negui_network_element_graphics_item_base.h"

class MyArrowHeadGraphicsItemBase : public MyNetworkElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadGraphicsItemBase(QGraphicsItem *parent = nullptr);

    const bool canAddEllipseShape() override;

    const bool canAddRectangleShape() override;

    const bool canAddPolygonShape() override;
};

class MyArrowHeadSceneGraphicsItem : public MyArrowHeadGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadSceneGraphicsItem(QGraphicsItem *parent = nullptr);
    
    void update(const QPointF& position, const qreal& rotation);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;

    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;
};

class MyArrowHeadIconGraphicsItem : public MyArrowHeadGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation, QGraphicsItem *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    
protected:
    qreal _rotation;
};

#endif
