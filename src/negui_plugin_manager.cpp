#include "negui_plugin_manager.h"
#include "negui_plugin_item_builder.h"
#include "negui_import_tools.h"
#include "negui_export_tools.h"
#include "negui_plugin_item_call_function.h"

#include <QPluginLoader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// MyPluginManager

MyPluginManager::MyPluginManager() {
    _generalInterface = NULL;
}

void MyPluginManager::load() {
    QString applicationDirectoryPath = askForApplicationDirectoryPath();
    QDir pluginsDir = getPluginsDirectory(QDir(applicationDirectoryPath));
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = pluginLoader.instance();
        if (plugin) {
            if (qobject_cast<GeneralInterface *>(plugin))
                setGeneralInterface(qobject_cast<GeneralInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
        }
    }
}

const QDir MyPluginManager::getPluginsDirectory(const QDir applicationDirectory) {
    QDir pluginsDir = applicationDirectory;
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS")
        pluginsDir.cdUp();
#endif
    pluginsDir.cd("plugins");

    return pluginsDir;
}

void MyPluginManager::setGeneralInterface(GeneralInterface* generalInterface, const QString &appPath, const QString &pluginsPath) {
    if (generalInterface) {
        _generalInterface = generalInterface;
        if (!generalInterface->initialize(appPath, pluginsPath))
            readPluginItemsInfo(_generalInterface->loadItemsInfo());
    }
}

GeneralInterface* MyPluginManager::generalInterface() {
    return _generalInterface;
}

void MyPluginManager::readPluginItemsInfo(const QJsonObject &json) {
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray itemsArray = json["items"].toArray();
        MyPluginItemBase* item = NULL;
        for (int itemIndex = 0; itemIndex < itemsArray.size(); ++itemIndex) {
            QJsonObject itemObject = itemsArray[itemIndex].toObject();
            item = createPluginItem(itemsArray[itemIndex].toObject());
            if (item)
                addPluginItem(item);
        }
    }
}

QList<MyPluginItemBase*>& MyPluginManager::pluginItems() {
    return _pluginItems;
}

void MyPluginManager::addPluginItem(MyPluginItemBase* pluginItem) {
    _pluginItems.push_back(pluginItem);
}

QStringList MyPluginManager::listOfPluginItemNames(const QString type) {
    QStringList pluginItemNames;
    QList<MyPluginItemBase*> pluginsOfType = getPluginsOfType(pluginItems(), type);
    for (MyPluginItemBase* pluginOfType: pluginsOfType)
        pluginItemNames.push_back(pluginOfType->name());

    return pluginItemNames;
}

QStringList MyPluginManager::listOfPluginItemCategories(const QString type) {
    QStringList pluginItemCategories;
    QList<MyPluginItemBase*> pluginsOfType = getPluginsOfType(pluginItems(), type);
    for (MyPluginItemBase* pluginOfType: pluginsOfType)
        pluginItemCategories.push_back(pluginOfType->category());

    return pluginItemCategories;
}

void MyPluginManager::readFromFile(const QString& importToolName) {
    MyPluginItemBase* importTool = findPluginByName(getPluginsOfType(pluginItems(), "importtool"), importToolName);
    if (importTool)
        readFromFile(importTool);
}

void MyPluginManager::readFromFile(MyPluginItemBase* importTool) {
    QString fileName = ((MyImportTool*)importTool)->getOpenFileName(askForWorkingDirectoryPath());
    if (!fileName.isEmpty())
        emit networkInfoIsReadFromFile(generalInterface()->call(importTool->callFunctions().at(0)->name(), createReadFromFileInputList(importTool, fileName)), importTool, fileName);
}

const QStringList MyPluginManager::createReadFromFileInputList(MyPluginItemBase* importTool, const QString& fileName) {
    QStringList readFromFileInputList;
    readFromFileInputList.append(fileName);

    return readFromFileInputList;
}

void MyPluginManager::writeDataToFile(const QString& exportToolName) {
    MyPluginItemBase* exportTool = findPluginByName(getPluginsOfType(pluginItems(), "dataexporttool"), exportToolName);
    if (exportTool)
        writeDataToFile(exportTool);
}

void MyPluginManager::writeDataToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName(askForWorkingDirectoryPath(), askForCurrentBaseFileName());
    if (!fileName.isEmpty())
        writeDataToFile(exportTool, fileName);
}

void MyPluginManager::writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName) {
    QStringList checkForGraphInfoCompatibilityInputList = createCheckForGraphInfoCompatibilityInputList(exportTool);
    ((MyDataExportTool*)exportTool)->readCompatibilityInfo(generalInterface()->call(exportTool->callFunctions().at(0)->name(), createCheckForGraphInfoCompatibilityInputList(exportTool)));
    if (((MyDataExportTool*)exportTool)->isInfoCompatible()) {
        QStringList writeToFileInputList = createWriteToFileInputList(exportTool, fileName);
        generalInterface()->call(exportTool->callFunctions().at(1)->name(), writeToFileInputList);
        emit networkInfoIsWrittenToFile(exportTool, fileName);
    }
    ((MyDataExportTool*)exportTool)->showMessages();
}

const QStringList MyPluginManager::createCheckForGraphInfoCompatibilityInputList(MyPluginItemBase* exportTool) {
    QStringList checkForGraphInfoCompatibilityInputList;
    checkForGraphInfoCompatibilityInputList.append(QJsonDocument(askForNetworkInfo()).toJson(QJsonDocument::Compact));

    return checkForGraphInfoCompatibilityInputList;
}

const QStringList MyPluginManager::createWriteToFileInputList(MyPluginItemBase* exportTool, const QString& fileName) {
    QStringList writeToFileInputList;
    writeToFileInputList.append(QJsonDocument(askForNetworkInfo()).toJson(QJsonDocument::Compact));
    writeToFileInputList.append(fileName);

    return writeToFileInputList;
}

void MyPluginManager::writeFigureToFile(const QString& exportToolName) {
    MyPluginItemBase* exportTool = findPluginByName(getPluginsOfType(pluginItems(), "printexporttool"), exportToolName);
    if (exportTool)
        writeFigureToFile(exportTool);
}

void MyPluginManager::writeFigureToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName(askForWorkingDirectoryPath());
    if (!fileName.isEmpty())
        emit askForExportFigure(fileName, ((MyPrintExportTool*)exportTool)->fileExtension());
}

void MyPluginManager::defaultPluginAction(MyPluginItemBase* defaultPluginItem) {
    for (MyBase* callFunction : defaultPluginItem->callFunctions()) {
        auto pythonConnection = connect((MyPluginItemCallFunction*)callFunction, &MyPluginItemCallFunction::askForCallPythonFunction, this, [this] (const QString& name, const QJsonValue& inputs) {
                return generalInterface()->call1(name, inputs); });
        auto cPlusPlusConnection = connect((MyPluginItemCallFunction*)callFunction, &MyPluginItemCallFunction::askForCallCPlusPlusFunction, this, [this] (const QString& name, const QJsonValue& inputs) {
                return askForTriggerAPIAction(name, inputs); });
        ((MyPluginItemCallFunction*)callFunction)->call();
        disconnect(pythonConnection);
        disconnect(cPlusPlusConnection);
    }
}
