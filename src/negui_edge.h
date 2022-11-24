#ifndef __NEGUI_EDGE_H
#define __NEGUI_EDGE_H

#include "negui_element_base.h"

class MyEdge : public MyElementBase {
    
public:
    
    MyEdge(const QString& name, MyElementBase* startNode, MyElementBase* endNode);
    
    ~MyEdge();
    
    ELEMENT_TYPE type() override;
    
    MyElementBase* startNode();
    MyElementBase* endNode();
    
    void updateGraphicsItem() override;
    
    void setStyle(MyElementStyleBase* style) override;
    
    void setSelected(const bool& selected) override;
    
    MyElementBase* arrowHead();
    void setArrowHead();
    const bool isSetArrowHead() const { return _isSetArrowHead; }
    
    // determine whether the edge is visible on the scene
    bool setActive(const bool& active) override;
    
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
    
    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;
    
    // read the edge info from the json object
    void read(const QJsonObject &json) override;
    
    // write the edge info to the json object
    void write(QJsonObject &json) override;

protected:
    MyElementBase* _startNode;
    MyElementBase* _endNode;
    MyElementBase* _arrowHead;
    bool _isSetArrowHead;
    bool _isConnectedToNodes;
};

#endif
