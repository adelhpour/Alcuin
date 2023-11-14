#include "negui_menu_button_manager.h"
#include "negui_menu_button_builder.h"

MyMenuButtonManager::MyMenuButtonManager() {

}

QList<QAbstractButton*> MyMenuButtonManager::getToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    return createToolbarMenuButtons(interactor, undoStack, plugins, iconsDirectoryPath);
}

QList<QAbstractButton*> MyMenuButtonManager::getModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    return createModeMenuButtons(interactor, plugins, iconsDirectoryPath);
}