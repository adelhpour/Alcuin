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
    void readImportInfo(const QJsonObject &json);
    
    // data export interface
    bool setDataExportInterface(DataExportInterface* dataExportInterface, const QString &path);
    DataExportInterface* dataExportInterface();
    const bool isSetDataExportInterface() const { return _isSetDataExportInterface; }
    void readDataExportInfo(const QJsonObject &json);
    
    // print export interface
    bool setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &path);
    PrintExportInterface* printExportInterface();
    const bool isSetPrintExportInterface() const { return _isSetPrintExportInterface; }
    void readPrintExportInfo(const QJsonObject &json);
    
    // element style interface
    bool setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &path);
    ElementStyleInterface* elementStyleInterface();
    const bool isSetElementStyleInterface() const { return _isSetElementStyleInterface; }
    void readElementStylesInfo(const QJsonObject &json);
    
    // autolayout interface
    bool setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &path);
    AutoLayoutInterface* autoLayoutInterface();
    const bool isSetAutoLayoutInterface() const { return _isSetAutoLayoutInterface; }
    void readAutoLayoutInfo(const QJsonObject &json);
    
    QList<MyPluginItemBase*>& plugins();
    
    // undo stack
    QUndoStack* undoStack();
    
    // modes
    void setMode(SceneMode mode);
    SceneMode mode();
    
    // network
    void resetNetwork();
    void setNetworkExtents(const QJsonObject& json);
    void setNetworkExtents(qreal x, qreal y, qreal width, qreal height);
    const QRectF& networkExtents();
    
    // nodes
    QList<MyElementBase*>& nodes();
    void addNodes(const QJsonObject &json);
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
    void addEdges(const QJsonObject &json);
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
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableAddEdgeMode(MyPluginItemBase* style);
    void enableSelectEdgeMode(const QString& edgeCategory = "");
    void enableRemoveMode();
    
private slots:
        
    void readFromFile(MyPluginItemBase* importTool);
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
    QList<QToolButton*> populateAddElementMenu();
    QToolButton* createPluginsOfCategoryAddNodeMenuButton(QList<MyPluginItemBase*> pluginsOfCategory, const QString& category);
    QToolButton* createPluginItemToolButton(QMenu* subMenu, const QString& text);
    QWidgetAction* createNodeStyleWidgetAction(QList<MyPluginItemBase*> nodeStyles, QWidget* parent);
    QWidgetAction* createEdgeStyleWidgetAction(QList<MyPluginItemBase*> edgeStyles, QWidget* parent);
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
    
    // element style interface
    ElementStyleInterface* _elementStyleInterface;
    bool _isSetElementStyleInterface;
    
    // autolayout interface
    AutoLayoutInterface* _autoLayoutInterface;
    bool _isSetAutoLayoutInterface;
    QList<MyPluginItemBase*> _autoLayoutEngines;
    
    // element styles
    MyElementStyleBase* _nodeStyle;
    MyElementStyleBase* _edgeStyle;
    
    QList<MyPluginItemBase*> _plugins;
    
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
    QPushButton* createItemPreviewButton(MyPluginItemBase* item);
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

MyElementBase* findElement(QList<MyElementBase*> elements, const QString& name);
MyElementBase* findStartNode(QList<MyElementBase*> nodes, const QJsonObject &json);
MyElementBase* findEndNode(QList<MyElementBase*> nodes, const QJsonObject &json);
QString getElementUniqueId(QList<MyElementBase*> elements, const QString& defaultIdSection);
bool nodesAreConnectable(MyElementBase* n1, MyElementBase* n2);

#endif
