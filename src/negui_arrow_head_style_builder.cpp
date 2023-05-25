#include "negui_arrow_head_style_builder.h"
#include "negui_arrow_head_style.h"

MyNetworkElementStyleBase* createArrowHeadStyle(const QJsonObject &json) {
    MyNetworkElementStyleBase* style = NULL;
    if (json.contains("name") && json["name"].isString()) {
        style = new MyClassicArrowHeadStyle(json["name"].toString());
        style->read(json);
    }

    return style;
}
