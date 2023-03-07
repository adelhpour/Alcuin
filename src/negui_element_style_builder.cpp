#include "negui_element_style_builder.h"
#include "negui_node_style.h"
#include "negui_edge_style.h"
#include "negui_arrow_head_style.h"
#include "negui_template_style.h"

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

MyElementStyleBase* createEdgeStyle(const QString& name) {
    return new MyEdgeStyle(name);
}

MyElementStyleBase* createEdgeStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("style") && json["style"].isObject() && json["style"].toObject().contains("name") && json["style"]["name"].isString()) {
        style = createEdgeStyle(json["style"]["name"].toString());
        style->read(json["style"].toObject());
    }
    
    return style;
}

MyElementStyleBase* createArrowHeadStyle(const QString& name) {
    return new MyClassicArrowHeadStyle(name);
}

MyElementStyleBase* createTemplateStyle(const QString& name) {
    return new MyTemplateStyle(name);
}
