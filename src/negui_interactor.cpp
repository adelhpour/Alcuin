#include "negui_interactor.h"
#include "negui_network_manager.h"
#include "negui_file_manager.h"
#include "negui_plugin_manager.h"
#include "negui_customized_interactor_widgets.h"
#include "negui_menu_button_builder.h"
#include "negui_multi_network_element_feature_menu.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QJsonArray>
#include <QVBoxLayout>

// MyInteractor

MyInteractor::MyInteractor(QObject *parent) : QObject(parent) {
    setUndoStack();
    setApplicationDirectory();
    setPluginManager();
    loadPlugins();
    setNetworkManager();
    setFileManager();
    initializeStageInfo();
};

void MyInteractor::setUndoStack() {
    _undoStack = new MyUndoStack(this);
}

QUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

void MyInteractor::setApplicationDirectory() {
    _applicationDirectory = QDir(QCoreApplication::applicationDirPath());
}

QDir MyInteractor::applicationDirectory() {
    return _applicationDirectory;
}

const QString MyInteractor::applicationDirectoryPath() {
    return _applicationDirectory.path();
}

QDir MyInteractor::iconsDirectory() {
#if defined(Q_OS_MAC)
    return QDir(applicationDirectoryPath() + "/../Resources/icons");
#else
    return QDir(applicationDirectoryPath() + "/resources/icons");
#endif
}

const QString MyInteractor::iconsDirectoryPath() {
    return iconsDirectory().path();
}

void MyInteractor::setPluginManager() {
    _pluginManager = new MyPluginManager();
    connect(_pluginManager, SIGNAL(askForApplicationDirectoryPath()), this, SLOT(applicationDirectoryPath()));
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForWorkingDirectoryPath, this, [this] () { return ((MyFileManager*)fileManager())->workingDirectoryPath(); });
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForCurrentBaseFileName, this, [this] () { return ((MyFileManager*)fileManager())->currentBaseFileName(); });
    connect(_pluginManager, SIGNAL(askForNetworkInfo()), this, SLOT(exportNetworkInfo()));
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::networkInfoIsReadFromFile, this, [this] (const QJsonObject &json, MyPluginItemBase* importTool, const QString& fileName) {
        createNetwork(json);
        createChangeStageCommand();
        resetCanvas();
        ((MyFileManager*)fileManager())->setCurrentExportToolCompatibleWithImportTool(importTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
        ((MyFileManager*)fileManager())->setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");
    } );
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::networkInfoIsWrittenToFile, this, [this] (MyPluginItemBase* exportTool, const QString& fileName) {
        ((MyFileManager*)fileManager())->setCurrentExportTool(exportTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
        ((MyFileManager*)fileManager())->setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");
    } );
    connect(_pluginManager, SIGNAL(askForExportFigure(const QString&, const QString&)), this, SIGNAL(askForExportFigure(const QString&, const QString&)));
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::auotLayoutAlgorithmIsApplied, this, [this] (const QJsonObject &json) {
        createNetwork(json);
        emit askForAdjustExtentsOfNodes();
        emit askForAdjustConnectedEdgesOfNodes();
        createChangeStageCommand();
        enableNormalMode();
    } );
}

void MyInteractor::loadPlugins() {
    ((MyPluginManager*)_pluginManager)->load();
}

QList<MyPluginItemBase*>& MyInteractor::pluginItems() {
    return ((MyPluginManager*)_pluginManager)->pluginItems();
}

const QStringList MyInteractor::listOfPluginItemNames(const QString type) {
    return ((MyPluginManager*)_pluginManager)->listOfPluginItemNames(type);
}

const QStringList MyInteractor::listOfPluginItemCategories(const QString type) {
    return ((MyPluginManager*)_pluginManager)->listOfPluginItemCategories(type);
}

void MyInteractor::addPluginItem(MyPluginItemBase* pluginItem) {
    ((MyPluginManager*)_pluginManager)->addPluginItem(pluginItem);
}

void MyInteractor::setNetworkManager() {
    _networkManager = new MyNetworkManager();
    connect(_networkManager, SIGNAL(askForClearScene()), this, SIGNAL(askForClearScene()));
    connect(_networkManager, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
    connect(_networkManager, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
    connect(_networkManager, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
    connect(_networkManager, SIGNAL(elementsCuttableStatusChanged(const bool&)), this, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(elementsCopyableStatusChanged(const bool&)), this, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(askForWhetherShiftModifierIsPressed()), this, SIGNAL(askForWhetherShiftModifierIsPressed()));
    connect(_networkManager, SIGNAL(askForWhetherControlModifierIsPressed()), this, SIGNAL(askForWhetherControlModifierIsPressed()));
    connect(_networkManager, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), this, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)));
    connect(_networkManager, SIGNAL(askForIconsDirectoryPath()), this, SLOT(iconsDirectoryPath()));
    connect(_networkManager, SIGNAL(pasteElementsStatusChanged(const bool&)), this, SIGNAL(pasteElementsStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()));
    connect(_networkManager, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));
    connect(_networkManager, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
    connect(_networkManager, SIGNAL(askForSetToolTip(const QString&)), this, SIGNAL(askForSetToolTip(const QString&)));
    connect(_networkManager, SIGNAL(askForSetNetworkBackgroundColor(const QString&)), this, SIGNAL(askForSetNetworkBackgroundColor(const QString&)));
    connect(_networkManager, SIGNAL(askForNetworkBackgroundColor()), this, SIGNAL(askForNetworkBackgroundColor()));
    connect(_networkManager, SIGNAL(askForItemsBoundingRect()), this, SIGNAL(askForItemsBoundingRect()));
    connect(_networkManager, SIGNAL(askForResetScale()), this, SIGNAL(askForResetScale()));
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForEnableNormalMode, this, [this] () { enableNormalMode(); });
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForClearUndoStack, this, [this] () { undoStack()->clear(); });
    connect(this, SIGNAL(singleNetworkElementFeatureMenuIsDisplayed(const QString&)), _networkManager, SIGNAL(singleNetworkElementFeatureMenuIsDisplayed(const QString&)));
    connect(this, SIGNAL(multiNetworkElementFeatureMenuIsDisplayed(const QString&)), _networkManager, SIGNAL(multiNetworkElementFeatureMenuIsDisplayed(const QString&)));
    connect(this, SIGNAL(askForAdjustExtentsOfNodes()), _networkManager, SIGNAL(askForAdjustExtentsOfNodes()));
    connect(this, SIGNAL(askForAdjustConnectedEdgesOfNodes()), _networkManager, SIGNAL(askForAdjustConnectedEdgesOfNodes()));
}

QObject* MyInteractor::networkManager() {
    return _networkManager;
}

void MyInteractor::setFileManager() {
    _fileManager = new MyFileManager(getPluginsOfType(pluginItems(), "importtool"), getPluginsOfType(pluginItems(), "dataexporttool"));
    connect(_fileManager, SIGNAL(currentFileNameIsUpdated(const QString&)), this, SIGNAL(currentFileNameIsUpdated(const QString&)));
    connect(this, &MyInteractor::askForWorkingDirectoryPath, this, [this] () { return ((MyFileManager*)fileManager())->workingDirectoryPath(); });
    connect(this, &MyInteractor::askForSettingWorkingDirectoryPath, this, [this] (const QString& workingDirectoryPath) { ((MyFileManager*)fileManager())->setWorkingDirectoryPath(QFileInfo(workingDirectoryPath).absolutePath() + "/"); });
}

QObject* MyInteractor::fileManager() {
    return _fileManager;
}

void MyInteractor::initializeStageInfo() {
    _stageInfo = getNetworkElementsAndColorInfo();
}

void MyInteractor::createNetwork(const QJsonObject& json) {
    ((MyNetworkManager*)_networkManager)->createNetwork(json);
}

void MyInteractor::setNewNetworkCanvas() {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    askForRemoveFeatureMenu();
    ((MyNetworkManager*)_networkManager)->resetNetworkCanvas();
    ((MyFileManager*)fileManager())->reset();
}

void MyInteractor::resetNetworkCanvas() {
    ((MyNetworkManager*)_networkManager)->resetNetworkCanvas();
}

void MyInteractor::resetNetwork() {
    ((MyNetworkManager*)_networkManager)->resetNetwork();
}

void MyInteractor::resetCanvas() {
    ((MyNetworkManager*)_networkManager)->resetCanvas();
}

void MyInteractor::setBackground(const QJsonObject &json) {
    ((MyNetworkManager*)_networkManager)->setBackground(json);
}

void MyInteractor::addNodes(const QJsonObject &json) {
    ((MyNetworkManager*)_networkManager)->addNodes(json);
}

void MyInteractor::addNode(const QJsonObject &json) {
    ((MyNetworkManager*)_networkManager)->addNode(json);
}

void MyInteractor::addNewNode(const QPointF& position) {
    ((MyNetworkManager*)_networkManager)->addNewNode(position);
}

void MyInteractor::clearNodesInfo() {
    ((MyNetworkManager*)_networkManager)->clearNodesInfo();
}

void MyInteractor::addEdges(const QJsonObject &json) {
    ((MyNetworkManager*)_networkManager)->addEdges(json);
}

void MyInteractor::addEdge(const QJsonObject &json) {
    ((MyNetworkManager*)_networkManager)->addEdge(json);
}

void MyInteractor::clearEdgesInfo() {
    ((MyNetworkManager*)_networkManager)->clearEdgesInfo();
}

const bool MyInteractor::areSelectedElementsCopyable() {
    return ((MyNetworkManager*)_networkManager)->areSelectedElementsCopyable();
}

const bool MyInteractor::areSelectedElementsCuttable() {
    return ((MyNetworkManager*)_networkManager)->areSelectedElementsCuttable();
}

const bool MyInteractor::areSelectedElementsAlignable() {
    return ((MyNetworkManager*)_networkManager)->areSelectedElementsAlignable();
}

const bool MyInteractor::areAnyElementsCopied() {
    return ((MyNetworkManager*)_networkManager)->areAnyElementsCopied();
}

const bool MyInteractor::areAnyElementsSelected() {
    return ((MyNetworkManager*)_networkManager)->areAnyElementsSelected();
}

void MyInteractor::copySelectedNetworkElements() {
    ((MyNetworkManager*)_networkManager)->copySelectedNetworkElements();
}

void MyInteractor::cutSelectedNetworkElements() {
    ((MyNetworkManager*)_networkManager)->cutSelectedNetworkElements();
}

void MyInteractor::pasteCopiedNetworkElements() {
    ((MyNetworkManager*)_networkManager)->pasteCopiedNetworkElements();
}

void MyInteractor::pasteCopiedNetworkElements(const QPointF& position) {
    ((MyNetworkManager*)_networkManager)->pasteCopiedNetworkElements(position);
}

void MyInteractor::resetCopiedNetworkElements() {
    ((MyNetworkManager*)_networkManager)->resetCopiedNetworkElements();
}

QJsonObject MyInteractor::getNetworkElementsAndColorInfo() {
    return ((MyNetworkManager*)_networkManager)->getNetworkElementsAndColorInfo();
}

QJsonObject MyInteractor::exportNetworkInfo() {
    return ((MyNetworkManager*)_networkManager)->exportNetworkInfo();
}

void MyInteractor::selectElements(const bool& selected) {
    ((MyNetworkManager*)_networkManager)->selectElements(selected);
}

void MyInteractor::selectElementsOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkManager*)_networkManager)->selectElementsOfCategory(selected, category);
}

void MyInteractor::selectNodes(const bool& selected) {
    ((MyNetworkManager*)_networkManager)->selectNodes(selected);
}

void MyInteractor::selectNodesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkManager*)_networkManager)->selectNodesOfCategory(selected, category);
}

void MyInteractor::selectEdges(const bool& selected) {
    ((MyNetworkManager*)_networkManager)->selectEdges(selected);
}

void MyInteractor::selectEdgesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkManager*)_networkManager)->selectEdgesOfCategory(selected, category);
}

void MyInteractor::setElementSelected(const QString& elementName) {
    ((MyNetworkManager*)_networkManager)->setElementSelected(elementName);
}

void MyInteractor::deleteSelectedNetworkElements() {
    ((MyNetworkManager*)_networkManager)->deleteSelectedNetworkElements();
}

void MyInteractor::alignSelectedNetworkElements(const QString& alignType) {
    ((MyNetworkManager*)_networkManager)->alignSelectedNetworkElements(alignType);
}

const QList<MyNetworkElementBase*> MyInteractor::getSelectedElements() {
    return ((MyNetworkManager*)_networkManager)->getSelectedElements();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedElement() {
    return ((MyNetworkManager*)_networkManager)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedNode() {
    return ((MyNetworkManager*)_networkManager)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedEdge() {
    return ((MyNetworkManager*)_networkManager)->getOneSingleSelectedElement();
}

void MyInteractor::setSceneMode(const SceneMode& sceneMode) {
    MySceneModeElementBase::setSceneMode(sceneMode);
    emit modeIsSet(getSceneModeAsString());
}

void MyInteractor::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    ((MyNetworkManager*)_networkManager)->enableNormalMode();

    emit askForSetToolTip("");
}

void MyInteractor::enableAddNodeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddNodeMode();
    askForRemoveFeatureMenu();
    ((MyNetworkManager*)_networkManager)->enableAddNodeMode(style);
    
    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
    emit addElementModeIsEnabled(style->name());
}

void MyInteractor::enableAddEdgeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddEdgeMode();
    askForRemoveFeatureMenu();
    ((MyNetworkManager*)_networkManager)->enableAddEdgeMode(style);

    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
    emit addElementModeIsEnabled(style->name());
}

void MyInteractor::enableSelectMode(const QString& elementCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectMode();
    ((MyNetworkManager*)_networkManager)->enableSelectMode();

    emit askForSetToolTip("Select" + elementCategory);
}

void MyInteractor::enableSelectNodeMode(const QString& nodeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectNodeMode();
    ((MyNetworkManager*)_networkManager)->enableSelectNodeMode();
    
    emit askForSetToolTip("Select " + nodeCategory + " nodes");
}

void MyInteractor::enableSelectEdgeMode(const QString& edgeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectEdgeMode();
    ((MyNetworkManager*)_networkManager)->enableSelectEdgeMode();
    
    emit askForSetToolTip("Select " + edgeCategory + " edges");
}

void MyInteractor::displayFeatureMenu() {
    if (askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()) {
        if (getOneSingleSelectedNode())
            getOneSingleSelectedNode()->createFeatureMenu();
        else if (getOneSingleSelectedEdge())
            getOneSingleSelectedEdge()->createFeatureMenu();
        else if (getSelectedElements().size()) {
            QWidget* multiNetworkElementFeatureMenu = new MyMultiNetworkElementFeatureMenu(getSelectedElements(), iconsDirectoryPath());
            connect(multiNetworkElementFeatureMenu, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
            askForDisplayFeatureMenu(multiNetworkElementFeatureMenu);
        }
        else
            askForDisplayFeatureMenu();
    }
}

void MyInteractor::displayFeatureMenu(QWidget* featureMenu) {
    QString elementName = featureMenu->objectName();
    if (((MyNetworkManager*)_networkManager)->canDisplaySingleElementFeatureMenu()) {
        emit askForDisplayFeatureMenu(featureMenu);
        emit singleNetworkElementFeatureMenuIsDisplayed(elementName);
    }
    else {
        featureMenu->deleteLater();
        QWidget* multiNetworkElementFeatureMenu = new MyMultiNetworkElementFeatureMenu(getSelectedElements(), iconsDirectoryPath());
        connect(multiNetworkElementFeatureMenu, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        emit askForDisplayFeatureMenu(multiNetworkElementFeatureMenu);
        emit multiNetworkElementFeatureMenuIsDisplayed(elementName);
    }
}

void MyInteractor::displaySelectionArea(const QPointF& position) {
    ((MyNetworkManager*)_networkManager)->displaySelectionArea(position);
}

void MyInteractor::clearSelectionArea() {
    ((MyNetworkManager*)_networkManager)->clearSelectionArea();
    displayFeatureMenu();
}

void MyInteractor::readFromFile(const QString& importToolName) {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    ((MyPluginManager*)_pluginManager)->readFromFile(importToolName);
}

void MyInteractor::readFromFile(MyPluginItemBase* importTool) {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    ((MyPluginManager*)_pluginManager)->readFromFile(importTool);
}

void MyInteractor::writeDataToFile(const QString& exportToolName) {
    ((MyPluginManager*)_pluginManager)->writeDataToFile(exportToolName);
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    ((MyPluginManager*)_pluginManager)->writeDataToFile(exportTool);
}

void MyInteractor::writeFigureToFile(const QString& exportToolName) {
    ((MyPluginManager*)_pluginManager)->writeFigureToFile(exportToolName);
}

void MyInteractor::writeFigureToFile(MyPluginItemBase* exportTool) {
    ((MyPluginManager*)_pluginManager)->writeFigureToFile(exportTool);
}

void MyInteractor::saveCurrentNetwork() {
    if (((MyFileManager*)fileManager())->lastSavedFileName().isEmpty())
        ((MyPluginManager*)_pluginManager)->writeDataToFile(((MyFileManager*)fileManager())->currentExportTool());
    else
        ((MyPluginManager*)_pluginManager)->writeDataToFile(((MyFileManager*)fileManager())->currentExportTool(), ((MyFileManager*)fileManager())->currentFileName());
}

void MyInteractor::autoLayout(MyPluginItemBase* autoLayoutEngine) {
    ((MyPluginManager*)_pluginManager)->autoLayout(autoLayoutEngine);
}

QList<QAbstractButton*> MyInteractor::getToolbarMenuButtons() {
    return createToolbarMenuButtons(this, undoStack(), pluginItems(), iconsDirectoryPath());
}

QList<QAbstractButton*> MyInteractor::getModeMenuButtons() {
    addDefaultNodeStyle();
    addDefaultEdgeStyle();
    return createModeMenuButtons(this, pluginItems(), iconsDirectoryPath());
}

void MyInteractor::addDefaultNodeStyle() {
    if (!getPluginsOfType(pluginItems(), "nodestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        addPluginItem(createNodeStyle(styleObject));
    }
}

void MyInteractor::addDefaultEdgeStyle() {
    if (!getPluginsOfType(pluginItems(), "edgestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        addPluginItem(createEdgeStyle(styleObject));
    }
}

void MyInteractor::createChangeStageCommand() {
    QJsonObject currentStageInfo = getNetworkElementsAndColorInfo();
    if (undoStack()->count() > undoStack()->index()) {
        const QUndoCommand* indexCommand = undoStack()->command(undoStack()->index());
        _stageInfo = ((MyChangeStageCommand*)indexCommand)->previousStageInfo();
    }
    if (currentStageInfo != _stageInfo) {
        ((MyFileManager*)fileManager())->setCurrentNetworkUnsaved(true);
        MyChangeStageCommand* changeStageCommand = new MyChangeStageCommand(_stageInfo, currentStageInfo);
        ((MyUndoStack*)undoStack())->addCommand(changeStageCommand);
        connect(changeStageCommand, SIGNAL(askForCreateNetwork(const QJsonObject&)), this, SLOT(createNetwork(const QJsonObject&)));
        _stageInfo = currentStageInfo;
    }
}
