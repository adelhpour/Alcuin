#include "negui_plugin_item_builder.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_template_style_builder.h"
#include "negui_default_plugin_item.h"

MyPluginItemBase* createPluginItem(const QJsonObject &json) {
    if (json.contains("type") && json["type"].isString()) {
        if (json["type"].toString() == "importtool")
            return createImportTool(json);
        else if (json["type"].toString() == "dataexporttool")
            return createDataExportTool(json);
        else if (json["type"].toString() == "printexporttool")
            return createPrintExportTool(json);
        else if (json["type"].toString() == "nodestyle")
            return createNodeStyle(json);
        else if (json["type"].toString() == "edgestyle")
            return createEdgeStyle(json);
        else if (json["type"].toString() == "templatestyle")
            return createTemplateStyle(json);
    }

    return createDefaultPlugin(json);
}

MyPluginItemBase* createImportTool(const QJsonObject &json) {
    MyPluginItemBase* pluginItem = NULL;
    if (json.contains("name") && json["name"].isString()) {
        pluginItem = new MyPluginItemBase(json["name"].toString());
        pluginItem->read(json);
    }

    return pluginItem;
}

MyPluginItemBase* createDataExportTool(const QJsonObject &json) {
    MyPluginItemBase* pluginItem = NULL;
    if (json.contains("name") && json["name"].isString()) {
        pluginItem = new MyPluginItemBase(json["name"].toString());
        pluginItem->read(json);
    }

    return pluginItem;
}

MyPluginItemBase* createPrintExportTool(const QJsonObject &json) {
    MyPluginItemBase* pluginItem = NULL;
    if (json.contains("name") && json["name"].isString()) {
        pluginItem = new MyPluginItemBase(json["name"].toString());
        pluginItem->read(json);
    }

    return pluginItem;
}

MyPluginItemBase* createDefaultPlugin(const QJsonObject &json) {
    MyPluginItemBase* pluginItem = NULL;
    if (json.contains("name") && json["name"].isString()) {
        pluginItem = new MyDefaultPluginItem(json["name"].toString());
        pluginItem->read(json);
    }

    return pluginItem;
}
