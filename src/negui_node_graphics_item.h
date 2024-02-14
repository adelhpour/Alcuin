#ifndef __NEGUI_NODE_GRAPHICS_ITEM_H
#define __NEGUI_NODE_GRAPHICS_ITEM_H

#include "negui_network_element_graphics_item_base.h"
#include <QKeyEvent>

class MyNodeGraphicsItemBase : public MyNetworkElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeGraphicsItemBase(QGraphicsItem *parent = nullptr);

    QMenu* createContextMenu() override;

    virtual QMenu* createContextMenuObject();

public slots:

    void displayContextMenu(const QPoint& position) override;

signals:

    const QLineF askForGetBezierAdjustLine();

    void bezierAdjustLineIsUpdated(const QLineF&);

};

class MyNodeSceneGraphicsItemBase : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:

    MyNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent = nullptr);

    void update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) override;

    void moveChildItems(const qreal& dx, const qreal& dy);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    virtual void move(qreal dx, qreal dy);
    
signals:
    
    void askForUpdateParentNode();
    
    void askForUpdateConnectedEdgesPoints();

    void positionChangedByMouseMoveEvent(const QPointF&);

public slots:

    void updateOriginalPosition();
    
protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyClassicNodeSceneGraphicsItemBase : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyClassicNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent = nullptr);

    const bool canAddEllipseShape() override;

    const bool canAddRectangleShape() override;

    const bool canAddPolygonShape() override;

    const bool canAddTextShape() override;

    void setFocused(const bool& isFocused) override;

public slots:

    void updateExtents(const QRectF& extents);
};

class MySimpleClassicNodeSceneGraphicsItem : public MyClassicNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MySimpleClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    const QRectF getExtents() const override;

    QList<QGraphicsItem*> createFocusedGraphicsItems() override;

    const bool whetherShapesAreOneSimpleTextShapeAndAnotherShape();
};

class MyComplexClassicNodeSceneGraphicsItem : public MyClassicNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyComplexClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);

};

class MyCentroidNodeSceneGraphicsItem : public MyNodeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyCentroidNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    void connectShapeGraphicsItem(MyShapeGraphicsItemBase *item) override;

    const bool canAddCentroidShape() override;

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

    const bool canAddEllipseShape() override;

    const bool canAddRectangleShape() override;

    const bool canAddPolygonShape() override;

    const bool canAddTextShape() override;
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
