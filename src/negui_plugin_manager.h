#ifndef __NEGUI_PLUGIN_MANAGER_H
#define __NEGUI_PLUGIN_MANAGER_H

#include "negui_plugin_item_base.h"
#include "negui_interfaces.h"

#include <QDir>
#include <QJsonDocument>

class MyPluginManager : public QObject {
    Q_OBJECT

public:

    MyPluginManager();

    void load();

    const QDir getPluginsDirectory(const QDir applicationDirectory);

    void readPluginItemsInfo(const QJsonObject &json);

    void setGeneralInterface(GeneralInterface* generalInterface, const QString &appPath, const QString &pluginsPath);

    GeneralInterface* generalInterface();

    QList<MyPluginItemBase*>& pluginItems();

    QJsonArray listOfPluginItemNames(const QString type);

    QJsonArray listOfPluginItemCategories(const QString type);

    void addPluginItem(MyPluginItemBase* pluginItem);

    void callPluginFunctions(MyPluginItemBase* plugin);

signals:

    QString askForApplicationDirectoryPath();

    QJsonValue askForCallAPIFunction(const QString&, const QJsonValue&);

protected:

    GeneralInterface* _generalInterface;
    QList<MyPluginItemBase*> _pluginItems;
};

#endif
