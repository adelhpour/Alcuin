#ifndef __NEGUI_FILE_MANAGER_H
#define __NEGUI_FILE_MANAGER_H

#include "negui_plugin_item_base.h"
#include "negui_export_tools.h"
#include "negui_import_tools.h"

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

    const QString currentFileName();

    void resetCurrentExportTool();

    void setCurrentExportTool(MyPluginItemBase* exportTool);

    void setCurrentExportToolCompatibleWithImportTool(MyPluginItemBase* importTool);

    MyPluginItemBase* currentExportTool();

    void resetLastSavedFileName();

    void setLastSavedFileName(const QString& fileName);

    const QString lastSavedFileName();

    const QString createDefaultFileName();

signals:

    QJsonObject askForNetworkInfo();
    void currentFileNameIsUpdated(const QString&);

protected:
    QList<MyPluginItemBase*> _importTools;
    QList<MyPluginItemBase*> _exportTools;
    QString _currentFileName;
    QString _lastSavedFileName;;
    MyPluginItemBase* _currentExportTool;
    qint32 _defaultFileNameIndex;
};

MyPluginItemBase* getDataExportTool(QList<MyPluginItemBase*> dataExportTools, const QString& fileName);

#endif
