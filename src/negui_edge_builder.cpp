#include "negui_edge_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_edge.h"
#include "negui_edge_style.h"

MyNetworkElementBase* createEdge(const QString& name, MyNetworkElementStyleBase* edgeStyle, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) {
    MyNetworkElementBase* edge = NULL;
    if (((MyEdgeStyleBase*)edgeStyle)->edgeStyleType() == MyEdgeStyleBase::CLASSIC_EDGE_STYLE)
        edge = new MyClassicEdge(name, sourceNode, targetNode);

    if (edge)
        edge->setStyle(edgeStyle);

    return edge;
}

MyNetworkElementBase* createEdge(const QJsonObject &json, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) {
    MyNetworkElementBase* edge = NULL;
    if (sourceNode && targetNode && json.contains("id") && json["id"].isString()) {
        if (json.contains("style") && json["style"].isObject()) {
            MyNetworkElementStyleBase* edgeStyle = createEdgeStyle(json["style"].toObject());
            if (edgeStyle)
                edge = createEdge(json["id"].toString(), edgeStyle, sourceNode, targetNode);
        }
    }

    return edge;
}
