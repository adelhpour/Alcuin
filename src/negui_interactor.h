#ifndef __NEGUI_INTERACTOR_H
#define __NEGUI_INTERACTOR_H

#include "negui_scene_mode_element_base.h"
#include "negui_plugin_item_base.h"

#include <QObject>
#include <QDir>
#include <QUndoStack>
#include <QAbstractButton>
#include <QGraphicsItem>
#include <QJsonArray>

class MyInteractor : public QObject, public MySceneModeElementBase {
    Q_OBJECT

public:

    MyInteractor(QObject *parent = nullptr);
    QList<QAbstractButton*> getToolbarMenuButtons();
    QList<QAbstractButton*> getModeMenuButtons();

signals:

    // network
    const QString askForNetworkBackgroundColor();
    void askForSetNetworkBackgroundColor(const QString&);

    // view
    void askForClearScene();
    void askForResetScale();
    void askForSetToolTip(const QString& toolTip);
    QRectF askForItemsBoundingRect();

    // menu
    void askForDisplayNullFeatureMenu();
    void askForRemoveFeatureMenu();
    void askForEnableFeatureMenuDisplay();
    bool askForWhetherFeatureMenuCanBeDisplayed();
    void askForDisplaySceneContextMenu(const qreal&, const qreal&);

    // pressed key
    const bool askForWhetherShiftModifierIsPressed();
    const bool askForWhetherControlModifierIsPressed();

    void canUndoChanged(const bool&);
    void canRedoChanged(const bool&);
    void addElementModeIsEnabled(const QString&);
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);
    void askForDisplayFeatureMenu(QWidget*);
    QWidget* askForCurrentlyBeingDisplayedFeatureMenu();
    QList<QGraphicsItem *> askForItemsAtPosition(const QPointF& position);
    void modeIsSet(const QString&);
    void currentFileNameIsUpdated(const QString&);
    void elementsCuttableStatusChanged(const bool&);
    void elementsCopyableStatusChanged(const bool&);
    void pasteElementsStatusChanged(const bool&);
    void askForCallAPIFunction(const QString& functionName, const QJsonValue& inputs);

public slots:
    // modes
    void enableNormalMode() override;
    void enableAddNodeMode(const QString& nodeStyleName);
    void enableAddEdgeMode(const QString& edgeStyleName);

    // network
    void createNetwork(const QJsonObject &json);
    QJsonObject exportNetworkInfo();
    void setNewNetworkCanvas();
    void resetNetworkCanvas();
    void resetNetwork();
    void resetCanvas();
    void saveCurrentNetwork();
    void saveCurrentNetworkWithUserPermission();

    // network elements
    void addNode(const qreal& x, const qreal& y);
    void cutSelectedNetworkElements();
    void copySelectedNetworkElements();
    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const qreal& x, const qreal& y);
    void resetCopiedNetworkElements();
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areAnyElementsCopied();
    void selectNetworkElements(const bool& selected);
    void selectNetworkElementsOfCategory(const bool& selected, const QString&);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setNetworkElementSelected(const QString& networkElementName, const bool& selected);
    const bool areAnyElementsSelected();
    void displaySelectionArea(const qreal& x, const qreal& y);
    void clearSelectionArea();
    const bool areSelectedElementsAlignable();
    void alignSelectedNetworkElements(const QString& alignType);
    void deleteSelectedNetworkElements();
    void adjustConnectedEdgesOfNodes();
    void adjustExtentsOfNodes();

    // feature menu
    void updateFeatureMenu();

    // undo/redo
    void triggerUndoAction();
    void triggerRedoAction();
    void createChangeStageCommand();

    // directories
    const QString applicationDirectoryPath();
    const QString iconsDirectoryPath();
    const QString& workingDirectoryPath();
    void setWorkingDirectoryPath(const QString& workingDirectoryPath);

    // file names
    const QString getOpenFileName(const QString& fileExtension);
    const QString getSaveFileName(const QString& defaultFileExtension);
    const QString getSaveAsFileName(const QString& fileExtension);
    const QString getSaveFigureFileName(const QString& fileExtension);

    // parameters
    const QJsonValue takeParameterFromUser(const QString& name, const QJsonValue defaultValue);

    // plugins
    QJsonArray listOfPluginItemNames(const QString& type);
    QJsonArray listOfPluginItemCategories(const QString& type);
    void callPluginFunctions(const QString& pluginName);

protected:

    void addDefaultNetworkElementStyles();
    void setSceneMode(const SceneMode& sceneMode) override;
    void addNodes(const QJsonObject &json);
    void addNode(const QJsonObject& json);
    void clearNodesInfo();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void clearEdgesInfo();
    void callPluginFunctions(MyPluginItemBase* plugin);
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableAddEdgeMode(MyPluginItemBase* style);
    QUndoStack* undoStack();
    void setUndoStack();
    QDir applicationDirectory();
    QDir iconsDirectory();
    QList<MyPluginItemBase*>& pluginItems();
    void addPluginItem(MyPluginItemBase* pluginItem);
    QObject* networkManager();
    QObject* fileManager();
    QObject* menuButtonManager();

    void setPluginManager();
    void loadPlugins();
    void setNetworkManager();
    void setFileManager();
    void setMenuButtonManager();
    void initializeStageInfo();
    QJsonObject getNetworkElementsAndColorInfo();

    QUndoStack* _undoStack;
    QJsonObject _stageInfo;
    QObject* _networkManager;
    QObject* _pluginManager;
    QObject* _fileManager;
    QObject* _menuButtonManager;
};

#endif
