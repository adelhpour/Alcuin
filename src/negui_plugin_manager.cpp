#include "negui_plugin_manager.h"
#include "negui_plugin_item_builder.h"
#include "negui_import_tools.h"
#include "negui_autolayout_engines.h"
#include "negui_export_tools.h"

#include <QPluginLoader>
#include <QJsonObject>
#include <QJsonArray>

// MyPluginManager

MyPluginManager::MyPluginManager() {
    // import interface
    _importInterface = NULL;
    _isSetImportInterface = false;

    // data export interface
    _dataExportInterface = NULL;
    _isSetDataExportInterface = false;

    // print export interface
    _printExportInterface = NULL;
    _isSetPrintExportInterface = false;

    // element style interface
    _elementStyleInterface = NULL;
    _isSetElementStyleInterface = false;

    // autolayout interface
    _autoLayoutInterface = NULL;
    _isSetAutoLayoutInterface = false;
}

void MyPluginManager::load() {
    QString applicationDirectoryPath = askForApplicationDirectoryPath();
    QDir pluginsDir = getPluginsDirectory(QDir(applicationDirectoryPath));
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = pluginLoader.instance();
        if (plugin) {
            // import interface
            if (qobject_cast<ImportInterface *>(plugin))
                setImportInterface(qobject_cast<ImportInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
                // data export interface
            else if (qobject_cast<DataExportInterface *>(plugin))
                setDataExportInterface(qobject_cast<DataExportInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
                // print export interface
            else if (qobject_cast<PrintExportInterface *>(plugin))
                setPrintExportInterface(qobject_cast<PrintExportInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
                // element style interface
            else if (qobject_cast<ElementStyleInterface *>(plugin))
                setElementStyleInterface(qobject_cast<ElementStyleInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
                // auto layout interface
            else if (qobject_cast<AutoLayoutInterface *>(plugin))
                setAutoLayoutInterface(qobject_cast<AutoLayoutInterface *>(plugin), applicationDirectoryPath, pluginsDir.path());
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

bool MyPluginManager::setImportInterface(ImportInterface* importInterface, const QString &appPath, const QString &pluginsPath) {
    if (importInterface) {
        _importInterface = importInterface;
        if (!_importInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_importInterface->loadItemsInfo());
            _isSetImportInterface = true;
        }
    }

    return _isSetImportInterface;
}

ImportInterface* MyPluginManager::importInterface() {
    return _importInterface;
}

bool MyPluginManager::setDataExportInterface(DataExportInterface* dataExportInterface, const QString &appPath, const QString &pluginsPath) {
    if (dataExportInterface) {
        _dataExportInterface = dataExportInterface;
        if (!_dataExportInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_dataExportInterface->loadItemsInfo());
            _isSetDataExportInterface = true;
        }
    }

    return _isSetDataExportInterface;
}

DataExportInterface* MyPluginManager::dataExportInterface() {
    return _dataExportInterface;
}

bool MyPluginManager::setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &appPath, const QString &pluginsPath) {
    if (printExportInterface) {
        _printExportInterface = printExportInterface;
        if (!_printExportInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_printExportInterface->loadItemsInfo());
            _isSetPrintExportInterface = true;
        }
    }

    return _isSetPrintExportInterface;
}

PrintExportInterface* MyPluginManager::printExportInterface() {
    return _printExportInterface;
}

bool MyPluginManager::setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &appPath, const QString &pluginsPath) {
    if (elementStyleInterface) {
        _elementStyleInterface = elementStyleInterface;
        if (!_elementStyleInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_elementStyleInterface->loadItemsInfo());
            _isSetElementStyleInterface = true;
        }
    }

    return _isSetElementStyleInterface;
}

ElementStyleInterface* MyPluginManager::elementStyleInterface() {
    return _elementStyleInterface;
}

bool MyPluginManager::setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &appPath, const QString &pluginsPath) {
    if (autoLayoutInterface) {
        _autoLayoutInterface = autoLayoutInterface;
        if (!_autoLayoutInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_autoLayoutInterface->loadItemsInfo());
            _isSetAutoLayoutInterface = true;
        }
    }

    return _isSetAutoLayoutInterface;
}

AutoLayoutInterface* MyPluginManager::autoLayoutInterface() {
    return _autoLayoutInterface;
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
    if (!fileName.isEmpty()) {
        QJsonObject networkInfo = (importInterface()->readGraphInfoFromFile(fileName, importTool->name()));
        emit networkInfoIsReadFromFile(networkInfo, importTool, fileName);
    }
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
    QJsonObject graphInfoObject = askForNetworkInfo();
    ((MyDataExportTool*)exportTool)->readCompatibilityInfo(dataExportInterface()->checkForGraphInfoCompatibility(graphInfoObject, exportTool->name()));
    if (((MyDataExportTool*)exportTool)->isInfoCompatible()) {
        dataExportInterface()->writeGraphInfoToFile(graphInfoObject, fileName, exportTool->name());
        emit networkInfoIsWrittenToFile(exportTool, fileName);
    }
    ((MyDataExportTool*)exportTool)->showMessages();
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

void MyPluginManager::autoLayout(const QString& autoLayoutEngineName) {
    MyPluginItemBase* autolayoutEngine = findPluginByName(getPluginsOfType(pluginItems(), "autolayoutengine"), autoLayoutEngineName);
    if (autolayoutEngine)
        autoLayout(autolayoutEngine);
}

void MyPluginManager::autoLayout(MyPluginItemBase* autoLayoutEngine) {
    if (!((MyAutoLayoutEngine*) autoLayoutEngine)->takeParameters()) {
        QJsonObject autoLayoutInfoObject;
        autoLayoutEngine->write(autoLayoutInfoObject);
        QJsonObject graphInfoObject = askForNetworkInfo();
        autoLayoutInterface()->autoLayout(graphInfoObject, autoLayoutInfoObject);
        emit auotLayoutAlgorithmIsApplied(graphInfoObject);
    }
}
