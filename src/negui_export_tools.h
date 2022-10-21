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
    
    // get the save file name from the user by poping up a file dialog
    const QString getSaveFileName() const;
    
    // get the icon associated with this plugin
    const QIcon icon() override;
    
    // get the icon dirctory of the export tool
    const QString& iconDirectory() const;
    
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
    
    QJsonObject getGraphInfoObject();
    
    const QList<QString>& nodeTypes() const;
    
    const QList<QString>& edgeTypes() const;
    
    void readCompatibilityInfo(const QJsonObject &json);
    
    const bool isInfoCompatible() const { return _isInfoCompatible; };
    
    const QString defaultUnsuccessfullExport() const;
    
    const QString messagesAsString() const;
    
    void showMessages();
    
    void annotateElementsWithTypes(QJsonObject &json);
    
    void selectElements();
    
    void annotateSelectedNodesWithNodeType(QList<QString> selectedNodes, const QString& nodeType);
    
    void annotateSelectedEdgesWithEdgeType(QList<QString> selectedEdges, const QString& edgeType);
    
    // read the export info from the json object
    void read(const QJsonObject &json) override;
    
    // write the export info to the json object
    void write(QJsonObject &json) override;
    
signals:
    void elementSelectionStarted();
    void askForNodeSelection(const QString& nodeType);
    const QList<QString> askForSelectedNodes();
    void askForEdgeSelection(const QString& edgeType);
    const QList<QString> askForSelectedEdges();
    void elementSelectionFinished(MyPluginItemBase*);
    
public slots:
    void annotateSelectedElements();

protected:
    QJsonObject _graphInfo;
    QList<QString> _nodeTypes;
    qint32 _nodeTypeIndex;
    QList<QString> _edgeTypes;
    qint32 _edgeTypeIndex;
    bool _isInfoCompatible;
    QList<QString> _messages;
};

class MyPrintExportTool : public MyExportToolBase {
public:
    
    MyPrintExportTool(const QString& name);
    
    QPrinter::OutputFormat outputFormat();
    
protected:
    QPrinter::OutputFormat _outputFormat;
};

#endif
