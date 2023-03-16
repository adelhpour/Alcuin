#ifndef __NEGUI_NODE_GRAPHICS_ITEM_H
#define __NEGUI_NODE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"
#include <QKeyEvent>

class MyNodeGraphicsItemBase : public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeGraphicsItemBase(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
};

class MyNodeSceneGraphicsItemBase : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:

    MyNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent = nullptr);
    
    void deparent();

    void moveChildItems(const QPointF& movedDistance);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void enableRemoveMode() override;
    
signals:
    
    void askForDeparent();
    
    void askForReparent();
    
    void askForResetPosition();
    
protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
    void keyPressEvent(QKeyEvent *event) override;
    
    void keyReleaseEvent(QKeyEvent *event) override;
    
    bool _reparent;
    QPointF _mousePressedPosition;
};

class MyClassicNodeSceneGraphicsItem : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    void clearResizeHandledGraphicsItems() override;

    void moveBy(qreal dx, qreal dy);

    void adjustOriginalPosition();

public slots:

    void updateExtents(const QRectF& extents);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;

    QPointF _mousePressedPosition;
};

class MyCentroidNodeSceneGraphicsItem : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyCentroidNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);
};

class MyNodeIconGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeIconGraphicsItem(const QPointF& position, QGraphicsItem *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
