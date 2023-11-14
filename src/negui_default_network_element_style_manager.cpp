#include "negui_default_network_element_style_manager.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"

MyDefaultNetworkElementStyleManager::MyDefaultNetworkElementStyleManager() {

}

MyPluginItemBase* MyDefaultNetworkElementStyleManager::createDefaultNodeStyle(QList<MyPluginItemBase*> plugins) {
    if (!getPluginsOfType(plugins, "nodestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        return createNodeStyle(styleObject);
    }

    return NULL;
}

MyPluginItemBase* MyDefaultNetworkElementStyleManager::createDefaultEdgeStyle(QList<MyPluginItemBase*> plugins) {
    if (!getPluginsOfType(plugins, "edgestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        return createEdgeStyle(styleObject);
    }

    return NULL;
}
