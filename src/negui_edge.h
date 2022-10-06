#ifndef __NEGUI_EDGE_H
#define __NEGUI_EDGE_H

#include "negui_base.h"
#include "negui_element_base.h"
#include "negui_node.h"
#include "negui_arrow_head.h"
#include "negui_edge_graphics_item.h"
#include "negui_edge_style.h"

class MyNode;
class MyArrowHead;


class MyEdge : public MyElementBase {
    
public:
    
    MyEdge(const QString& name, MyNode* startNode, MyNode* endNode);
    
    ~MyEdge();
    
    ELEMENT_TYPE type() override;
    
    const QString typeAsString() const override;
    
    MyNode* startNode();
    MyNode* endNode();
    
    void updateGraphicsItem() override;
    
    void setSelected(const bool& selected) override;
    
    MyArrowHead* arrowHead();
    void setArrowHead();
    const bool isSetArrowHead() const { return _isSetArrowHead; }
    
    // determine whether the edge is visible on the scene
    bool setActive(const bool& active);
    
    // return true if the edge is visible on the scene
    const bool isActive() const { return _isActive; }
    
    // determine whether the edge is connected to its start and end nodes
    bool connectToNodes(const bool& connect);
    
    // return true if the edge is connected to its start and end nodes
    const bool isConnectedToNodes() const { return _isConnectedToNodes; }
    
    // update the ending points of the edge using the start and end node positions
    void updatePoints();
    
    const QPointF getStartPosition();
    
    const QPointF getEndPosition();
    
    // enable the normal mode of the edge
    void enableNormalMode() override;
    
    // enable the add node mode of the edge
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    // enable the add edge mode of the edge
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    // enable the remove mode of the edge
    void enableRemoveMode() override;
    
    const qint32 calculateZValue() override;
    
    // read the edge info from the json object
    void read(const QJsonObject &json) override;
    
    // write the edge info to the json object
    void write(QJsonObject &json) override;
    
private slots:
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) override;
    
    QWidget* getFeatureMenu() override;

protected:
    MyNode* _startNode;
    MyNode* _endNode;
    MyArrowHead* _arrowHead;
    bool _isSetArrowHead;
    bool _isActive;
    bool _isConnectedToNodes;
};

#endif
