#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_H

#include "negui_network_element_graphics_item_base.h"

class MyEdgeGraphicsItemBase: public MyNetworkElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;

    QMenu* createContextMenu() override;
    
    void setLine(const QLineF &line);
    
    const qreal getEndSlope() const;

    void adjustStartPointToControlBezierLine(const QLineF& controlBezierLine);

    void adjustEndPointToControlBezierLine(const QLineF& controlBezierLine);
    
signals:
    
    void askForUpdateArrowHeadPlacement();

    void askForUpdateConnectedEdgesToSourceNode(const QPointF&);

    void askForUpdateConnectedEdgesToTargetNode(const QPointF&);

    void askForAdjustStartPointToControlBezierLine(const QLineF&);

    void askForAdjustEndPointToControlBezierLine(const QLineF&);

protected:
    QLineF _initialLine;
};

class MyEdgeSceneGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void setFocused(const bool& isFocused) override;

signals:

    void askForSetConnectedElementsFocused(const bool&);
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    QPointF _mousePressedPosition;
};

class MyEdgeIconGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
