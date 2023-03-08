#include "negui_edge_style_builder.h"
#include "negui_edge_style.h"

MyElementStyleBase* createEdgeStyle(const QString& name) {
    return new MyClassicEdgeStyle(name);
}

MyElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("style") && json["style"].isObject() && json["style"].toObject().contains("name") && json["style"]["name"].isString()) {
        style = createEdgeStyle(json["style"]["name"].toString());
        style->read(json["style"].toObject());
    }

    return style;
}
