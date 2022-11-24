#ifndef __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H
#define __NEGUI_ARROW_HEAD_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"

class MyArrowHeadGraphicsItemBase : public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyArrowHeadGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
    
    QList<QGraphicsItem*> createResizeHandleBaredGraphicsItems() override;
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;

    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void enableRemoveMode() override;
    
protected:
    QPointF _originalPosition;
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
