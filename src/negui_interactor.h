#ifndef __NEGUI_INTERACTOR_H
#define __NEGUI_INTERACTOR_H

#include "negui_network_element_base.h"
#include "negui_interfaces.h"
#include "negui_scene_mode_element_base.h"

#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QUndoStack>
#include <QWidgetAction>
#include <QToolButton>
#include <QPushButton>

class MyInteractor : public QObject, public MySceneModeElementBase {
    Q_OBJECT
    
public:
    
    MyInteractor(QObject *parent = nullptr);
    
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

    QList<MyPluginItemBase*>& plugins();
    
    // undo stack
    QUndoStack* undoStack();

    QObject* fileManager();
    
    // modes
    void setSceneMode(SceneMode sceneMode) override;
    
    // network
    void resetNetworkCanvas();
    void resetCanvas();
    void resetNetwork();
    void setNetworkExtents(const QJsonObject& json);
    void setNetworkExtents(qreal x, qreal y, qreal width, qreal height);
    const QRectF& networkExtents();
    
    // nodes
    QList<MyNetworkElementBase*>& nodes();
    void addNodes(const QJsonObject &json);
    void addNode(const QJsonObject& json);
    void addNode(MyNetworkElementBase* node);
    void removeNode(MyNetworkElementBase* node);
    void updateNodeParents();
    void clearNodesInfo();
    void setNodeStyle(MyNetworkElementStyleBase* style);
    MyNetworkElementStyleBase* nodeStyle();
    MyNetworkElementStyleBase* copiedNodeStyle();
    
    // edges
    QList<MyNetworkElementBase*>& edges();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void addEdge(MyNetworkElementBase* edge);
    void removeEdge(MyNetworkElementBase* edge);
    void clearEdgesInfo();
    void setEdgeStyle(MyNetworkElementStyleBase* style);
    MyNetworkElementStyleBase* edgeStyle();
    MyNetworkElementStyleBase* copiedEdgeStyle();
    QList<MyNetworkElementBase*> copiedNetworkElements();
    void resetCopiedNetworkElements();
    void deleteNewEdgeBuilder();
    bool edgeExists(MyNetworkElementBase* n1, MyNetworkElementBase* n2);

    QDir applicationDirectory();
    QDir iconsDirectory();
    
signals:
    
    void askForExportFigure(const QString& fileName, const QString& fileExtension);
    void askForAddGraphicsItem(QGraphicsItem* item);
    void askForRemoveGraphicsItem(QGraphicsItem* item);
    void askForSetSceneRect(qreal x, qreal y, qreal width, qreal height);
    void askForClearScene();
    void askForResetScale();
    void askForSetToolTip(const QString& toolTip);
    void askForDisplayFeatureMenu(QWidget*);
    QList<QGraphicsItem *> askForItemsAtPosition(const QPointF& position);
    void modeIsSet(const QString&);
    void currentFileNameIsUpdated(const QString&);
    const bool askForWhetherShiftModifierIsPressed();
    QRectF askForItemsBoundingRect();
    void elementsCuttableStatusChanged(const bool&);
    void elementsCopyableStatusChanged(const bool&);
    void pasteElementsStatusChanged(const bool&);
    
    void enterKeyIsPressed();
    
public slots:
    
    // menus
    QList<QToolButton*> getToolBarMenuButtons();
    QToolButton* getNormalModeButton();
    QList<QToolButton*> getAddModeButtons();
    
    // network
    void setNewNetworkCanvas();
    void createNetwork(const QJsonObject &json);
    QJsonObject exportNetworkInfo();
    
    // network elements
    void addNewNode(const QPointF& position);
    void addNewEdge(MyNetworkElementBase* element);
    void deleteNode(MyNetworkElementBase* node);
    void deleteEdge(MyNetworkElementBase* edge);
    void deleteSelectedNetworkElements();
    void alignSelectedNetworkElements(const QString& alignType);
    const QList<MyNetworkElementBase*> selectedNodes();
    const QList<MyNetworkElementBase*> selectedEdges();
    void selectElement(MyNetworkElementBase* element);
    void selectElements(const bool& selected);
    void selectElements(const bool& selected, const QString& category);
    void selectNodes(const bool& selected);
    void selectNodes(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdges(const bool& selected, const QString& category);
    void setCopiedNode(MyNetworkElementBase* node);
    void setCutNode(MyNetworkElementBase* node);
    void setCopiedNodeStyle(MyNetworkElementStyleBase* style);
    void setCopiedEdgeStyle(MyNetworkElementStyleBase* style);
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areSelectedElementsAlignable();
    const bool areAnyElementsCopied();
    const bool areAnyElementsSelected();
    
    // modes
    void enableNormalMode() override;
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableAddEdgeMode(MyPluginItemBase* style);
    void enableSelectMode(const QString& elementCategory = "");
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableSelectEdgeMode(const QString& edgeCategory = "");
    
    void clearElementsFocusedGraphicsItems();
    void displaySelectionArea(const QPointF& position);
    void clearSelectionArea();

    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const QPointF& position);

    // plugins
    const QStringList listOfPluginItemNames(const QString type);
    const QStringList listOfPluginItemCategories(const QString type);
    
private slots:

    void saveCurrentNetwork();

    void readFromFile(const QString& importToolName);
    void readFromFile(MyPluginItemBase* importTool);
    void writeDataToFile(const QString& exportToolName);
    void writeDataToFile(MyPluginItemBase* exportTool);
    void writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName);
    void writeFigureToFile(const QString& exportToolName);
    void writeFigureToFile(MyPluginItemBase* exportTool);
    void autoLayout(MyPluginItemBase* autoLayoutEngine);
    MyNetworkElementBase* parentNodeAtPosition(MyNetworkElementBase* currentNode, const QPointF& position);
    void createChangeStageCommand();
    void createSelectionAreaGraphicsItem(const QPointF& position);
    void selectSelectionAreaCoveredNodes();
    void selectSelectionAreaCoveredEdges();
    const bool isSetCopiedNodeStyle();
    void pasteCopiedNodeStyle(MyNetworkElementBase* element);
    const bool isSetCopiedEdgeStyle();
    void pasteCopiedEdgeStyle(MyNetworkElementBase* element);
    const bool areAnyOtherElementsSelected(MyNetworkElementBase* element);
    void copySelectedNetworkElements();
    void cutSelectedNetworkElements();
    
protected:

    // plugins
    void loadPlugins();

    // file manager
    void setFileManager();
    
    // menu buttons
    QToolButton* createNormalModeMenuButton();
    QToolButton* createImportMenuButton();
    QToolButton* createDataExportMenuButton();
    QToolButton* createPrintExportMenuButton();
    QToolButton* createSaveMenuButton();
    QList<QToolButton*> createAddElementMenuButtons();
    void addDefaultNodeStyle();
    void addDefaultEdgeStyle();
    QList<QToolButton*> createElementStyleButtons();
    QMenu* createCategoryMenu(QList<MyPluginItemBase*> nodeStylesOfCategory, QList<MyPluginItemBase*> edgeStylesOfCategory, QList<MyPluginItemBase*> templateStylesOfCategory);
    QToolButton* createPluginsOfCategoryAddNodeMenuButton(QList<MyPluginItemBase*> pluginsOfCategory, const QString& category);
    QToolButton* createPluginItemToolButton(QMenu* subMenu, const QString& text);
    QWidgetAction* createElementStyleWidgetAction(QList<MyPluginItemBase*> elementStyles, QWidget* parent);
    QToolButton* createAutoLayoutMenuButton();
    QToolButton* createUndoActionMenuButton();
    QToolButton* createRedoActionMenuButton();
    QToolButton* createResetSceneMenuButton();
    
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
    MyNetworkElementStyleBase* _nodeStyle;
    MyNetworkElementStyleBase* _edgeStyle;

    // copied elements
    QList<MyNetworkElementBase*> _copiedNetworkElements;

    // copied styles
    MyNetworkElementStyleBase* _copiedNodeStyle;
    MyNetworkElementStyleBase* _copiedEdgeStyle;

    QDir _applicationDirectory;
    
    QList<MyPluginItemBase*> _plugins;
    
    // undo stack
    QUndoStack* _undoStack;
    
    // elements
    QList<MyNetworkElementBase*> _nodes;
    QList<MyNetworkElementBase*> _edges;
    
    // builder
    QObject* _newEdgeBuilder;
    QGraphicsItem* _selectionAreaGraphicsItem;
    
    // network
    QRectF _networkExtents;
    QJsonObject _stageInfo;

    // file
    QObject* _fileManager;
};

QString getElementUniqueName(QList<MyNetworkElementBase*> elements, const QString& defaultIdSection);
MyNetworkElementBase* findElement(QList<MyNetworkElementBase*> elements, const QString& name);
MyNetworkElementBase* findSourceNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json);
MyNetworkElementBase* findTargetNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json);
MyNetworkElementStyleBase* getCopyNodeStyle(const QString& name, MyNetworkElementStyleBase* style);
MyNetworkElementStyleBase* getCopyEdgeStyle(const QString& name, MyNetworkElementStyleBase* edgeStyle);

#endif
