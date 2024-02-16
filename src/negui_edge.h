#ifndef __NEGUI_EDGE_H
#define __NEGUI_EDGE_H

#include "negui_network_element_base.h"

class MyEdgeBase : public MyNetworkElementBase {
    Q_OBJECT
    
public:

    typedef enum {
        CLASSIC_EDGE,
        CONNECTED_TO_SOURCE_CENTROID_NODE_EDGE,
        CONNECTED_TO_TARGET_CENTROID_NODE_EDGE
    } EDGE_TYPE;
    
    MyEdgeBase(const QString& name);

    ELEMENT_TYPE type() override;

    virtual EDGE_TYPE edgeType() = 0;

    void connectGraphicsItem() override;

    void setSourceNode(MyNetworkElementBase* sourceNode);

    MyNetworkElementBase* sourceNode();

    void setTargetNode(MyNetworkElementBase* targetNode);

    MyNetworkElementBase* targetNode();

    MyNetworkElementGraphicsItemBase* arrowHeadGraphicsItem();
    
    void updateGraphicsItem() override;

    void updateArrowHeadGraphicsItem();
    
    void setStyle(MyNetworkElementStyleBase* style) override;

    void setSelectedWithColor(const bool& selected) override;

    void setArrowHeadGraphicsItem();
    
    // determine whether the edge is visible on the scene
    bool setActive(const bool& active) override;

    // update the ending points of the edge using the source and target node positions
    void updatePoints();

    const bool canBeMoved() override;

    void move(const qreal& dx, const qreal& dy) override;
    
    const QRectF getExtents() override;
    
    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;

    const qint32 calculateArrowHeadZValue();
    
    // read the edge info from the json object
    void read(const QJsonObject &json) override;
    
    // write the edge info to the json object
    void write(QJsonObject &json) override;
    
public slots:

    void setSelected(const bool& selected) override;
    
    void updateArrowHeadPlacement();

signals:

    void askForAdjustNodePositionToNeighborNodes(const bool&);

    void askForSetNodeParentUsingNeighborNodesParent();

    void askForSetConnectedElementsSelected(const bool& isSelected);

protected:
    MyNetworkElementBase* _sourceNode;
    MyNetworkElementBase* _targetNode;
    MyNetworkElementGraphicsItemBase* _arrowHeadGraphicsItem;
};

class MyClassicEdge : public MyEdgeBase {
    Q_OBJECT

public:

    MyClassicEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

    EDGE_TYPE edgeType() override;

    const bool isCopyable() override;

    const bool isCuttable() override;
};

class MyConnectedToCentroidNodeEdgeBase : public MyEdgeBase {
    Q_OBJECT

public:

    MyConnectedToCentroidNodeEdgeBase(const QString& name);

    void connectGraphicsItem() override;

    virtual const QPointF nonCentroidNodePosition() = 0;

    virtual MyNetworkElementBase* nonCentroidNodeParent() = 0;

signals:

    const QPointF askForConnectedToCentroidNodeControlPoint();
};

class MyConnectedToSourceCentroidNodeEdge : public MyConnectedToCentroidNodeEdgeBase {
    Q_OBJECT

public:

    MyConnectedToSourceCentroidNodeEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

    EDGE_TYPE edgeType() override;

    const bool isCopyable() override;

    const bool isCuttable() override;

    const QPointF nonCentroidNodePosition() override;

    MyNetworkElementBase* nonCentroidNodeParent() override;
};

class MyConnectedToTargetCentroidNodeEdge : public MyConnectedToCentroidNodeEdgeBase {
    Q_OBJECT

public:

    MyConnectedToTargetCentroidNodeEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

    EDGE_TYPE edgeType() override;

    const bool isCopyable() override;

    const bool isCuttable() override;

    const QPointF nonCentroidNodePosition() override;

    MyNetworkElementBase* nonCentroidNodeParent() override;
};

const QPointF getEndOfTheLinePosition(MyNetworkElementBase* mainNode, MyNetworkElementBase* connectedNode);

qreal getRadius(const QRectF& rect, const QPointF& point);

#endif
