#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        if (isConnectedToSourceCentroidNode())
            style = new MyConnectedToSourceCentroidNodeEdgeStyle(json["name"].toString());
        else if (isConnectedToTargetCentroidNode())
            style = new MyConnectedToTargetCentroidNodeEdgeStyle(json["name"].toString());
        else
            style = new MyClassicEdgeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}

const bool isConnectedToSourceCentroidNode() {
    return false;
}

const bool isConnectedToTargetCentroidNode() {
    return true;
}