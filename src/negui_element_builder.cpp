#include "negui_element_builder.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_arrow_head.h"

MyElementBase* createNode(const QString& name, const qreal& x, const qreal& y) {
    return new MyNode(name, x, y);
}

MyElementBase* createNode(const QJsonObject &json) {
    MyElementBase* node = NULL;
    if (json.contains("id") && json["id"].isString() && json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
        node = createNode(json["id"].toString(), json["position"]["x"].toDouble(), json["position"]["y"].toDouble());
        
        if (json.contains("parent") && json["parent"].isString())
            ((MyNode*)node)->setParentNodeId(json["parent"].toString());
    }
    
    return node;
}

MyElementBase* createEdge(const QString& name, MyElementBase* startNode, MyElementBase* endNode) {
    return new MyEdge(name, startNode, endNode);
}

MyElementBase* createEdge(const QJsonObject &json, MyElementBase* startNode, MyElementBase* endNode) {
    MyElementBase* edge = NULL;
    if (startNode && endNode && json.contains("id") && json["id"].isString()) {
        edge = createEdge(json["id"].toString(), startNode, endNode);
    }
    
    return edge;
}

MyElementBase* createArrowHead(const QString& name, MyElementBase* edge) {
    return new MyArrowHead(name, edge);
}
