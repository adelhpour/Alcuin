#ifndef __NEGUI_NODE_H
#define __NEGUI_NODE_H

#include "negui_base.h"
#include "negui_element_base.h"
#include "negui_edge.h"
#include "negui_node_graphics_item.h"

class MyEdge;

class MyNode : public MyElementBase {
    Q_OBJECT
    
public:
    
    MyNode(const QString& name, const qreal& x, const qreal& y);
    
    ~MyNode();
    
    ELEMENT_TYPE type() override;
    
    const QString typeAsString() const override;
    
    // add to edges
    void addEdge(MyEdge* e);
    
    // remove from edges
    void removeEdge(MyEdge* e);
    
    // get edges
    QList<MyEdge*>& edges();
    
    void setSelected(const bool& selected) override;
    
    // get the positoin of the node
    const QPointF position() const;
    
    // get the id of parent node of the node
    const QString& parentNodeId() const;
    
    // set the id of the parent node of the node
    void setParentNodeId(const QString& parentNodeId);
    
    // get the parent node of the node
    MyNode* parentNode();
    
    // set the parent node of the node
    void setParentNode(MyNode* parentNode);
    
    // return true if the parent node of the node is set
    const bool isSetParentNode() const { return _isSetParentNode; }
    
    // lock the position of the parent node
    void lockParentNode(const bool& locked);
    
    // return true if the child nodes of the node are locked
    const bool isParentNodeLocked() const { return _isParentNodeLocked; }
    
    // add to child nodes
    void addChildNode(MyNode* n);
    
    // remove the node from child nodes
    void removeChildNode(MyNode* n);
    
    // get the child nodes
    QList<MyNode*>& childNodes();
    
    // lock the position of the child nodes
    void lockChildNodes(const bool& locked);
    
    // set whether the child nodes are movable or not
    void updateChildNodesMobility();
    
    // return true if the child nodes of the node are locked
    const bool areChildNodesLocked() const { return _areChildNodesLocked; }
    
    // determine whether the node is visible on the scene
    bool setActive(const bool& active);
    
    // return true if the node is visible on the scene
    const bool isActive() const { return _isActive; }
    
    // enable the normal mode of the node
    void enableNormalMode() override;
    
    // enable the add node mode of the node
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    // enable the add edge mode of the node
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    // enable the remove mode of the node
    void enableRemoveMode() override;
    
    // get node extents based on its children extents
    const QRectF getExtents();
    
    // adjust node extents to fit all its children
    void adjustExtentsTochildren();
    
    const qint32 calculateZValue() override;
    
    // read the node info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node info to the json object
    void write(QJsonObject &json) override;
    
signals:
    
    MyElementBase* askForParentNodeAtPosition(MyElementBase* currentNode, const QPointF& position);
    
private slots:
    
    // unset the parent of the node
    void deparent();
    
    // set a new parent to the node
    void reparent();
    
    // set the position of the node
    void setPosition(const QPointF& position);
    
    // set the position of the node using its current position
    void resetPosition();
    
    QWidget* getFeatureMenu() override;
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) override;

protected:
    QString _parentNodeId;
    MyNode* _parentNode;
    QList<MyEdge*> _edges;
    QList<MyNode*> _childNodes;
    QPointF _position;
    bool _isSetParentNode;
    bool _isActive;
    bool _isParentNodeLocked;
    bool _areChildNodesLocked;
};

#endif
