#include "negui_edge_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_edge.h"
#include "negui_edge_style.h"

MyElementBase* createEdge(const QString& name, MyElementStyleBase* edgeStyle, MyElementBase* startNode, MyElementBase* endNode) {
    MyElementBase* edge = NULL;
    if (((MyEdgeStyleBase*)edgeStyle)->edgeStyleType() == MyEdgeStyleBase::CLASSIC_EDGE_STYLE)
        edge = new MyClassicEdge(name, startNode, endNode);

    if (edge)
        edge->setStyle(edgeStyle);

    return edge;
}

MyElementBase* createEdge(const QJsonObject &json, MyElementBase* startNode, MyElementBase* endNode) {
    MyElementBase* edge = NULL;
    if (startNode && endNode && json.contains("id") && json["id"].isString()) {
        if (json.contains("style") && json["style"].isObject()) {
            MyElementStyleBase* edgeStyle = createEdgeStyle(json["style"].toObject());
            if (edgeStyle)
                edge = createEdge(json["id"].toString(), edgeStyle, startNode, endNode);
        }
    }

    return edge;
}
