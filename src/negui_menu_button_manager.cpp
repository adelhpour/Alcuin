#include "negui_menu_button_manager.h"
#include "negui_menu_button_builder.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"

MyMenuButtonManager::MyMenuButtonManager() {

}

QList<QAbstractButton*> MyMenuButtonManager::getToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    return createToolbarMenuButtons(interactor, undoStack, plugins, iconsDirectoryPath);
}

QList<QAbstractButton*> MyMenuButtonManager::getModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    addDefaultNodeStyle(plugins);
    addDefaultEdgeStyle(plugins);
    return createModeMenuButtons(interactor, plugins, iconsDirectoryPath);
}

void MyMenuButtonManager::addDefaultNodeStyle(QList<MyPluginItemBase*> plugins) {
    if (!getPluginsOfType(plugins, "nodestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        askForAddPluginItem(createNodeStyle(styleObject));
    }
}

void MyMenuButtonManager::addDefaultEdgeStyle(QList<MyPluginItemBase*> plugins) {
    if (!getPluginsOfType(plugins, "edgestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        askForAddPluginItem(createEdgeStyle(styleObject));
    }
}