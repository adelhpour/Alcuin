#include "negui_element_builder.h"
#include "negui_element_style_builder.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_arrow_head.h"

MyElementBase* createNode(const QString& name, MyElementStyleBase* nodeStyle, const qreal& x, const qreal& y) {
    MyElementBase* node = new MyNodeBase(name, x, y);
    node->setStyle(nodeStyle);
    return node;
}

MyElementBase* createNode(const QJsonObject &json) {
    MyElementBase* node = NULL;
    if (json.contains("id") && json["id"].isString() && json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
        MyElementStyleBase* nodeStyle = createNodeStyle(json);
        if (nodeStyle) {
            node = createNode(json["id"].toString(), nodeStyle, json["position"]["x"].toDouble(), json["position"]["y"].toDouble());
            if (json.contains("parent") && json["parent"].isString())
                ((MyNodeBase*)node)->setParentNodeId(json["parent"].toString());
        }
    }
    
    return node;
}

MyElementBase* createEdge(const QString& name, MyElementStyleBase* edgeStyle, MyElementBase* startNode, MyElementBase* endNode) {
    MyElementBase* edge = new MyEdge(name, startNode, endNode);
    edge->setStyle(edgeStyle);
    ((MyEdge*)edge)->connectToNodes(true);
    return edge;
}

MyElementBase* createEdge(const QJsonObject &json, MyElementBase* startNode, MyElementBase* endNode) {
    MyElementBase* edge = NULL;
    if (startNode && endNode && json.contains("id") && json["id"].isString()) {
        MyElementStyleBase* edgeStyle = createEdgeStyle(json);
        if (edgeStyle) {
            edge = createEdge(json["id"].toString(), edgeStyle, startNode, endNode);
        }
    }
    
    return edge;
}

MyElementBase* createArrowHead(const QString& name, MyElementStyleBase* arrowHeadStyle, MyElementBase* edge) {
    MyElementBase* arrowHead = new MyArrowHead(name, edge);
    arrowHead->setStyle(arrowHeadStyle);
    return arrowHead;
}
