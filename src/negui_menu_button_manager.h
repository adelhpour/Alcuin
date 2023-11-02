#ifndef __NEGUI_MENU_BUTTON_MANAGER_H
#define __NEGUI_MENU_BUTTON_MANAGER_H

#include <QObject>
#include <QAbstractButton>

#include "negui_plugin_item_base.h"

class MyMenuButtonManager : public QObject {
    Q_OBJECT

public:

    MyMenuButtonManager();

    QList<QAbstractButton*> getToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

    QList<QAbstractButton*> getModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

    void addDefaultNodeStyle(QList<MyPluginItemBase*> plugins);

    void addDefaultEdgeStyle(QList<MyPluginItemBase*> plugins);

signals:

    void askForAddPluginItem(MyPluginItemBase*);
};

#endif