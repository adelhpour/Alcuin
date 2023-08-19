#ifndef __NEGUI_NODE_GRAPHICS_ITEM_H
#define __NEGUI_NODE_GRAPHICS_ITEM_H

#include "negui_network_element_graphics_item_base.h"
#include <QKeyEvent>

class MyNodeGraphicsItemBase : public MyNetworkElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeGraphicsItemBase(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;

    QMenu* createContextMenu() override;

    virtual QMenu* createContextMenuObject();

signals:

    const QLineF askForGetBezierAdjustLine();

    void bezierAdjustLineIsUpdated(const QLineF&);

    QString askForElementDisplayName();

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

    void enableDisplayFeatureMenuMode() override;
    
signals:
    
    void askForDeparent();
    
    void askForReparent();
    
    void askForResetPosition();

    void positionChangedByMouseMoveEvent();
    
protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    
    void keyPressEvent(QKeyEvent *event) override;
    
    void keyReleaseEvent(QKeyEvent *event) override;
    
    bool _reparent;
};

class MyClassicNodeSceneGraphicsItemBase : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyClassicNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent = nullptr);

    void clearFocusedGraphicsItems() override;

    void moveBy(qreal dx, qreal dy);

    void adjustOriginalPosition();

public slots:

    void updateExtents(const QRectF& extents);
};

class MySimpleClassicNodeSceneGraphicsItem : public MyClassicNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MySimpleClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);;

};

class MyComplexClassicNodeSceneGraphicsItem : public MyClassicNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyComplexClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);;

};

class MyCentroidNodeSceneGraphicsItem : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyCentroidNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    QMenu* createContextMenuObject() override;

    void connectContextMenu(QMenu* contextMenu) override;

signals:

    void askForConnectNodePositionToNeighborNodes(const bool&);

    const bool askForWhetherNodePositionIsConnectedToNeighborNodes();
};

class MyNodeIconGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeIconGraphicsItem(const QPointF& position, QGraphicsItem *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
