#ifndef __NEGUI_INTERACTOR_H
#define __NEGUI_INTERACTOR_H

#include "negui_network_element_base.h"
#include "negui_scene_mode_element_base.h"

#include <QObject>
#include <QDir>
#include <QUndoStack>
#include <QWidgetAction>
#include <QAbstractButton>

class MyInteractor : public QObject, public MySceneModeElementBase {
    Q_OBJECT
    
public:
    
    MyInteractor(QObject *parent = nullptr);
    
    // undo stack
    QUndoStack* undoStack();

    QObject* fileManager();
    
    // modes
    void setSceneMode(const SceneMode& sceneMode) override;

    // network
    void resetNetworkCanvas();
    void resetCanvas();
    void resetNetwork();
    void setBackground(const QJsonObject &json);
    
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

    void addElementModeIsEnabled(const QString&);
    void askForExportFigure(const QString& fileName, const QString& fileExtension);
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);
    const QRectF askForNetworkExtents();
    const QString askForNetworkBackgroundColor();
    void askForSetNetworkBackgroundColor(const QString&);
    void askForClearScene();
    void askForResetScale();
    void askForSetToolTip(const QString& toolTip);
    void askForDisplayFeatureMenu();
    void askForDisplayFeatureMenu(QWidget*);
    void askForRemoveFeatureMenu();
    QWidget* askForCurrentlyBeingDisplayedNetworkElementFeatureMenu();
    QList<QGraphicsItem *> askForItemsAtPosition(const QPointF& position);
    void modeIsSet(const QString&);
    void currentFileNameIsUpdated(const QString&);
    const bool askForWhetherShiftModifierIsPressed();
    const bool askForWhetherControlModifierIsPressed();
    QRectF askForItemsBoundingRect();
    void elementsCuttableStatusChanged(const bool&);
    void elementsCopyableStatusChanged(const bool&);
    void pasteElementsStatusChanged(const bool&);
    void askForDisplaySceneContextMenu(const QPointF&);
    QString askForWorkingDirectoryPath();
    void askForSettingWorkingDirectoryPath(const QString&);
    void askForAdjustConnectedEdgesOfNodes();
    void askForAdjustExtentsOfNodes();
    void singleNetworkElementFeatureMenuIsDisplayed(const QString&);
    void multiNetworkElementFeatureMenuIsDisplayed(const QString&);

    void enterKeyIsPressed();
    
public slots:
    
    // menus
    QList<QAbstractButton*> getToolbarMenuButtons();
    QList<QAbstractButton*> getModeMenuButtons();
    
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
    const QList<MyNetworkElementBase*> getSelectedElements();
    const QList<MyNetworkElementBase*> getSelectedNodes();
    const QList<MyNetworkElementBase*> getSelectedEdges();
    MyNetworkElementBase* getOneSingleSelectedElement();
    MyNetworkElementBase* getOneSingleSelectedNode();
    MyNetworkElementBase* getOneSingleSelectedEdge();
    void changeElementSelectionsStatus(MyNetworkElementBase* element);
    void setElementSelected(const QString& elementName);
    void selectElements(const bool& selected);
    void selectElementsOfCategory(const bool& selected, const QString& category);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setCopiedNode(MyNetworkElementBase* node);
    void setCutNode(MyNetworkElementBase* node);
    void setCopiedNodeStyle(MyNetworkElementStyleBase* style);
    void setCopiedEdgeStyle(MyNetworkElementStyleBase* style);
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areSelectedElementsAlignable();
    const bool areAnyElementsCopied();
    const bool areAnyElementsSelected();
    const QString applicationDirectoryPath();
    const QString iconsDirectoryPath();
    QJsonObject getNetworkElementsAndColorInfo();
    void moveSelectedNetworkElements(MyNetworkElementBase* movedNode, const QPointF& movedDistance);
    
    // modes
    void enableNormalMode() override;
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableAddEdgeMode(MyPluginItemBase* style);
    void enableSelectMode(const QString& elementCategory = "");
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableSelectEdgeMode(const QString& edgeCategory = "");

    void displayFeatureMenu();
    void displayFeatureMenu(QWidget* featureMenu);

    void displaySelectionArea(const QPointF& position);
    void clearSelectionArea();

    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const QPointF& position);

    // plugins
    QList<MyPluginItemBase*>& pluginItems();
    const QStringList listOfPluginItemNames(const QString type);
    const QStringList listOfPluginItemCategories(const QString type);
    void addPluginItem(MyPluginItemBase* pluginItem);
    
private slots:

    void readFromFile(const QString& importToolName);
    void readFromFile(MyPluginItemBase* importToo);
    void writeDataToFile(const QString& exportToolName);
    void writeDataToFile(MyPluginItemBase* exportTool);
    void writeFigureToFile(const QString& exportToolName);
    void writeFigureToFile(MyPluginItemBase* exportTool);
    void saveCurrentNetwork();
    void autoLayout(MyPluginItemBase* autoLayoutEngine);
    void createChangeStageCommand();
    void createSelectionAreaGraphicsItem(const QPointF& position);
    void selectSelectionAreaCoveredNodes();
    void selectSelectionAreaCoveredEdges();
    const bool isSetCopiedNodeStyle();
    void pasteCopiedNodeStyle(MyNetworkElementBase* element);
    const bool isSetCopiedEdgeStyle();
    void pasteCopiedEdgeStyle(MyNetworkElementBase* element);
    void copySelectedNetworkElements();
    void cutSelectedNetworkElements();
    const bool isElementNameAlreadyUsed(const QString& elementName);
    
protected:

    // plugins
    void setPluginManager();
    void loadPlugins();

    // file manager
    void setFileManager();

    // network element selector
    void setNetworkElementSelector();
    
    // menu buttons
    void addDefaultNodeStyle();
    void addDefaultEdgeStyle();
    
    // element styles
    MyNetworkElementStyleBase* _nodeStyle;
    MyNetworkElementStyleBase* _edgeStyle;

    // copied elements
    QList<MyNetworkElementBase*> _copiedNetworkElements;

    // copied styles
    MyNetworkElementStyleBase* _copiedNodeStyle;
    MyNetworkElementStyleBase* _copiedEdgeStyle;

    QDir _applicationDirectory;
    
    // undo stack
    QUndoStack* _undoStack;
    
    // elements
    QList<MyNetworkElementBase*> _nodes;
    QList<MyNetworkElementBase*> _edges;
    
    // builder
    QObject* _newEdgeBuilder;
    QGraphicsItem* _selectionAreaGraphicsItem;
    
    // network
    QJsonObject _stageInfo;

    // file
    QObject* _pluginManager;

    // file
    QObject* _fileManager;

    // select
    QObject* _networkElementSelector;
};

#endif
