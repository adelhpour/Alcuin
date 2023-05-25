#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyClassicEdgeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}
