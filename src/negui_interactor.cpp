#include "negui_interactor.h"
#include "negui_network_manager.h"
#include "negui_file_manager.h"
#include "negui_plugin_manager.h"
#include "negui_menu_button_manager.h"
#include "negui_default_network_element_style_manager.h"
#include "negui_customized_interactor_widgets.h"

#include <QCoreApplication>

// MyInteractor

MyInteractor::MyInteractor(QObject *parent) : QObject(parent) {
    setUndoStack();
    setPluginManager();
    loadPlugins();
    setNetworkManager();
    setFileManager();
    setMenuButtonManager();
    initializeStageInfo();
};

void MyInteractor::setUndoStack() {
    _undoStack = new MyUndoStack(this);
    connect(_undoStack, SIGNAL(canUndoChanged(const bool&)), this, SIGNAL(canUndoChanged(const bool&)));
    connect(_undoStack, SIGNAL(canRedoChanged(const bool&)), this, SIGNAL(canRedoChanged(const bool&)));
}

QUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

QDir MyInteractor::applicationDirectory() {
    return QDir(QCoreApplication::applicationDirPath());
}

const QString MyInteractor::applicationDirectoryPath() {
    return applicationDirectory().path();
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
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForApplicationDirectoryPath, this, [this] () { return applicationDirectoryPath(); } );
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForWorkingDirectoryPath, this, [this] () { return ((MyFileManager*)fileManager())->workingDirectoryPath(); });
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForCurrentBaseFileName, this, [this] () { return ((MyFileManager*)fileManager())->currentBaseFileName(); });
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::askForNetworkInfo, this, [this] () { return exportNetworkInfo(); });
    connect((MyPluginManager*)_pluginManager, &MyPluginManager::networkInfoIsWrittenToFile, this, [this] (MyPluginItemBase* exportTool, const QString& fileName) {
        ((MyFileManager*)fileManager())->setCurrentExportTool(exportTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
        ((MyFileManager*)fileManager())->setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");
    } );
    connect(_pluginManager, SIGNAL(askForExportFigure(const QString&, const QString&)), this, SIGNAL(askForExportFigure(const QString&, const QString&)));
    connect(_pluginManager, SIGNAL(askForTriggerAPIAction(const QString&, const QJsonValue&)), this, SLOT(triggerAPIAction(const QString&, const QJsonValue&)));
}

void MyInteractor::loadPlugins() {
    ((MyPluginManager*)_pluginManager)->load();
}

QList<MyPluginItemBase*>& MyInteractor::pluginItems() {
    return ((MyPluginManager*)_pluginManager)->pluginItems();
}

QStringList MyInteractor::listOfPluginItemNames(const QString& type) {
    return ((MyPluginManager*)_pluginManager)->listOfPluginItemNames(type);
}

QStringList MyInteractor::listOfPluginItemCategories(const QString& type) {
    return ((MyPluginManager*)_pluginManager)->listOfPluginItemCategories(type);
}

void MyInteractor::addPluginItem(MyPluginItemBase* pluginItem) {
    if (pluginItem)
        ((MyPluginManager*)_pluginManager)->addPluginItem(pluginItem);
}

void MyInteractor::setNetworkManager() {
    _networkManager = new MyNetworkManager();
    connect(_networkManager, SIGNAL(askForClearScene()), this, SIGNAL(askForClearScene()));
    connect(_networkManager, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
    connect(_networkManager, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
    connect(_networkManager, SIGNAL(elementsCuttableStatusChanged(const bool&)), this, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(elementsCopyableStatusChanged(const bool&)), this, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(askForWhetherShiftModifierIsPressed()), this, SIGNAL(askForWhetherShiftModifierIsPressed()));
    connect(_networkManager, SIGNAL(askForWhetherControlModifierIsPressed()), this, SIGNAL(askForWhetherControlModifierIsPressed()));
    connect(_networkManager, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), this, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)));
    connect(_networkManager, SIGNAL(pasteElementsStatusChanged(const bool&)), this, SIGNAL(pasteElementsStatusChanged(const bool&)));
    connect(_networkManager, SIGNAL(askForWhetherFeatureMenuCanBeDisplayed()), this, SIGNAL(askForWhetherFeatureMenuCanBeDisplayed()));
    connect(_networkManager, SIGNAL(askForEnableFeatureMenuDisplay()), this, SIGNAL(askForEnableFeatureMenuDisplay()));
    connect(_networkManager, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()));
    connect(_networkManager, SIGNAL(askForDisplayNullFeatureMenu()), this, SIGNAL(askForDisplayNullFeatureMenu()));
    connect(_networkManager, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
    connect(_networkManager, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
    connect(_networkManager, SIGNAL(askForSetToolTip(const QString&)), this, SIGNAL(askForSetToolTip(const QString&)));
    connect(_networkManager, SIGNAL(askForSetNetworkBackgroundColor(const QString&)), this, SIGNAL(askForSetNetworkBackgroundColor(const QString&)));
    connect(_networkManager, SIGNAL(askForNetworkBackgroundColor()), this, SIGNAL(askForNetworkBackgroundColor()));
    connect(_networkManager, SIGNAL(askForItemsBoundingRect()), this, SIGNAL(askForItemsBoundingRect()));
    connect(_networkManager, SIGNAL(askForResetScale()), this, SIGNAL(askForResetScale()));
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForCreateChangeStageCommand, this, [this] () { createChangeStageCommand(); });
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForEnableNormalMode, this, [this] () { enableNormalMode(); });
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForClearUndoStack, this, [this] () { undoStack()->clear(); });
    connect((MyNetworkManager*)_networkManager, &MyNetworkManager::askForIconsDirectoryPath, this, [this] () { return iconsDirectoryPath(); });
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

void MyInteractor::setMenuButtonManager() {
    _menuButtonManager = new MyMenuButtonManager();
}

QObject* MyInteractor::menuButtonManager() {
    return _menuButtonManager;
}

void MyInteractor::initializeStageInfo() {
    _stageInfo = getNetworkElementsAndColorInfo();
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

void MyInteractor::enableAddNodeMode(const QString& nodeStyleName) {
    MyPluginItemBase* nodeStyle = findPluginByName(getPluginsOfType(pluginItems(), "nodeStyle"), nodeStyleName);
    if (nodeStyle)
        enableAddNodeMode(nodeStyle);
}

void MyInteractor::enableAddNodeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddNodeMode();
    askForRemoveFeatureMenu();
    ((MyNetworkManager*)_networkManager)->enableAddNodeMode(style);

    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
    emit addElementModeIsEnabled(style->name());
}

void MyInteractor::enableAddEdgeMode(const QString& edgeStyleName) {
    MyPluginItemBase* edgeStyle = findPluginByName(getPluginsOfType(pluginItems(), "edgeStyle"), edgeStyleName);
    if (edgeStyle)
        enableAddEdgeMode(edgeStyle);
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

void MyInteractor::createNetwork(const QJsonObject& json) {
    ((MyNetworkManager*)_networkManager)->createNetwork(json);
}

void MyInteractor::setNewNetworkCanvas() {
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

void MyInteractor::addNode(const QPointF& position) {
    ((MyNetworkManager*)_networkManager)->addNode(position);
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

void MyInteractor::updateFeatureMenu() {
    ((MyNetworkManager*)_networkManager)->updateFeatureMenu();
}

void MyInteractor::displaySelectionArea(const QPointF& position) {
    ((MyNetworkManager*)_networkManager)->displaySelectionArea(position);
}

void MyInteractor::clearSelectionArea() {
    ((MyNetworkManager*)_networkManager)->clearSelectionArea();
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
    MyPluginItemBase* savePlugin = ((MyFileManager*)fileManager())->getSavePlugin(pluginItems());
    if (savePlugin && ((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        defaultPluginAction(savePlugin);
}

QList<QAbstractButton*> MyInteractor::getToolbarMenuButtons() {
    return ((MyMenuButtonManager*)_menuButtonManager)->getToolbarMenuButtons(this, undoStack(), pluginItems(), iconsDirectoryPath());
}

QList<QAbstractButton*> MyInteractor::getModeMenuButtons() {
    addDefaultNetworkElementStyles();
    return ((MyMenuButtonManager*)_menuButtonManager)->getModeMenuButtons(this, pluginItems(), iconsDirectoryPath());
}

void MyInteractor::addDefaultNetworkElementStyles() {
    MyDefaultNetworkElementStyleManager* defaultNetworkElementStyleManager = new MyDefaultNetworkElementStyleManager();
    addPluginItem(defaultNetworkElementStyleManager->createDefaultNodeStyle(pluginItems()));
    addPluginItem(defaultNetworkElementStyleManager->createDefaultEdgeStyle(pluginItems()));
    defaultNetworkElementStyleManager->deleteLater();
}

void MyInteractor::createChangeStageCommand() {
    QJsonObject currentStageInfo = getNetworkElementsAndColorInfo();
    if (undoStack()->count() > undoStack()->index()) {
        const QUndoCommand* indexCommand = undoStack()->command(undoStack()->index());
        _stageInfo = ((MyChafngeStageCommand*)indexCommand)->previousStageInfo();
    }
    if (currentStageInfo != _stageInfo) {
        ((MyFileManager*)fileManager())->setCurrentNetworkUnsaved(true);
        MyChangeStageCommand* changeStageCommand = new MyChangeStageCommand(_stageInfo, currentStageInfo);
        ((MyUndoStack*)undoStack())->addCommand(changeStageCommand);
        connect((MyChangeStageCommand*)changeStageCommand, &MyChangeStageCommand::askForCreateNetwork, this, [this] (const QJsonObject& json) { createNetwork(json); });
        _stageInfo = currentStageInfo;
    }
}

void MyInteractor::triggerUndoAction() {
    undoStack()->undo();
}

void MyInteractor::triggerRedoAction() {
    undoStack()->redo();
}

void MyInteractor::selectAllElements() {
    selectElements(true);
}

void MyInteractor::selectAllElements(const QString& category) {
    selectElementsOfCategory(true, category);
}

const QJsonValue MyInteractor::takeParameterFromUser(const QString& name, const QJsonValue defaultValue) {
    MyTakeParameterDialog* takeParameterDialog = new MyTakeParameterDialog(name, defaultValue);
    return takeParameterDialog->execute();
}

const QString MyInteractor::getOpenFileName(const QString& fileExtension) {
    return ((MyFileManager*)fileManager())->getOpenFileName(fileExtension);
}

const QString MyInteractor::getSaveFileName(const QString& defaultFileExtension) {
    return ((MyFileManager*)fileManager())->getSaveFileName(defaultFileExtension);
}

const QString MyInteractor::getSaveAsFileName(const QString& fileExtension) {
    return ((MyFileManager*)fileManager())->getSaveAsFileName(fileExtension);
}

void MyInteractor::defaultPluginAction(const QString& defaultPluginItemName) {
    defaultPluginAction(findPluginByName(pluginItems(), defaultPluginItemName));
}

void MyInteractor::defaultPluginAction(MyPluginItemBase* defaultPluginItem) {
    if (defaultPluginItem)
        ((MyPluginManager*)_pluginManager)->defaultPluginAction(defaultPluginItem);
}

const QJsonValue MyInteractor::triggerAPIAction(const QString& functionName, const QJsonValue& inputs) {
    if (inputs.isArray()) {
        QJsonArray inputArray = inputs.toArray();
        if (functionName == "selectElements") {
            if (inputArray.size() == 1 && inputArray[0].isBool())
                selectElements(inputArray[0].toBool());
        }
        else if (functionName == "deleteSelectedNetworkElements")
            deleteSelectedNetworkElements();
        else if (functionName == "exportNetworkInfo")
            return exportNetworkInfo();
        else if (functionName == "createNetwork") {
            if (inputArray.size() == 1 && inputArray[0].isObject())
                createNetwork(inputArray[0].toObject());
        }
        else if (functionName == "askForAdjustExtentsOfNodes")
            emit askForAdjustExtentsOfNodes();
        else if (functionName == "askForAdjustConnectedEdgesOfNodes")
            emit askForAdjustConnectedEdgesOfNodes();
        else if (functionName == "createChangeStageCommand")
            createChangeStageCommand();
        else if (functionName == "resetCanvas")
            resetCanvas();
        else if (functionName == "enableNormalMode")
            enableNormalMode();
        else if (functionName == "takeParameterFromUser") {
            if (inputArray.size() == 2 && inputArray[0].isString())
                return takeParameterFromUser(inputArray[0].toString(), inputArray[1]);
        }
        else if (functionName == "saveCurrentNetwork")
            saveCurrentNetwork();
        else if (functionName == "getOpenFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return getOpenFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveAsFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return getSaveAsFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return getSaveFileName(inputArray[0].toString());
        }
    }

    return QJsonObject();
}
