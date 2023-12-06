#ifndef __NEGUI_EXPORT_TOOLS_H
#define __NEGUI_EXPORT_TOOLS_H

#include "negui_plugin_item_base.h"
#include <QPrinter>
#include <QJsonObject>

class MyExportToolBase : public MyPluginItemBase {
public:
    
    MyExportToolBase(const QString& name);
    
    // get the file extension of the export tool
    const QString& fileExtension() const;

    // get the save file name from the user by popping up a file dialog and using the entered save file name
    const QString getSaveFileName(const QString& workingDirectory, const QString& saveFileName) const;
    
    // get the save file name from the user by popping up a file dialog
    const QString getSaveFileName(const QString& workingDirectory) const;

    // get the default save file name of the export tool
    const QString& defaultSaveFileName() const;
    
    // read the export info from the json object
    void read(const QJsonObject &json) override;
    
    // write the export info to the json object
    void write(QJsonObject &json) override;
    
protected:
    QString _fileExtension;
    QString _iconDirectory;
    QString _defaultSaveFileName;
};

class MyDataExportTool : public QObject, public MyExportToolBase {
    Q_OBJECT
public:
    
    MyDataExportTool(const QString& name);
    
    const QString type() const override;
    
    QJsonObject getGraphInfoObject();
    
    const QList<QString>& nodeTypes() const;
    
    const QList<QString>& edgeTypes() const;
    
    void readCompatibilityInfo(const QJsonObject &json);
    
    const bool isInfoCompatible() const { return _isInfoCompatible; };
    
    const QString defaultUnsuccessfullExport() const;
    
    const QString messagesAsString() const;
    
    void showMessages();

protected:
    bool _isInfoCompatible;
    QList<QString> _messages;
};

class MyPrintExportTool : public MyExportToolBase {
public:
    
    MyPrintExportTool(const QString& name);
    
    const QString type() const override;
};

#endif
