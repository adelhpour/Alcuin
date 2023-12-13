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

    void canUndoChanged(const bool&);
    void canRedoChanged(const bool&);
    void addElementModeIsEnabled(const QString&);
    void askForSaveFigure(const QString& fileName);
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);
    const QString askForNetworkBackgroundColor();
    void askForSetNetworkBackgroundColor(const QString&);
    void askForClearScene();
    void askForResetScale();
    void askForSetToolTip(const QString& toolTip);
    void askForDisplayNullFeatureMenu();
    void askForDisplayFeatureMenu(QWidget*);
    void askForRemoveFeatureMenu();
    void askForEnableFeatureMenuDisplay();
    QWidget* askForCurrentlyBeingDisplayedFeatureMenu();
    bool askForWhetherFeatureMenuCanBeDisplayed();
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

public slots:

    void setSceneMode(const SceneMode& sceneMode) override;
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableAddEdgeMode(MyPluginItemBase* style);
    void enableNormalMode() override;
    void enableAddNodeMode(const QString& nodeStyleName);
    void enableAddEdgeMode(const QString& edgeStyleName);
    void enableSelectMode(const QString& elementCategory = "");
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableSelectEdgeMode(const QString& edgeCategory = "");
    void createNetwork(const QJsonObject &json);
    void resetNetworkCanvas();
    void resetNetwork();
    void resetCanvas();
    void setBackground(const QJsonObject &json);
    void setNewNetworkCanvas();
    QStringList listOfPluginItemNames(const QString& type);
    QStringList listOfPluginItemCategories(const QString& type);
    void saveCurrentNetwork();
    void saveCurrentNetworkWithUserPermission();
    void saveFigure(const QString& fileName);
    void triggerUndoAction();
    void triggerRedoAction();
    void cutSelectedNetworkElements();
    void copySelectedNetworkElements();
    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const QPointF& position);
    void resetCopiedNetworkElements();
    void selectAllElements();
    void selectAllElements(const QString&);
    void selectElements(const bool& selected);
    void selectElementsOfCategory(const bool& selected, const QString& category);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setElementSelected(const QString& elementName);
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areSelectedElementsAlignable();
    const bool areAnyElementsCopied();
    const bool areAnyElementsSelected();
    void deleteSelectedNetworkElements();
    void alignSelectedNetworkElements(const QString& alignType);
    void updateFeatureMenu();
    void displaySelectionArea(const QPointF& position);
    void clearSelectionArea();
    void addNodes(const QJsonObject &json);
    void addNode(const QJsonObject& json);
    void addNode(const QPointF& position);
    void clearNodesInfo();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void clearEdgesInfo();
    QJsonObject exportNetworkInfo();
    void addDefaultNetworkElementStyles();
    void createChangeStageCommand();
    const QString applicationDirectoryPath();
    const QString iconsDirectoryPath();
    const QJsonValue takeParameterFromUser(const QString& name, const QJsonValue defaultValue);
    const QString getOpenFileName(const QString& fileExtension);
    const QString getSaveFileName(const QString& defaultFileExtension);
    const QString getSaveAsFileName(const QString& fileExtension);
    const QString getSaveFigureFileName(const QString& fileExtension);
    void callPluginFunctions(const QString& pluginName);
    void callPluginFunctions(MyPluginItemBase* plugin);
    const QJsonValue callAPIFunction(const QString& functionName, const QJsonValue& inputs);

protected:

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
