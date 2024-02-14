#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_H

#include "negui_network_element_graphics_item_base.h"

class MyEdgeGraphicsItemBase: public MyNetworkElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeGraphicsItemBase(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;

    virtual MyShapeGraphicsItemBase* createLineShapeGraphicsItem() = 0;

    void connectShapeGraphicsItem(MyShapeGraphicsItemBase* item) override;

    const bool canAddLineShape() override;

    QMenu* createContextMenu() override;
    
    void setLine(const QLineF &line);
    
    const qreal getEndSlope() const;

    void adjustStartPointToControlBezierLine(const QLineF& controlBezierLine);

    void adjustEndPointToControlBezierLine(const QLineF& controlBezierLine);

public slots:

    void displayContextMenu(const QPoint& position) override;
    
signals:
    
    void askForUpdateArrowHeadPlacement();

    void askForAdjustStartPointToControlBezierLine(const QLineF&);

    void askForAdjustEndPointToControlBezierLine(const QLineF&);

protected:
    QLineF _initialLine;
};

class MyEdgeSceneGraphicsItemBase: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeSceneGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;
};

class MyClassicEdgeSceneGraphicsItem: public MyEdgeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyClassicEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createLineShapeGraphicsItem() override;
};

class MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase: public MyEdgeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase(QGraphicsItem *parent = nullptr);

    void connectShapeGraphicsItem(MyShapeGraphicsItemBase* item) override;

signals:

    const QPointF askForConnectedToCentroidNodeControlPoint();
};

class MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem: public MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createLineShapeGraphicsItem() override;
};

class MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem: public MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createLineShapeGraphicsItem() override;
};

class MyEdgeIconGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);

    MyShapeGraphicsItemBase* createLineShapeGraphicsItem() override;
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
