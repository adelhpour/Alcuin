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

    // import interface
    bool setImportInterface(ImportInterface* importInterface, const QString &appPath, const QString &pluginsPath);
    ImportInterface* importInterface();
    const bool isSetImportInterface() const { return _isSetImportInterface; }

    // data export interface
    bool setDataExportInterface(DataExportInterface* dataExportInterface, const QString &appPath, const QString &pluginsPath);
    DataExportInterface* dataExportInterface();
    const bool isSetDataExportInterface() const { return _isSetDataExportInterface; }

    // print export interface
    bool setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &appPath, const QString &pluginsPath);
    PrintExportInterface* printExportInterface();
    const bool isSetPrintExportInterface() const { return _isSetPrintExportInterface; }

    // element style interface
    bool setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &appPath, const QString &pluginsPath);
    ElementStyleInterface* elementStyleInterface();
    const bool isSetElementStyleInterface() const { return _isSetElementStyleInterface; }

    // autolayout interface
    bool setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &appPath, const QString &pluginsPath);
    AutoLayoutInterface* autoLayoutInterface();
    const bool isSetAutoLayoutInterface() const { return _isSetAutoLayoutInterface; }

    QList<MyPluginItemBase*>& pluginItems();

    QStringList listOfPluginItemNames(const QString type);

    QStringList listOfPluginItemCategories(const QString type);

    void addPluginItem(MyPluginItemBase* pluginItem);

    void readFromFile(const QString& importToolName);

    void readFromFile(MyPluginItemBase* importTool);

    void writeDataToFile(const QString& exportToolName);

    void writeDataToFile(MyPluginItemBase* exportTool);

    void writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName);

    void writeFigureToFile(const QString& exportToolName);

    void writeFigureToFile(MyPluginItemBase* exportTool);

    void autoLayout(MyPluginItemBase* autoLayoutEngine);

signals:

    QString askForApplicationDirectoryPath();

    QString askForWorkingDirectoryPath();

    QString askForCurrentBaseFileName();

    QJsonObject askForNetworkInfo();

    void networkInfoIsReadFromFile(const QJsonObject&, MyPluginItemBase*, const QString&);

    void networkInfoIsWrittenToFile(MyPluginItemBase*, const QString&);

    void askForExportFigure(const QString& fileName, const QString& fileExtension);

    void auotLayoutAlgorithmIsApplied(const QJsonObject&);

protected:

    // import interface
    ImportInterface* _importInterface;
    bool _isSetImportInterface;
    QList<MyPluginItemBase*> _importTools;

    // data export interface
    DataExportInterface* _dataExportInterface;
    bool _isSetDataExportInterface;
    QList<MyPluginItemBase*> _dataExportTools;

    // print export interface
    PrintExportInterface* _printExportInterface;
    bool _isSetPrintExportInterface;
    QList<MyPluginItemBase*> _printExportTools;

    // element style interface
    ElementStyleInterface* _elementStyleInterface;
    bool _isSetElementStyleInterface;

    // autolayout interface
    AutoLayoutInterface* _autoLayoutInterface;
    bool _isSetAutoLayoutInterface;
    QList<MyPluginItemBase*> _autoLayoutEngines;

    QList<MyPluginItemBase*> _pluginItems;
};

#endif
