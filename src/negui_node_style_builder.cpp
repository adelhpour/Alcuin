#include "negui_node_style_builder.h"
#include "negui_node_style.h"

MyElementStyleBase* createNodeStyle(const QString& name) {
    return new MyClassicNodeStyle(name);
}

MyElementStyleBase* createNodeStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("style") && json["style"].isObject() && json["style"].toObject().contains("name") && json["style"]["name"].isString()) {
        style = createNodeStyle(json["style"]["name"].toString());
        style->read(json["style"].toObject());
    }

    return style;
}
