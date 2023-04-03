#ifndef __NEGUI_EDGE_H
#define __NEGUI_EDGE_H

#include "negui_element_base.h"

class MyEdgeBase : public MyElementBase {
    Q_OBJECT
    
public:

    typedef enum {
        CLASSIC_EDGE,
    } EDGE_TYPE;
    
    MyEdgeBase(const QString& name, MyElementBase* startNode, MyElementBase* endNode);
    
    ~MyEdgeBase();
    
    ELEMENT_TYPE type() override;

    virtual EDGE_TYPE edgeType() = 0;
    
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
    
    const QRectF getExtents() override;
    
    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;
    
    // read the edge info from the json object
    void read(const QJsonObject &json) override;
    
    // write the edge info to the json object
    void write(QJsonObject &json) override;
    
public slots:
    
    void updateArrowHeadPlacement();

signals:
    void askForAdjustConnectedEdges();

protected:
    MyElementBase* _startNode;
    MyElementBase* _endNode;
    MyElementBase* _arrowHead;
    bool _isSetArrowHead;
    bool _isConnectedToNodes;
};

class MyClassicEdge : public MyEdgeBase {
    Q_OBJECT

public:

    MyClassicEdge(const QString& name, MyElementBase* startNode, MyElementBase* endNode);

    EDGE_TYPE edgeType() override;
};

const QPointF getEndOfTheLinePosition(MyElementBase* mainNode, MyElementBase* connectedNode);

qreal getRadius(const QRectF& rect, const QPointF& point);

#endif
