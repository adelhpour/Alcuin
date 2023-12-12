#ifndef __NEGUI_FILE_MANAGER_H
#define __NEGUI_FILE_MANAGER_H

#include "negui_plugin_item_base.h"

#include <QObject>

class MyFileManager : public QObject {
    Q_OBJECT

public:
    MyFileManager(QList<MyPluginItemBase*> importTools, QList<MyPluginItemBase*> exportTools);

    void reset();

    QList<MyPluginItemBase*> importTools();

    QList<MyPluginItemBase*> exportTools();

    void resetCurrentFileName();

    void setCurrentFileName(const QString& fileName);

    const QString& currentFileName();

    const QString currentBaseFileName();

    void setWorkingDirectoryPath(const QString& workingDirectoryPath);

    const QString& workingDirectoryPath();

    void resetCurrentExportTool();

    void setCurrentExportTool(MyPluginItemBase* exportTool);

    void setCurrentExportToolCompatibleWithImportTool(MyPluginItemBase* importTool);

    MyPluginItemBase* currentExportTool();

    void resetLastSavedFileName();

    void setLastSavedFileName(const QString& fileName);

    const QString lastSavedFileName();

    const QString createDefaultFileName();

    const bool canSaveCurrentNetwork();

    const bool isWillingToSaveCurrentNetwork();

    const bool isCurrentNetworkUnsaved();

    void setCurrentNetworkUnsaved(const bool& currentNetworkUnsaved);

    const QString getOpenFileName(const QString& fileExtension);

    const QString getSaveFileName(const QString& fileExtension);

signals:

    QJsonObject askForNetworkInfo();
    void currentFileNameIsUpdated(const QString&);

protected:
    QList<MyPluginItemBase*> _importTools;
    QList<MyPluginItemBase*> _exportTools;
    QString _currentFileName;
    QString _workingDirectoryPath;
    QString _lastSavedFileName;
    MyPluginItemBase* _currentExportTool;
    qint32 _defaultFileNameIndex;
    bool _isCurrentNetworkUnsaved;
};

MyPluginItemBase* getDataExportTool(QList<MyPluginItemBase*> dataExportTools, const QString& fileName);

#endif
