#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        if (isConnectedToCentroidNodeEdgeStyle(json))
            style = new MyConnectedToCentroidNodeEdgeStyle(json["name"].toString());
        else
            style = new MyClassicEdgeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}

const bool isConnectedToCentroidNodeEdgeStyle(const QJsonObject &json) {
    return  false;
}
