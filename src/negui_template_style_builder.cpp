#include "negui_template_style_builder.h"
#include "negui_template_style.h"

MyNetworkElementStyleBase* createTemplateStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyTemplateStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}