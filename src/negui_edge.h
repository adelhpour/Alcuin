#ifndef __NEGUI_EDGE_H
#define __NEGUI_EDGE_H

#include "negui_network_element_base.h"

class MyEdgeBase : public MyNetworkElementBase {
    Q_OBJECT
    
public:

    typedef enum {
        CLASSIC_EDGE,
    } EDGE_TYPE;
    
    MyEdgeBase(const QString& name);
    
    ~MyEdgeBase();

    ELEMENT_TYPE type() override;

    virtual EDGE_TYPE edgeType() = 0;

    void connectGraphicsItem() override;

    void setSourceNode(MyNetworkElementBase* sourceNode);

    MyNetworkElementBase* sourceNode();

    void setTargetNode(MyNetworkElementBase* targetNode);

    MyNetworkElementBase* targetNode();
    
    void updateGraphicsItem() override;
    
    void setStyle(MyNetworkElementStyleBase* style) override;

    const bool isCopyable() override;

    const bool isCuttable() override;

    void setSelectedWithColor(const bool& selected) override;
    
    MyNetworkElementBase* arrowHead();
    void setArrowHead();
    const bool isSetArrowHead() const { return _isSetArrowHead; }
    
    // determine whether the edge is visible on the scene
    bool setActive(const bool& active) override;

    // update the ending points of the edge using the source and target node positions
    void updatePoints();
    
    // enable the normal mode of the edge
    void enableNormalMode() override;
    
    // enable the add node mode of the edge
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    // enable the add edge mode of the edge
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;

    const QPointF middlePosition();
    
    const QRectF getExtents() override;
    
    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;
    
    // read the edge info from the json object
    void read(const QJsonObject &json) override;
    
    // write the edge info to the json object
    void write(QJsonObject &json) override;
    
public slots:

    void setSelected(const bool& selected) override;

    void adjustConnectedEdgesToSourceNode(const QPointF& updatedSourcePoint);

    void adjustConnectedEdgesToTargetNode(const QPointF& updatedTargetPoint);
    
    void updateArrowHeadPlacement();

signals:
    void askForAdjustConnectedEdges(const QPointF&);

    void askForAdjustNodePositionToNeighborNodes();

    void askForDisconnectNodePositionFromNeighborNodes();

    void askForSetConnectedElementsFocused(const bool& isFocused);

protected:
    MyNetworkElementBase* _sourceNode;
    MyNetworkElementBase* _targetNode;
    MyNetworkElementBase* _arrowHead;
    bool _isSetArrowHead;
};

class MyClassicEdge : public MyEdgeBase {
    Q_OBJECT

public:

    MyClassicEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

    EDGE_TYPE edgeType() override;
};

const QPointF getEndOfTheLinePosition(MyNetworkElementBase* mainNode, MyNetworkElementBase* connectedNode);

qreal getRadius(const QRectF& rect, const QPointF& point);

#endif
