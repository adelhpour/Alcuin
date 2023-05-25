#include "negui_edge_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_edge.h"
#include "negui_edge_style.h"

MyNetworkElementBase* createEdge(const QString& name, MyElementStyleBase* edgeStyle, MyNetworkElementBase* startNode, MyNetworkElementBase* endNode) {
    MyNetworkElementBase* edge = NULL;
    if (((MyEdgeStyleBase*)edgeStyle)->edgeStyleType() == MyEdgeStyleBase::CLASSIC_EDGE_STYLE)
        edge = new MyClassicEdge(name, startNode, endNode);

    if (edge)
        edge->setStyle(edgeStyle);

    return edge;
}

MyNetworkElementBase* createEdge(const QJsonObject &json, MyNetworkElementBase* startNode, MyNetworkElementBase* endNode) {
    MyNetworkElementBase* edge = NULL;
    if (startNode && endNode && json.contains("id") && json["id"].isString()) {
        if (json.contains("style") && json["style"].isObject()) {
            MyElementStyleBase* edgeStyle = createEdgeStyle(json["style"].toObject());
            if (edgeStyle)
                edge = createEdge(json["id"].toString(), edgeStyle, startNode, endNode);
        }
    }

    return edge;
}
