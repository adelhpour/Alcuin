#ifndef __NEGUI_INTERACTOR_H
#define __NEGUI_INTERACTOR_H

#include "negui_element_base.h"
#include "negui_interfaces.h"

#include <QObject>
#include <QPluginLoader>
#include <QUndoStack>
#include <QWidgetAction>
#include <QPrinter>

class MyInteractor : public QObject {
    Q_OBJECT
    
public:
    
    typedef enum {
        NORMAL_MODE,
        ADD_NODE_MODE,
        SELECT_NODE_MODE,
        ADD_EDGE_MODE,
        SELECT_EDGE_MODE,
        REMOVE_MODE,
    } SceneMode;
    
    MyInteractor(QObject *parent = nullptr);
    
    // import interface
    bool setImportInterface(ImportInterface* importInterface, const QString &path);
    ImportInterface* importInterface();
    const bool isSetImportInterface() const { return _isSetImportInterface; }
    QList<MyPluginItemBase*>& importTools();
    void readImportInfo(const QJsonObject &json);
    void clearImportInfo();
    
    // data export interface
    bool setDataExportInterface(DataExportInterface* dataExportInterface, const QString &path);
    DataExportInterface* dataExportInterface();
    const bool isSetDataExportInterface() const { return _isSetDataExportInterface; }
    QList<MyPluginItemBase*>& dataExportTools();
    void readDataExportInfo(const QJsonObject &json);
    void clearDataExportInfo();
    void disconnectPressedEnterKeyFromDataExportTools();
    
    // print export interface
    bool setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &path);
    PrintExportInterface* printExportInterface();
    const bool isSetPrintExportInterface() const { return _isSetPrintExportInterface; }
    QList<MyPluginItemBase*>& printExportTools();
    void readPrintExportInfo(const QJsonObject &json);
    void clearPrintExportInfo();
    
    // node style interface
    bool setNodeStyleInterface(NodeStyleInterface* nodeStyleInterface, const QString &path);
    NodeStyleInterface* nodeStyleInterface();
    const bool isSetNodeStyleInterface() const { return _isSetNodeStyleInterface; }
    QList<MyPluginItemBase*>& nodeStyles();
    void readNodeStylesInfo(const QJsonObject &json);
    void clearNodeStylesInfo();
    
    // edge style interface
    bool setEdgeStyleInterface(EdgeStyleInterface* edgeStyleInterface, const QString &path);
    EdgeStyleInterface* edgeStyleInterface();
    const bool isSetEdgeStyleInterface() const { return _isSetEdgeStyleInterface; }
    QList<MyPluginItemBase*>& edgeStyles();
    void clearEdgeStylesInfo();
    void readEdgeStylesInfo(const QJsonObject &json);
    
    // autolayout interface
    bool setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &path);
    AutoLayoutInterface* autoLayoutInterface();
    const bool isSetAutoLayoutInterface() const { return _isSetAutoLayoutInterface; }
    QList<MyPluginItemBase*>& autoLayoutEngines();
    void readAutoLayoutInfo(const QJsonObject &json);
    void clearAutoLayoutInfo();
    
    // undo stack
    QUndoStack* undoStack();
    
    // modes
    void setMode(SceneMode mode);
    SceneMode mode();
    
    // network
    void resetNetwork();
    void setNetworkExtents(qreal x, qreal y, qreal width, qreal height);
    const QRectF& networkExtents();
    
    // nodes
    QList<MyElementBase*>& nodes();
    QString getNodeUniqueId();
    MyElementBase* findNode(const QString& name);
    void addNode(const QJsonObject& json);
    void addNode(MyElementBase* node);
    void removeNode(MyElementBase* node);
    void updateNodeParetns();
    void clearNodesInfo();
    void setNodeStyle(MyElementStyleBase* style);
    MyElementStyleBase* nodeStyle();
    MyElementStyleBase* getCopyNodeStyle(const QString& name);
    
    // edges
    QList<MyElementBase*>& edges();
    QString getEdgeUniqueId();
    MyElementBase* findEdge(const QString& name);
    void addEdge(const QJsonObject& json);
    void addEdge(MyElementBase* edge);
    void removeEdge(MyElementBase* edge);
    void clearEdgesInfo();
    void setEdgeStyle(MyElementStyleBase* style);
    MyElementStyleBase* edgeStyle();
    MyElementStyleBase* getCopyEdgeStyle(const QString& name);
    void setSelectedEdgeStartNode(MyElementBase* n);
    void unSetSelectedEdgeStartNode();
    MyElementBase* selectedEdgeStartNode();
    const bool isSetSelectedEdgeStartNode() const { return _isSetSelectedEdgeStartNode; }
    bool edgeExists(MyElementBase* n1, MyElementBase* n2);
    
signals:
    
    void askForExportFigure(const QString& fileName, QPrinter::OutputFormat outputFormat);
    void askForAddGraphicsItem(QGraphicsItem* item);
    void askForRemoveGraphicsItem(QGraphicsItem* item);
    void askForSetSceneRect(qreal x, qreal y, qreal width, qreal height);
    void askForClearScene();
    void askForSetToolTip(const QString& toolTip);
    QList<QGraphicsItem *> askForItemsAtPosition(const QPointF& position);
    void nodeIsSelected(const QString& nodeName);
    void edgeIsSelected(const QString& edgeName);
    void enterKeyIsPressed();
    
public slots:
    
    // menus
    QList<QToolButton*> getMenuButtons();
    
    // network
    void createNetwork(const QJsonObject &json);
    void exportNetworkInfo(QJsonObject &json);
    
    // network elements
    void addNewNode(const QPointF& position);
    void selectNode(MyElementBase* element);
    void addNewEdge(MyElementBase* element);
    void selectEdge(MyElementBase* element);
    void removeItem(MyElementBase* element);
    const QList<QString> selectedNodes();
    const QList<QString> selectedEdges();
    
    // modes
    void enableNormalMode();
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableSelectNodeMode(const QString& nodeType = "");
    void enableAddEdgeMode(MyPluginItemBase* style);
    void enableSelectEdgeMode(const QString& edgeType = "");
    void enableRemoveMode();
    
private slots:
        
    void readFromFile(MyPluginItemBase* importTool);
    void annotateExportData(MyPluginItemBase* exportTool);
    void writeDataToFile(MyPluginItemBase* exportTool);
    void writeFigureToFile(MyPluginItemBase* exportTool);
    void autoLayout(MyPluginItemBase* autoLayoutEngine);
    MyElementBase* parentNodeAtPosition(MyElementBase* currentNode, const QPointF& position);
    
protected:
    
    // plugins
    void loadPlugins();
    
    // menus
    QToolButton* populateImportMenu();
    QToolButton* populateExportMenu();
    QToolButton* populateAddNodeMenu();
    QToolButton* populateAddEdgeMenu();
    QToolButton* populateRemoveItemMenu();
    QToolButton* populateAutoLayoutMenu();
    QToolButton* populateUndoActionMenu();
    QToolButton* populateRedoActionMenu();
    QToolButton* populateResetSceneMenu();
    
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
    
    // node style interface
    NodeStyleInterface* _nodeStyleInterface;
    bool _isSetNodeStyleInterface;
    QList<MyPluginItemBase*> _nodeStyles;
    MyElementStyleBase* _nodeStyle;
    
    // edge style interface
    EdgeStyleInterface* _edgeStyleInterface;
    bool _isSetEdgeStyleInterface;
    QList<MyPluginItemBase*> _edgeStyles;
    MyElementStyleBase* _edgeStyle;
    
    // autolayout interface
    AutoLayoutInterface* _autoLayoutInterface;
    bool _isSetAutoLayoutInterface;
    QList<MyPluginItemBase*> _autoLayoutEngines;
    
    // undo stack
    QUndoStack* _undoStack;
    
    // modes
    SceneMode _mode;
    
    // elements
    QList<MyElementBase*> _nodes;
    QList<MyElementBase*> _edges;
    MyElementBase* _selectedEdgeStartNode;
    bool _isSetSelectedEdgeStartNode;
    
    // network
    QRectF _networkExtents;
};

class MyUndoStack : public QUndoStack {
    
public:
    
    MyUndoStack(QObject *parent = nullptr);
    
    void addCommand(QUndoCommand* command);
    
    void clear();
};

class MyToolButton : public QToolButton {
    
public:
    
    MyToolButton(QWidget* parent = nullptr);
};

class MyToolButtonMenu : public QMenu {
    
public:
    
    MyToolButtonMenu(QWidget* parent = nullptr);
};

class MyWidgetAction : public QWidgetAction {
    Q_OBJECT
    
public:
    
    MyWidgetAction(QObject* parent = nullptr);
    
    void setItems(QList<MyPluginItemBase*> items);
    
signals:
    void itemIsChosen(MyPluginItemBase*);
    
protected:
    QWidget* createItemPreviewWidget(QList<MyPluginItemBase*> items);
};

class MyItemPreviewButton : public QPushButton {
    
public:
    
    MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent = nullptr);
};

class MyAddNodeCommand : public QUndoCommand {
    
public:
    
    MyAddNodeCommand(MyInteractor *interactor, MyElementBase* node, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

protected:
    
    MyElementBase* _node;
    MyInteractor* _interactor;
};

class MyRemoveNodeCommand : public QUndoCommand {
    
public:
    
    MyRemoveNodeCommand(MyInteractor *interactor, MyElementBase* node, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

protected:
    
    MyElementBase* _node;
    MyInteractor* _interactor;
};

class MyAddEdgeCommand : public QUndoCommand {
    
public:
    
    MyAddEdgeCommand(MyInteractor *interactor, MyElementBase* edge, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

protected:
    
    MyElementBase* _edge;
    MyInteractor* _interactor;
};

class MyRemoveEdgeCommand : public QUndoCommand {
    
public:
    
    MyRemoveEdgeCommand(MyInteractor *interactor, MyElementBase* edge, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

protected:
    
    MyElementBase* _edge;
    MyInteractor* _interactor;
};


#endif
