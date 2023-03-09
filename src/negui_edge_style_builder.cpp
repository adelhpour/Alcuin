#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

MyElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyClassicEdgeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}
