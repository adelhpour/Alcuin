#include "negui_plugin_item_builder.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_template_style_builder.h"

MyPluginItemBase* createPluginItem(const QJsonObject &json) {
    if (json.contains("type") && json["type"].isString()) {
        if (json["type"].toString() == "nodestyle")
            return createNodeStyle(json);
        else if (json["type"].toString() == "edgestyle")
            return createEdgeStyle(json);
        else if (json["type"].toString() == "templatestyle")
            return createTemplateStyle(json);
    }

    return createPluginItemBase(json);
}

MyPluginItemBase* createPluginItemBase(const QJsonObject &json) {
    MyPluginItemBase* pluginItem = NULL;
    if (json.contains("name") && json["name"].isString()) {
        pluginItem = new MyPluginItemBase(json["name"].toString());
        pluginItem->read(json);
    }

    return pluginItem;
}
