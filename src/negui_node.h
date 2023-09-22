#ifndef __NEGUI_NODE_H
#define __NEGUI_NODE_H

#include "negui_network_element_base.h"

class MyNodeBase : public MyNetworkElementBase {
    Q_OBJECT
    
public:

    typedef enum {
        SIMPLE_CLASSIC_NODE,
        COMPLEX_CLASSIC_NODE,
        CENTROID_NODE,
    } NODE_TYPE;
    
    MyNodeBase(const QString& name, const qreal& x, const qreal& y);
    
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
    
    void setSelectedWithColor(const bool& selected) override;

    const bool isCuttable() override;

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

    // unset the parent node of the node
    void unsetParentNode();

    // return true if the parent node of the node is set
    const bool isSetParentNode() const { return _isSetParentNode; }

    // lock the position of the parent node
    void lockParentNode(const bool& locked);

    // return true if the child nodes of the node are locked
    const bool isParentNodeLocked() const { return _isParentNodeLocked; }

    const QRectF getExtents() override;

    const qreal endEdgePadding();
    
    const qint32 calculateZValue() override;

    const qint32 calculateParentZValue();

    virtual const qint32 calculateNodeZValue() = 0;

    virtual const qint32 calculateConnectedEdgeZValue() = 0;

    void addParentFeaturesToFeatureMenu(QWidget* featureMenu);
    
    // read the node info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node info to the json object
    void write(QJsonObject &json) override;
    
signals:
    
    MyNetworkElementBase* askForParentNodeAtPosition(MyNetworkElementBase* currentNode, const QPointF& position);

    void positionChangedByMouseMoveEvent(MyNetworkElementBase*, const QPointF&);
    
public slots:
    
    // unset the parent of the node
    void deparent();
    
    // set a new parent to the node
    void reparent();
    
    // set the position of the node using its current position
    void resetPosition();
    
    // set the position of the node
    virtual void setPosition(const QPointF& position);

    void adjustConnectedEdges(const bool& movedByParentNodeMove = false);

    void setConnectedNodesParents();

    virtual void adjustExtents() = 0;

protected:
    QString _parentNodeId;
    MyNetworkElementBase* _parentNode;
    QList<MyNetworkElementBase*> _edges;
    QPointF _position;
    bool _isSetParentNode;
    bool _isParentNodeLocked;
    qreal _endEdgePadding;
};

class MyClassicNodeBase : public MyNodeBase {
    Q_OBJECT

public:

    MyClassicNodeBase(const QString& name, const qreal& x, const qreal& y);

    const bool isCopyable() override;

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

    void moveExternally(const qreal& dx, const qreal& dy) override;

    const bool canBeMovedExternally() override;

    // get node extents based on its children extents
    const QRectF getExtents() override;

    const QRectF createParentNodeExtentsRectangle(qreal extentsX, qreal extentsY, qreal extentsWidth, qreal extentsHeight);

    const qint32 calculateNodeZValue() override;

    const qint32 calculateConnectedEdgeZValue() override;

public slots:

    // set the position of the node
    void setPosition(const QPointF& position) override;

    void adjustExtents() override;

protected:
    QList<MyNetworkElementBase*> _childNodes;
    bool _areChildNodesLocked;
};

class MySimpleClassicNode : public MyClassicNodeBase {
    Q_OBJECT

public:

    MySimpleClassicNode(const QString& name, const qreal& x, const qreal& y);

    NODE_TYPE nodeType() override;

    MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) override;

    void connectGraphicsItem() override;

    const QString& displayName();

    void setDisplayName(const QString& displayName);

    QWidget* getFeatureMenu() override;

    void addDisplayNameToFeatureMenu(QWidget* featureMenu);

protected:

    QString _displayName;
};

class MyComplexClassicNode : public MyClassicNodeBase {
    Q_OBJECT

public:

    MyComplexClassicNode(const QString& name, const qreal& x, const qreal& y);

    NODE_TYPE nodeType() override;

    MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) override;

    QWidget* getFeatureMenu() override;
};

class MyCentroidNode : public MyNodeBase {
    Q_OBJECT

public:

    MyCentroidNode(const QString& name, const qreal& x, const qreal& y);

    NODE_TYPE nodeType() override;

    void connectGraphicsItem() override;

    MyNetworkElementGraphicsItemBase* createGraphicsItem(const QPointF &position) override;

    const bool isCopyable() override;

    // add to edges
    void addEdge(MyNetworkElementBase* e) override;

    // remove from edges
    void removeEdge(MyNetworkElementBase* e) override;

    const QPointF getNodeUpdatedPositionUsingConnectedEdges();

    const bool connectedBezierCurvesNeedsToBeAdjusted();

    void moveExternally(const qreal& dx, const qreal& dy) override;

    const bool canBeMovedExternally() override;

    QWidget* getFeatureMenu() override;

signals:

    void bezierAdjustLineIsUpdated(const QLineF&);

public slots:

    void adjustNodePositionToNeighborNodes(const bool& movedByParentNodeMove = false);

    void adjustExtents() override;

private slots:

    void adjustConnectedBezierCurves();

    const bool isNodePositionConnectedToNeighborNodes();

    void connectNodePositionToNeighborNodes(const bool& connected);

    void setNodeParentUsingNeighborNodesParent();

    void setSelected(const bool& selected) override;

    const QLineF createBezierAdjustLine();

    const qint32 calculateNodeZValue() override;

    const qint32 calculateConnectedEdgeZValue() override;

protected:
    QList<MyNetworkElementBase*> _childNodes;
    bool _areChildNodesLocked;
    bool _isNodePositionConnectedToNeighborNodes;
};

#endif
