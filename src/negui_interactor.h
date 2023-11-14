#ifndef __NEGUI_INTERACTOR_H
#define __NEGUI_INTERACTOR_H

#include "negui_scene_mode_element_base.h"
#include "negui_plugin_item_base.h"

#include <QObject>
#include <QDir>
#include <QUndoStack>
#include <QAbstractButton>
#include <QGraphicsItem>

class MyInteractor : public QObject, public MySceneModeElementBase {
    Q_OBJECT
    
public:
    
    MyInteractor(QObject *parent = nullptr);

    QUndoStack* undoStack();
    void setUndoStack();
    QDir applicationDirectory();
    const QString applicationDirectoryPath();
    QDir iconsDirectory();
    const QString iconsDirectoryPath();
    QList<MyPluginItemBase*>& pluginItems();
    QStringList listOfPluginItemNames(const QString& type);
    QStringList listOfPluginItemCategories(const QString& type);
    void addPluginItem(MyPluginItemBase* pluginItem);
    QObject* networkManager();
    QObject* fileManager();
    QObject* menuButtonManager();
    void setSceneMode(const SceneMode& sceneMode) override;
    void enableSelectMode(const QString& elementCategory = "");
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableSelectEdgeMode(const QString& edgeCategory = "");
    void createNetwork(const QJsonObject &json);
    void resetNetworkCanvas();
    void resetNetwork();
    void resetCanvas();
    void setBackground(const QJsonObject &json);
    void addNodes(const QJsonObject &json);
    void addNode(const QJsonObject& json);
    void addNewNode(const QPointF& position);
    void clearNodesInfo();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void clearEdgesInfo();
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areSelectedElementsAlignable();
    const bool areAnyElementsCopied();
    const bool areAnyElementsSelected();
    void copySelectedNetworkElements();
    void cutSelectedNetworkElements();
    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const QPointF& position);
    void resetCopiedNetworkElements();
    QJsonObject getNetworkElementsAndColorInfo();
    QJsonObject exportNetworkInfo();
    void selectElements(const bool& selected);
    void selectElementsOfCategory(const bool& selected, const QString& category);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setElementSelected(const QString& elementName);
    void deleteSelectedNetworkElements();
    void alignSelectedNetworkElements(const QString& alignType);
    void updateFeatureMenu();
    void displaySelectionArea(const QPointF& position);
    void clearSelectionArea();
    void readFromFile(const QString& importToolName);
    void writeDataToFile(const QString& exportToolName);
    void writeFigureToFile(const QString& exportToolName);
    QList<QAbstractButton*> getToolbarMenuButtons();
    QList<QAbstractButton*> getModeMenuButtons();
    void addDefaultNetworkElementStyles();
    void createChangeStageCommand();

signals:

    void addElementModeIsEnabled(const QString&);
    void askForExportFigure(const QString& fileName, const QString& fileExtension);
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

    void enableNormalMode() override;
    void enableAddNodeMode(MyPluginItemBase* style);
    void enableAddEdgeMode(MyPluginItemBase* style);
    void setNewNetworkCanvas();
    void readFromFile(MyPluginItemBase* importToo);
    void saveCurrentNetwork();
    void writeDataToFile(MyPluginItemBase* exportTool);
    void writeFigureToFile(MyPluginItemBase* exportTool);
    void autoLayout(MyPluginItemBase* autoLayoutEngine);

    
protected:

    void setPluginManager();
    void loadPlugins();
    void setNetworkManager();
    void setFileManager();
    void setMenuButtonManager();
    void initializeStageInfo();

    QUndoStack* _undoStack;
    QJsonObject _stageInfo;
    QObject* _networkManager;
    QObject* _pluginManager;
    QObject* _fileManager;
    QObject* _menuButtonManager;
};

#endif
