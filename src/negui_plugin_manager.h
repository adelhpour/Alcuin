#ifndef __NEGUI_PLUGIN_MANAGER_H
#define __NEGUI_PLUGIN_MANAGER_H

#include "negui_plugin_item_base.h"
#include "negui_interfaces.h"

#include <QDir>

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

    QStringList listOfPluginItemNames(const QString type);

    QStringList listOfPluginItemCategories(const QString type);

    void addPluginItem(MyPluginItemBase* pluginItem);

    void readFromFile(const QString& importToolName);

    void readFromFile(MyPluginItemBase* importTool);

    const QStringList createReadFromFileInputList(MyPluginItemBase* importTool, const QString& fileName);

    void processReadFromFileOutputList(const QStringList& readFromFileOutputList, MyPluginItemBase* importTool, const QString& fileName);

    void writeDataToFile(const QString& exportToolName);

    void writeDataToFile(MyPluginItemBase* exportTool);

    void writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName);

    const QStringList createCheckForGraphInfoCompatibilityInputList(MyPluginItemBase* exportTool);

    void processCheckForGraphInfoCompatibilityOutputList(const QStringList& checkForGraphInfoCompatibilityOutputList, MyPluginItemBase* exportTool);

    const QStringList createWriteToFileInputList(MyPluginItemBase* exportTool, const QString& fileName);

    void writeFigureToFile(const QString& exportToolName);

    void writeFigureToFile(MyPluginItemBase* exportTool);

    void autoLayout(const QString& autoLayoutEngineName);

    void autoLayout(MyPluginItemBase* autoLayoutEngine);

    const QStringList createAutoLayoutInputList(MyPluginItemBase* autoLayoutEngine);

    void processAutoLayoutOutputList(const QStringList& autoLayoutOutputList);

signals:

    QString askForApplicationDirectoryPath();

    QString askForWorkingDirectoryPath();

    QString askForCurrentBaseFileName();

    QJsonObject askForNetworkInfo();

    void networkInfoIsReadFromFile(const QJsonObject&, MyPluginItemBase*, const QString&);

    void networkInfoIsWrittenToFile(MyPluginItemBase*, const QString&);

    void askForExportFigure(const QString& fileName, const QString& fileExtension);

    void autoLayoutAlgorithmIsApplied(const QJsonObject&);

protected:

    GeneralInterface* _generalInterface;
    QList<MyPluginItemBase*> _pluginItems;
};

#endif
