#include "negui_node_style_builder.h"
#include "negui_node_style.h"

MyElementStyleBase* createNodeStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyClassicNodeStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}
