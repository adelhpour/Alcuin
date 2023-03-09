#include "negui_template_style_builder.h"
#include "negui_template_style.h"

MyElementStyleBase* createTemplateStyle(const QJsonObject &json) {
    MyElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyTemplateStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}