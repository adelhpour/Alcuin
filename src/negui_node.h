#ifndef __NEGUI_NODE_H
#define __NEGUI_NODE_H

#include "negui_network_element_base.h"

class MyNodeBase : public MyNetworkElementBase {
    Q_OBJECT
    
public:

    typedef enum {
        CLASSIC_NODE,
        CENTROID_NODE,
    } NODE_TYPE;
    
    MyNodeBase(const QString& name, const qreal& x, const qreal& y);
    
    ~MyNodeBase();
    
    ELEMENT_TYPE type() override;

    virtual NODE_TYPE nodeType() = 0;

    void connectGraphicsItem() override;
    
    // add to edges
    virtual void addEdge(MyNetworkElementBase* e);
    
    // remove from edges
    virtual void removeEdge(MyNetworkElementBase* e);
    
    // get edges
    QList<MyNetworkElementBase*>& edges();

    virtual MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) = 0;
    
    void updateGraphicsItem() override;
    
    void setSelected(const bool& selected) override;

    // get the position of the node
    const QPointF position() const;

    // get the id of parent node of the node
    const QString& parentNodeId() const;

    // set the id of the parent node of the node
    void setParentNodeId(const QString& parentNodeId);

    // get the parent node of the node
    MyNetworkElementBase* parentNode();

    // set the parent node of the node
    void setParentNode(MyNetworkElementBase* parentNode);

    // return true if the parent node of the node is set
    const bool isSetParentNode() const { return _isSetParentNode; }

    // lock the position of the parent node
    void lockParentNode(const bool& locked);

    // return true if the child nodes of the node are locked
    const bool isParentNodeLocked() const { return _isParentNodeLocked; }

    const QRectF getExtents() override;

    const qreal endEdgePadding();
    
    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;
    
    // read the node info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node info to the json object
    void write(QJsonObject &json) override;
    
signals:
    
    MyNetworkElementBase* askForParentNodeAtPosition(MyNetworkElementBase* currentNode, const QPointF& position);

    void askForCopyNode(MyNetworkElementBase*);
    
public slots:
    
    // unset the parent of the node
    void deparent();
    
    // set a new parent to the node
    void reparent();
    
    // set the position of the node using its current position
    void resetPosition();
    
    // set the position of the node
    virtual void setPosition(const QPointF& position);

    void adjustConnectedEdges();

protected:
    QString _parentNodeId;
    MyNetworkElementBase* _parentNode;
    QList<MyNetworkElementBase*> _edges;
    QPointF _position;
    bool _isSetParentNode;
    bool _isParentNodeLocked;
    qreal _endEdgePadding;
};

class MyClassicNode : public MyNodeBase {
    Q_OBJECT

public:

    MyClassicNode(const QString& name, const qreal& x, const qreal& y);

    NODE_TYPE nodeType() override;

    MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) override;

    // add to child nodes
    void addChildNode(MyNetworkElementBase* n);

    // remove the node from child nodes
    void removeChildNode(MyNetworkElementBase* n);

    // get the child nodes
    QList<MyNetworkElementBase*>& childNodes();

    // lock the position of the child nodes
    void lockChildNodes(const bool& locked);

    // set whether the child nodes are movable or not
    void updateChildNodesMobility();

    // return true if the child nodes of the node are locked
    const bool areChildNodesLocked() const { return _areChildNodesLocked; }

    // get node extents based on its children extents
    const QRectF getExtents() override;

    // adjust node extents to fit all its children
    void adjustExtents();

    QWidget* getFeatureMenu() override;

public slots:

    // set the position of the node
    void setPosition(const QPointF& position) override;

protected:
    QList<MyNetworkElementBase*> _childNodes;
    bool _areChildNodesLocked;
};

class MyCentroidNode : public MyNodeBase {
    Q_OBJECT

public:

    MyCentroidNode(const QString& name, const qreal& x, const qreal& y);

    NODE_TYPE nodeType() override;

    void connectGraphicsItem() override;

    MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) override;

    // add to edges
    void addEdge(MyNetworkElementBase* e) override;

    // remove from edges
    void removeEdge(MyNetworkElementBase* e) override;

    const QLineF createControlBezierLine(const QPointF& updatedPoint);

    const QPointF getNodeUpdatedPositionUsingConnectedEdges();

    const QLineF adjustControlBezierLine(const QLineF& controlBezierLine);

    const qreal getAdjustControlBezierLineStartPointX(const QLineF& controlBezierLine);

    const qreal getAdjustControlBezierLineEndPointX(const QLineF& controlBezierLine);

    const qreal getControlBezierLineAdjustmentLengthX(const qreal& adjustedStartPointX, const qreal& adjustedEndPointX);

    const qreal getAdjustControlBezierLineStartPointY(const QLineF& controlBezierLine);

    const qreal getAdjustControlBezierLineEndPointY(const QLineF& controlBezierLine);

    const qreal getControlBezierLineAdjustmentLengthY(const qreal& adjustedStartPointY, const qreal& adjustedEndPointY);

    signals:

    void controlBezierLineIsUpdated(const QLineF&);

private slots:

    void adjustNodePositionToNeighborNodes();

    void adjustConnectedEdges(const QPointF&);

    void disconnectNodePositionFromNeighborNodes();

protected:
    QList<MyNetworkElementBase*> _childNodes;
    bool _areChildNodesLocked;
    bool _doesNodePositionDependOnNeighboringNodes;
};

#endif
