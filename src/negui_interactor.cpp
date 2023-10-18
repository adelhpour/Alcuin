#include "negui_interactor.h"
#include "negui_file_manager.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_builder.h"
#include "negui_edge_builder.h"
#include "negui_new_edge_builder.h"
#include "negui_copied_network_elements_paster.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_plugin_item_builder.h"
#include "negui_import_tools.h"
#include "negui_autolayout_engines.h"
#include "negui_export_tools.h"
#include "negui_customized_interactor_widgets.h"
#include "negui_menu_button_builder.h"
#include "negui_network_element_aligner.h"
#include "negui_network_element_aligner_builder.h"
#include "negui_network_element_mover.h"
#include "negui_network_element_selector.h"
#include "negui_multi_network_element_feature_menu.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QJsonArray>
#include <QVBoxLayout>

// MyInteractor

MyInteractor::MyInteractor(QObject *parent) : QObject(parent) {
    // import interface
    _importInterface = NULL;
    _isSetImportInterface = false;
    
    // data export interface
    _dataExportInterface = NULL;
    _isSetDataExportInterface = false;
    
    // print export interface
    _printExportInterface = NULL;
    _isSetPrintExportInterface = false;
    
    // element style interface
    _elementStyleInterface = NULL;
    _isSetElementStyleInterface = false;
    
    // autolayout interface
    _autoLayoutInterface = NULL;
    _isSetAutoLayoutInterface = false;

    // undo stack
    _undoStack = new MyUndoStack(this);
    
    // builder
    _newEdgeBuilder = NULL;

    _applicationDirectory = QDir(QCoreApplication::applicationDirPath());

    // plugins
    loadPlugins();

    // file manager
    setFileManager();

    // network element selector
    setNetworkElementSelector();

    // network
    resetNetwork();
    _stageInfo = getNetworkElementsAndColorInfo();
};

void MyInteractor::readPluginItemsInfo(const QJsonObject &json) {
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray itemsArray = json["items"].toArray();
        MyPluginItemBase* item = NULL;
        for (int itemIndex = 0; itemIndex < itemsArray.size(); ++itemIndex) {
            QJsonObject itemObject = itemsArray[itemIndex].toObject();
            item = createPluginItem(itemsArray[itemIndex].toObject());
            if (item)
                _plugins.push_back(item);
        }
    }
}

bool MyInteractor::setImportInterface(ImportInterface* importInterface, const QString &appPath, const QString &pluginsPath) {
    if (importInterface) {
        _importInterface = importInterface;
        if (!_importInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_importInterface->loadItemsInfo());
            _isSetImportInterface = true;
        }
    }
    
    return _isSetImportInterface;
}

ImportInterface* MyInteractor::importInterface() {
    return _importInterface;
}

bool MyInteractor::setDataExportInterface(DataExportInterface* dataExportInterface, const QString &appPath, const QString &pluginsPath) {
    if (dataExportInterface) {
        _dataExportInterface = dataExportInterface;
        if (!_dataExportInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_dataExportInterface->loadItemsInfo());
            _isSetDataExportInterface = true;
        }
    }
    
    return _isSetDataExportInterface;
}

DataExportInterface* MyInteractor::dataExportInterface() {
    return _dataExportInterface;
}

bool MyInteractor::setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &appPath, const QString &pluginsPath) {
    if (printExportInterface) {
        _printExportInterface = printExportInterface;
        if (!_printExportInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_printExportInterface->loadItemsInfo());
            _isSetPrintExportInterface = true;
        }
    }
    
    return _isSetPrintExportInterface;
}

PrintExportInterface* MyInteractor::printExportInterface() {
    return _printExportInterface;
}

QList<MyPluginItemBase*>& MyInteractor::plugins() {
    return _plugins;
}

const QStringList MyInteractor::listOfPluginItemNames(const QString type) {
    QStringList pluginItemNames;
    QList<MyPluginItemBase*> pluginsOfType = getPluginsOfType(plugins(), type);
    for (MyPluginItemBase* pluginOfType: pluginsOfType)
        pluginItemNames.push_back(pluginOfType->name());

    return pluginItemNames;
}

const QStringList MyInteractor::listOfPluginItemCategories(const QString type) {
    QStringList pluginItemCategories;
    QList<MyPluginItemBase*> pluginsOfType = getPluginsOfType(plugins(), type);
    for (MyPluginItemBase* pluginOfType: pluginsOfType)
        pluginItemCategories.push_back(pluginOfType->category());

    return pluginItemCategories;
}

bool MyInteractor::setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &appPath, const QString &pluginsPath) {
    if (elementStyleInterface) {
        _elementStyleInterface = elementStyleInterface;
        if (!_elementStyleInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_elementStyleInterface->loadItemsInfo());
            _isSetElementStyleInterface = true;
        }
    }
    
    return _isSetElementStyleInterface;
}

ElementStyleInterface* MyInteractor::elementStyleInterface() {
    return _elementStyleInterface;
}

bool MyInteractor::setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &appPath, const QString &pluginsPath) {
    if (autoLayoutInterface) {
        _autoLayoutInterface = autoLayoutInterface;
        if (!_autoLayoutInterface->initialize(appPath, pluginsPath)) {
            readPluginItemsInfo(_autoLayoutInterface->loadItemsInfo());
            _isSetAutoLayoutInterface = true;
        }
    }
    
    return _isSetAutoLayoutInterface;
}

AutoLayoutInterface* MyInteractor::autoLayoutInterface() {
    return _autoLayoutInterface;
}

QDir MyInteractor::applicationDirectory() {
    return _applicationDirectory;
}

QDir MyInteractor::iconsDirectory() {
#if defined(Q_OS_MAC)
    return QDir(applicationDirectory().path() + "/../Resources/icons");
#else
    return QDir(applicationDirectory().path() + "/resources/icons");
#endif
}

const QString MyInteractor::iconsDirectoryPath() {
    return iconsDirectory().path();
}

QUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

QObject* MyInteractor::fileManager() {
    return _fileManager;
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

void MyInteractor::setSceneMode(const SceneMode& sceneMode) {
    MySceneModeElementBase::setSceneMode(sceneMode);
    emit modeIsSet(getSceneModeAsString());
}

void MyInteractor::createNetwork(const QJsonObject& json) {
    resetNetwork();
    setBackground(json);
    addNodes(json);
    addEdges(json);
}

void MyInteractor::setNewNetworkCanvas() {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    askForRemoveFeatureMenu();
    resetNetworkCanvas();
    ((MyFileManager*)fileManager())->reset();
}

void MyInteractor::saveCurrentNetwork() {
    if (((MyFileManager*)fileManager())->lastSavedFileName().isEmpty())
        writeDataToFile(((MyFileManager*)fileManager())->currentExportTool());
    else
        writeDataToFile(((MyFileManager*)fileManager())->currentExportTool(), ((MyFileManager*)fileManager())->currentFileName());
}

void MyInteractor::resetNetworkCanvas() {
    resetNetwork();
    resetCanvas();
}

void MyInteractor::resetCanvas() {
    emit askForResetScale();
    enableNormalMode();
    undoStack()->clear();
}

void MyInteractor::resetNetwork() {
    clearNodesInfo();
    clearEdgesInfo();
    emit askForClearScene();
}

void MyInteractor::setBackground(const QJsonObject &json) {
    if (json.contains("background-color") && json["background-color"].isString())
        askForSetNetworkBackgroundColor(json["background-color"].toString());
}

QList<MyNetworkElementBase*>& MyInteractor::nodes() {
    return _nodes;
}

void MyInteractor::addNodes(const QJsonObject &json) {
    if (json.contains("nodes") && json["nodes"].isArray()) {
        QJsonArray nodesArray = json["nodes"].toArray();
        for (int nodeIndex = 0; nodeIndex < nodesArray.size(); ++nodeIndex)
            addNode(nodesArray[nodeIndex].toObject());
        updateNodeParents();
    }
}

void MyInteractor::addNode(const QJsonObject &json) {
    MyNetworkElementBase* node = createNode(json);
    if (node)
        addNode(node);
}

void MyInteractor::addNode(MyNetworkElementBase* n) {
    if (n && !n->isActive()) {
        _nodes.push_back(n);
        n->setActive(true);
        n->updateGraphicsItem();
        connect(n, SIGNAL(askForSelectNetworkElement(MyNetworkElementBase*)), this, SLOT(changeElementSelectionsStatus(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForSelectNetworkElement(MyNetworkElementBase*)), this, SLOT(addNewEdge(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForDeleteNetworkElement(MyNetworkElementBase*)), this, SLOT(deleteNode(MyNetworkElementBase*)));
        connect(n, &MyNetworkElementBase::askForListOfElements, this, [this] () { return nodes() + edges(); } );
        connect(n, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
        connect(n, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(n, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));
        connect(n, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()));
        connect(n, SIGNAL(askForCheckWhetherNetworkElementNameIsAlreadyUsed(const QString&)), this, SLOT(isElementNameAlreadyUsed(const QString&)));
        connect(n, SIGNAL(askForCopyNetworkElement(MyNetworkElementBase*)), this, SLOT(setCopiedNode(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForCutNetworkElement(MyNetworkElementBase*)), this, SLOT(setCutNode(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForCopyNetworkElementStyle(MyNetworkElementStyleBase*)), this, SLOT(setCopiedNodeStyle(MyNetworkElementStyleBase*)));
        connect(n, SIGNAL(askForPasteNetworkElementStyle(MyNetworkElementBase*)), this, SLOT(pasteCopiedNodeStyle(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForWhetherElementStyleIsCopied()), this, SLOT(isSetCopiedNodeStyle()));
        connect(n, &MyNetworkElementBase::askForWhetherAnyOtherElementsAreSelected, this, [this] (QList<MyNetworkElementBase*> networkElements) {
            ((MyNetworkElementSelector*)_networkElementSelector)->areAnyOtherElementsSelected(networkElements); });
        connect(n, SIGNAL(askForIconsDirectoryPath()), this, SLOT(iconsDirectoryPath()));
        connect(n, SIGNAL(positionChangedByMouseMoveEvent(MyNetworkElementBase*, const QPointF&)), this, SLOT(moveSelectedNetworkElements(MyNetworkElementBase*, const QPointF&)));
        connect(n, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), this, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)));
        connect(n, SIGNAL(askForWhetherControlModifierIsPressed()), this, SIGNAL(askForWhetherControlModifierIsPressed()));
        connect(n->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(this, SIGNAL(askForAdjustConnectedEdgesOfNodes()), n, SLOT(adjustConnectedEdges()));
        connect(this, SIGNAL(askForAdjustExtentsOfNodes()), n, SLOT(adjustExtents()));
        emit askForAddGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::addNewNode(const QPointF& position) {
    if (getSceneMode() == ADD_NODE_MODE) {
        MyNetworkElementBase* node = createNode(getElementUniqueName(nodes(), nodeStyle()->category()), getCopyNodeStyle(getElementUniqueName(nodes(), nodeStyle()->category()) + "_style", nodeStyle()), position.x(), position.y());
        addNode(node);
        createChangeStageCommand();
    }
}

void MyInteractor::removeNode(MyNetworkElementBase* n) {
    if (n && n->isActive()) {
        _nodes.removeOne(n);
        n->setActive(false);
        n->setSelected(false);
        emit askForRemoveGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::updateNodeParents() {
    MyNetworkElementBase* parentNode = NULL;
    for (MyNetworkElementBase *node : qAsConst(nodes())) {
        parentNode = findElement(nodes(), ((MyNodeBase*)node)->parentNodeId());
        if (parentNode)
            ((MyNodeBase*)node)->setParentNode((MyNodeBase*)parentNode);
    }
}

void MyInteractor::clearNodesInfo() {
    while(nodes().size())
        delete nodes().takeLast();
}

void MyInteractor::setNodeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _nodeStyle = style;
    else
        _nodeStyle = NULL;
}

MyNetworkElementStyleBase* MyInteractor::nodeStyle() {
    return _nodeStyle;
}

void MyInteractor::setCopiedNode(MyNetworkElementBase* node) {
    resetCopiedNetworkElements();
    if (node)
        _copiedNetworkElements.push_back(node);
}

void MyInteractor::setCutNode(MyNetworkElementBase* node) {
    setCopiedNode(node);
    removeNode(node);
}

const bool MyInteractor::isSetCopiedNodeStyle() {
    if (_copiedNodeStyle)
        return true;

    return false;
}

void MyInteractor::setCopiedNodeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _copiedNodeStyle = style;
    else
        _copiedNodeStyle = NULL;
}

MyNetworkElementStyleBase* MyInteractor::copiedNodeStyle() {
    return _copiedNodeStyle;
}

void MyInteractor::pasteCopiedNodeStyle(MyNetworkElementBase* element) {
    element->updateStyle(getCopyNodeStyle(element->style()->name(), _copiedNodeStyle)->shapeStyles());
    element->updateGraphicsItem();
    createChangeStageCommand();
}

QList<MyNetworkElementBase*>& MyInteractor::edges() {
    return _edges;
}

void MyInteractor::addEdges(const QJsonObject &json) {
    if (json.contains("edges") && json["edges"].isArray()) {
        QJsonArray edgesArray = json["edges"].toArray();
        for (int edgeIndex = 0; edgeIndex < edgesArray.size(); ++edgeIndex)
            addEdge(edgesArray[edgeIndex].toObject());
    }
}

void MyInteractor::addEdge(const QJsonObject &json) {
    MyNetworkElementBase* edge = createEdge(json, findSourceNode(nodes(), json), findTargetNode(nodes(), json));
    if (edge)
        addEdge(edge);
}

void MyInteractor::addEdge(MyNetworkElementBase* e) {
    if (e && !edgeExists(((MyEdgeBase*)e)->sourceNode(), ((MyEdgeBase*)e)->targetNode()) && e->setActive(true)) {
        _edges.push_back(e);
        e->updateGraphicsItem();
        connect(e, SIGNAL(askForSelectNetworkElement(MyNetworkElementBase*)), this, SLOT(changeElementSelectionsStatus(MyNetworkElementBase*)));
        connect(e, SIGNAL(askForDeleteNetworkElement(MyNetworkElementBase*)), this, SLOT(deleteEdge(MyNetworkElementBase*)));
        connect(e, &MyNetworkElementBase::askForListOfElements, this, [this] () { return nodes() + edges(); } );
        connect(e, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
        connect(e, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(e, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));
        connect(e, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedNetworkElementFeatureMenu()));
        connect(e, SIGNAL(askForCheckWhetherNetworkElementNameIsAlreadyUsed(const QString&)), this, SLOT(isElementNameAlreadyUsed(const QString&)));
        connect(e, SIGNAL(askForCopyNetworkElementStyle(MyNetworkElementStyleBase*)), this, SLOT(setCopiedEdgeStyle(MyNetworkElementStyleBase*)));
        connect(e, SIGNAL(askForPasteNetworkElementStyle(MyNetworkElementBase*)), this, SLOT(pasteCopiedEdgeStyle(MyNetworkElementBase*)));
        connect(e, &MyNetworkElementBase::askForWhetherAnyOtherElementsAreSelected, this, [this] (QList<MyNetworkElementBase*> networkElements) {
             ((MyNetworkElementSelector*)_networkElementSelector)->areAnyOtherElementsSelected(networkElements); });
        connect(e, SIGNAL(askForWhetherElementStyleIsCopied()), this, SLOT(isSetCopiedEdgeStyle()));
        connect(e, SIGNAL(askForIconsDirectoryPath()), this, SLOT(iconsDirectoryPath()));
        connect(e->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        emit askForAddGraphicsItem(e->graphicsItem());

        if (((MyEdgeBase*)e)->isSetArrowHead())
            emit askForAddGraphicsItem(((MyEdgeBase*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::addNewEdge(MyNetworkElementBase* element) {
    if (getSceneMode() == ADD_EDGE_MODE) {
        if (!_newEdgeBuilder) {
            if (edgeStyle()->type() == "templatestyle") {
                _newEdgeBuilder = new MyNewTemplateBuilder(edgeStyle());
                connect((MyNewTemplateBuilder*)_newEdgeBuilder, &MyNewTemplateBuilder::askForAddNode, this, [this] (MyNetworkElementBase* node) { this->addNode(node); });
                connect((MyNewTemplateBuilder*)_newEdgeBuilder, &MyNewTemplateBuilder::askForNodeUniqueName, this, [this] (MyNetworkElementStyleBase* nodeStyle) { return getElementUniqueName(this->nodes(), nodeStyle->category()); });
            }
            else
                _newEdgeBuilder = new MyNewEdgeBuilder(edgeStyle());
            connect((MyNewEdgeBuilderBase*)_newEdgeBuilder, &MyNewEdgeBuilderBase::askForAddEdge, this, [this] (MyNetworkElementBase* edge) { this->addEdge(edge); });
            connect((MyNewEdgeBuilderBase*)_newEdgeBuilder, &MyNewEdgeBuilderBase::askForEdgeUniqueName, this, [this] (MyNetworkElementStyleBase* edgeStyle) { return getElementUniqueName(this->edges(), edgeStyle->category()); });
        }
        ((MyNewEdgeBuilderBase*)_newEdgeBuilder)->build(element);
        emit askForSetToolTip(((MyNewEdgeBuilderBase*)_newEdgeBuilder)->toolTipText());
        if (((MyNewEdgeBuilderBase*)_newEdgeBuilder)->isNewEdgeBuilt()) {
            deleteNewEdgeBuilder();
            createChangeStageCommand();
        }
    }
}

void MyInteractor::removeEdge(MyNetworkElementBase* e) {
    if (e && e->isActive()) {
        _edges.removeOne(e);
        e->setActive(false);
        e->setSelected(false);
        emit askForRemoveGraphicsItem(e->graphicsItem());
        if (((MyEdgeBase*)e)->isSetArrowHead())
            emit askForRemoveGraphicsItem(((MyEdgeBase*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::clearEdgesInfo() {
    while(edges().size())
        delete edges().takeLast();
}

void MyInteractor::setEdgeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _edgeStyle = style;
    else
        _edgeStyle = NULL;
}

MyNetworkElementStyleBase* MyInteractor::edgeStyle() {
    return _edgeStyle;
}

const bool MyInteractor::isSetCopiedEdgeStyle() {
    if (_copiedEdgeStyle)
        return true;

    return false;
}

void MyInteractor::setCopiedEdgeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _copiedEdgeStyle = style;
    else
        _copiedEdgeStyle = NULL;
}

MyNetworkElementStyleBase* MyInteractor::copiedEdgeStyle() {
    return _copiedEdgeStyle;
}

void MyInteractor::pasteCopiedEdgeStyle(MyNetworkElementBase* element) {
    element->updateStyle(getCopyEdgeStyle(element->style()->name(), _copiedEdgeStyle)->shapeStyles());
    element->updateGraphicsItem();
    createChangeStageCommand();
}

const bool MyInteractor::areSelectedElementsCopyable() {
    if (getSelectedNodes().size() || getSelectedEdges().size()) {
        for (MyNetworkElementBase* selectedNode : getSelectedNodes()) {
            if (!selectedNode->isCopyable())
                return false;
        }
        for (MyNetworkElementBase* selectedEdge : getSelectedEdges()) {
            if (!selectedEdge->isCopyable())
                return false;
        }

        return true;
    }

    return false;
}

const bool MyInteractor::areSelectedElementsCuttable() {
    if (getSelectedNodes().size() || getSelectedEdges().size()) {
        for (MyNetworkElementBase* selectedNode : getSelectedNodes()) {
            if (!selectedNode->isCuttable())
                return false;
        }
        for (MyNetworkElementBase* selectedEdge : getSelectedEdges()) {
            if (!selectedEdge->isCuttable())
                return false;
        }

        return true;
    }

    return false;
}

const bool MyInteractor::areSelectedElementsAlignable() {
    if (getSelectedNodes().size() > 1)
        return true;

    return false;
}

const bool MyInteractor::areAnyElementsCopied() {
    if (copiedNetworkElements().size())
        return true;

    return false;
}

const bool MyInteractor::areAnyElementsSelected() {
    if (getSelectedNodes().size() || getSelectedEdges().size())
        return true;

    return false;
}

void MyInteractor::copySelectedNetworkElements() {
    resetCopiedNetworkElements();
    for (MyNetworkElementBase* selectedElement : getSelectedElements()) {
        if (!selectedElement->isCopyable())
            return;
    }

    for (MyNetworkElementBase* selectedNode : getSelectedNodes())
        _copiedNetworkElements.push_back(selectedNode);
    for (MyNetworkElementBase* selectedEdge : getSelectedEdges())
        _copiedNetworkElements.push_back(selectedEdge);
    if (_copiedNetworkElements.size())
        emit pasteElementsStatusChanged(true);
}

void MyInteractor::cutSelectedNetworkElements() {
    copySelectedNetworkElements();
    if (copiedNetworkElements().size()) {
        for (MyNetworkElementBase* selectedEdge : getSelectedEdges())
            removeEdge(selectedEdge);
        for (MyNetworkElementBase* selectedNode : getSelectedNodes())
            removeNode(selectedNode);
    }
}

void MyInteractor::pasteCopiedNetworkElements() {
    pasteCopiedNetworkElements(askForItemsBoundingRect().center());
}

void MyInteractor::pasteCopiedNetworkElements(const QPointF& position) {
    MyCopiedNetworkElementsPaster* copiedNetworkElementsPaster = new MyCopiedNetworkElementsPaster(copiedNetworkElements(), position);
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForAddNode, this, [this] (MyNetworkElementBase* node) { this->addNode(node); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForNodeUniqueName, this, [this] (MyNetworkElementStyleBase* nodeStyle) { return getElementUniqueName(this->nodes(), nodeStyle->category()); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForAddEdge, this, [this] (MyNetworkElementBase* edge) { this->addEdge(edge); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForEdgeUniqueName, this, [this] (MyNetworkElementStyleBase* edgeStyle) { return getElementUniqueName(this->edges(), edgeStyle->category()); });
    copiedNetworkElementsPaster->paste();
    copiedNetworkElementsPaster->deleteLater();
    createChangeStageCommand();
}

QList<MyNetworkElementBase*> MyInteractor::copiedNetworkElements() {
    return _copiedNetworkElements;
}

void MyInteractor::resetCopiedNetworkElements() {
    _copiedNetworkElements.clear();
    emit pasteElementsStatusChanged(false);
}

void MyInteractor::moveSelectedNetworkElements(MyNetworkElementBase* movedNode, const QPointF& movedDistance) {
    MyNetworkElementMoverBase* nodeMover = new MyNodeMover(getSelectedNodes(), movedNode);
    nodeMover->move(movedDistance.x(), movedDistance.y());
    nodeMover->deleteLater();
}

void MyInteractor::deleteNewEdgeBuilder() {
    for (MyNetworkElementBase* selectedNode : getSelectedNodes())
        selectedNode->setSelected(false);
    if (_newEdgeBuilder)
        _newEdgeBuilder->deleteLater();
    _newEdgeBuilder = NULL;
}

bool MyInteractor::edgeExists(MyNetworkElementBase* n1, MyNetworkElementBase* n2) {
    for (MyNetworkElementBase *edge : qAsConst(edges())) {
        if ((((MyEdgeBase*)edge)->sourceNode() == n1 && ((MyEdgeBase*)edge)->targetNode() == n2) || (((MyEdgeBase*)edge)->sourceNode() == n2 && ((MyEdgeBase*)edge)->targetNode() == n1)) {
            return true;
        }
    }
    
    return false;
}

QJsonObject MyInteractor::getNetworkElementsAndColorInfo() {
    QJsonObject json;

    // background color
    json["background-color"] = askForNetworkBackgroundColor();

    // nodes
    QJsonArray nodesArray;
    for (MyNetworkElementBase *node : qAsConst(nodes())) {
        QJsonObject nodeObject;
        node->write(nodeObject);
        nodesArray.append(nodeObject);
    }
    json["nodes"] = nodesArray;

    // edges
    QJsonArray edgesArray;
    for (MyNetworkElementBase *edge : qAsConst(edges())) {
        QJsonObject edgeObject;
        edge->write(edgeObject);
        edgesArray.append(edgeObject);
    }
    json["edges"] = edgesArray;

    return json;
}

QJsonObject MyInteractor::exportNetworkInfo() {
    QJsonObject json = getNetworkElementsAndColorInfo();

    QRectF networkExtents = askForNetworkExtents();
    // position
    QJsonObject positionObject;
    positionObject["x"] = networkExtents.x() + 0.5 * networkExtents.width();
    positionObject["y"] = networkExtents.y() + 0.5 * networkExtents.height();
    json["position"] = positionObject;

    // dimensions
    QJsonObject dimensionsObject;
    dimensionsObject["width"] = networkExtents.width();
    dimensionsObject["height"] = networkExtents.height();
    json["dimensions"] = dimensionsObject;

    return json;
}

void MyInteractor::selectElements(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectElements(selected);
}

void MyInteractor::selectElementsOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectElementsOfCategory(category, selected);
}

void MyInteractor::selectNodes(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNodes(selected);
}

void MyInteractor::selectNodesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNodesOfCategory(category, selected);
}

void MyInteractor::selectEdges(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectEdges(selected);
}

void MyInteractor::selectEdgesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectEdgesOfCategory(category, selected);
}

void MyInteractor::changeElementSelectionsStatus(MyNetworkElementBase* element) {
    ((MyNetworkElementSelector*)_networkElementSelector)->changeElementSelectionsStatus(element);
}

void MyInteractor::setElementSelected(const QString& elementName) {
    ((MyNetworkElementSelector*)_networkElementSelector)->setElementSelected(elementName);
}

void MyInteractor::deleteNode(MyNetworkElementBase* node) {
    for (MyNetworkElementBase *edge : qAsConst(((MyNodeBase*)node)->edges())) {
        ((MyNodeBase*)node)->removeEdge(edge);
        removeEdge(edge);
    }
    removeNode(node);
    createChangeStageCommand();
}

void MyInteractor::deleteEdge(MyNetworkElementBase* edge) {
    removeEdge(edge);
    createChangeStageCommand();
}

void MyInteractor::deleteSelectedNetworkElements() {
    for (MyNetworkElementBase* selectedNode : getSelectedNodes()) {
        for (MyNetworkElementBase *edge : qAsConst(((MyNodeBase*)selectedNode)->edges())) {
            ((MyNodeBase*)selectedNode)->removeEdge(edge);
            removeEdge(edge);
        }
        removeNode(selectedNode);
    }
    for (MyNetworkElementBase* selectedEdge : getSelectedEdges()) {
        if (selectedEdge) {
            removeEdge(selectedEdge);
        }
    }
    createChangeStageCommand();
}

void MyInteractor::alignSelectedNetworkElements(const QString& alignType) {
    MyNetworkElementAlignerBase* networkElementAligner = createNetworkElementAligner(getSelectedNodes(), getSelectedEdges(), alignType);
    if (networkElementAligner) {
        networkElementAligner->align();
        networkElementAligner->deleteLater();
        createChangeStageCommand();
    }
}

const QList<MyNetworkElementBase*> MyInteractor::getSelectedElements() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedElements();
}

const QList<MyNetworkElementBase*> MyInteractor::getSelectedNodes() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedNodes();
}

const QList<MyNetworkElementBase*> MyInteractor::getSelectedEdges() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedEdges();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedElement() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedNode() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyInteractor::getOneSingleSelectedEdge() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

void MyInteractor::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    resetCopiedNetworkElements();
    setCopiedNode(NULL);
    setNodeStyle(NULL);
    setCopiedNodeStyle(NULL);
    setEdgeStyle(NULL);
    setCopiedEdgeStyle(NULL);
    deleteNewEdgeBuilder();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableNormalMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableNormalMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableNormalMode();

    emit askForSetToolTip("");
}

void MyInteractor::enableAddNodeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddNodeMode();
    askForRemoveFeatureMenu();
    setNodeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    ((MyNetworkElementSelector*)_networkElementSelector)->enableAddNodeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableAddNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableAddNodeMode();
    
    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
    emit addElementModeIsEnabled(style->name());
}

void MyInteractor::enableAddEdgeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddEdgeMode();
    askForRemoveFeatureMenu();
    setEdgeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    ((MyNetworkElementSelector*)_networkElementSelector)->enableAddEdgeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableAddEdgeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableAddEdgeMode();

    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
    emit addElementModeIsEnabled(style->name());
}

void MyInteractor::enableSelectMode(const QString& elementCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();

    emit askForSetToolTip("Select" + elementCategory);
}

void MyInteractor::enableSelectNodeMode(const QString& nodeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectNodeMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectNodeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectNodeMode();
    
    emit askForSetToolTip("Select " + nodeCategory + " nodes");
}

void MyInteractor::enableSelectEdgeMode(const QString& edgeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectEdgeMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectEdgeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectEdgeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();
    
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
    if (((MyNetworkElementSelector*)_networkElementSelector)->canDisplaySingleElementFeatureMenu()) {
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
    ((MyNetworkElementSelector*)_networkElementSelector)->displaySelectionArea(position);
}

void MyInteractor::createSelectionAreaGraphicsItem(const QPointF& position) {
    ((MyNetworkElementSelector*)_networkElementSelector)->createSelectionAreaGraphicsItem(position);
}

void MyInteractor::selectSelectionAreaCoveredNodes() {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectSelectionAreaCoveredNodes();
}

void MyInteractor::selectSelectionAreaCoveredEdges() {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectSelectionAreaCoveredEdges();
}

void MyInteractor::clearSelectionArea() {
    ((MyNetworkElementSelector*)_networkElementSelector)->clearSelectionArea();
    displayFeatureMenu();
}

void MyInteractor::readFromFile(const QString& importToolName) {
    QList<MyPluginItemBase*> importTools = getPluginsOfType(plugins(), "importtool");
    for (MyPluginItemBase* importTool : importTools) {
        if (importTool->name() == importToolName)
            return readFromFile(importTool);
    }
}

void MyInteractor::readFromFile(MyPluginItemBase* importTool) {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    QString fileName = ((MyImportTool*)importTool)->getOpenFileName(((MyFileManager*)fileManager())->workingDirectory());
    if (!fileName.isEmpty()) {
        createNetwork(importInterface()->readGraphInfoFromFile(fileName, importTool->name()));
        createChangeStageCommand();
        resetCanvas();
        ((MyFileManager*)fileManager())->setCurrentExportToolCompatibleWithImportTool(importTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
        ((MyFileManager*)fileManager())->setWorkingDirectory(QFileInfo(fileName).absolutePath() + "/");
    }
}

void MyInteractor::writeDataToFile(const QString& exportToolName) {
    QList<MyPluginItemBase*> exportTools = getPluginsOfType(plugins(), "dataexporttool");
    for (MyPluginItemBase* exportTool : exportTools) {
        if (exportTool->name() == exportToolName)
            return writeDataToFile(exportTool);
    }
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName(((MyFileManager*)fileManager())->workingDirectory(), ((MyFileManager*)fileManager())->currentBaseFileName());
    if (!fileName.isEmpty())
        writeDataToFile(exportTool, fileName);
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName) {
    QJsonObject graphInfoObject = exportNetworkInfo();
    ((MyDataExportTool*)exportTool)->readCompatibilityInfo(dataExportInterface()->checkForGraphInfoCompatibility(graphInfoObject, exportTool->name()));
    if (((MyDataExportTool*)exportTool)->isInfoCompatible()) {
        dataExportInterface()->writeGraphInfoToFile(graphInfoObject, fileName, exportTool->name());
        ((MyFileManager*)fileManager())->setCurrentExportTool(exportTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
        ((MyFileManager*)fileManager())->setWorkingDirectory(QFileInfo(fileName).absolutePath() + "/");
    }
    ((MyDataExportTool*)exportTool)->showMessages();
}

void MyInteractor::writeFigureToFile(const QString& exportToolName) {
    QList<MyPluginItemBase*> exportTools = getPluginsOfType(plugins(), "printexporttool");
    for (MyPluginItemBase* exportTool : exportTools) {
        if (exportTool->name() == exportToolName)
            return writeFigureToFile(exportTool);
    }
}

void MyInteractor::writeFigureToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName(((MyFileManager*)fileManager())->workingDirectory());
    if (!fileName.isEmpty()) {
        emit askForExportFigure(fileName, ((MyPrintExportTool*)exportTool)->fileExtension());
    }
}

void MyInteractor::autoLayout(MyPluginItemBase* autoLayoutEngine) {
    if (!((MyAutoLayoutEngine*)autoLayoutEngine)->takeParameters()) {
        QJsonObject autoLayoutInfoObject;
        autoLayoutEngine->write(autoLayoutInfoObject);
        QJsonObject graphInfoObject = exportNetworkInfo();
        autoLayoutInterface()->autoLayout(graphInfoObject, autoLayoutInfoObject);
        createNetwork(graphInfoObject);
        emit askForAdjustExtentsOfNodes();
        emit askForAdjustConnectedEdgesOfNodes();
        createChangeStageCommand();
        enableNormalMode();
    }
}

void MyInteractor::loadPlugins() {
    QDir pluginsDir = applicationDirectory();
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS")
        pluginsDir.cdUp();
#endif
    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = pluginLoader.instance();
        if (plugin) {
            // import interface
            if (qobject_cast<ImportInterface *>(plugin))
                setImportInterface(qobject_cast<ImportInterface *>(plugin), applicationDirectory().path(), pluginsDir.path());
            // data export interface
            else if (qobject_cast<DataExportInterface *>(plugin))
                setDataExportInterface(qobject_cast<DataExportInterface *>(plugin), applicationDirectory().path(), pluginsDir.path());
            // print export interface
            else if (qobject_cast<PrintExportInterface *>(plugin))
                setPrintExportInterface(qobject_cast<PrintExportInterface *>(plugin), applicationDirectory().path(), pluginsDir.path());
            // element style interface
            else if (qobject_cast<ElementStyleInterface *>(plugin))
                setElementStyleInterface(qobject_cast<ElementStyleInterface *>(plugin), applicationDirectory().path(), pluginsDir.path());
            // auto layout interface
            else if (qobject_cast<AutoLayoutInterface *>(plugin))
                setAutoLayoutInterface(qobject_cast<AutoLayoutInterface *>(plugin), applicationDirectory().path(), pluginsDir.path());
        }
    }
}

void MyInteractor::setFileManager() {
    _fileManager = new MyFileManager(getPluginsOfType(plugins(), "importtool"), getPluginsOfType(plugins(), "dataexporttool"));
    connect(_fileManager, SIGNAL(currentFileNameIsUpdated(const QString&)), this, SIGNAL(currentFileNameIsUpdated(const QString&)));
    connect(this, &MyInteractor::askForWorkingDirectoryPath, this, [this] () { return ((MyFileManager*)fileManager())->workingDirectory(); });
    connect(this, &MyInteractor::askForSettingWorkingDirectoryPath, this, [this] (const QString& workingDirectoryPath) { ((MyFileManager*)fileManager())->setWorkingDirectory(QFileInfo(workingDirectoryPath).absolutePath() + "/"); });
}

void MyInteractor::setNetworkElementSelector() {
    _networkElementSelector = new MyNetworkElementSelector();
    connect(_networkElementSelector, SIGNAL(askForWhetherShiftModifierIsPressed()), this, SIGNAL(askForWhetherShiftModifierIsPressed()));
    connect((MyNetworkElementSelector*)_networkElementSelector, &MyNetworkElementSelector::askForNodes, this, [this] () { return nodes(); });
    connect((MyNetworkElementSelector*)_networkElementSelector, &MyNetworkElementSelector::askForEdges, this, [this] () { return edges(); });
    connect((MyNetworkElementSelector*)_networkElementSelector, &MyNetworkElementSelector::networkElementsSelectedStatusIsChanged, this, [this] () {
        emit elementsCuttableStatusChanged(areSelectedElementsCuttable());
        emit elementsCopyableStatusChanged(areSelectedElementsCopyable());
    });
    connect(this, &MyInteractor::singleNetworkElementFeatureMenuIsDisplayed, this, [this] (const QString& elementName) {
        ((MyNetworkElementSelector*)_networkElementSelector)->selectElements(false);
        ((MyNetworkElementSelector*)_networkElementSelector)->setElementSelected(elementName);
    });
    connect(this, &MyInteractor::multiNetworkElementFeatureMenuIsDisplayed, this, [this] (const QString& elementName) {
        ((MyNetworkElementSelector*)_networkElementSelector)->setElementSelected(elementName);
    });
    connect(_networkElementSelector, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
    connect(_networkElementSelector, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
}

QList<QAbstractButton*> MyInteractor::getToolbarMenuButtons() {
    return createToolbarMenuButtons(this, undoStack(), plugins(), iconsDirectoryPath());
}

QList<QAbstractButton*> MyInteractor::getModeMenuButtons() {
    addDefaultNodeStyle();
    addDefaultEdgeStyle();
    return createModeMenuButtons(this, plugins(), iconsDirectoryPath());
}

void MyInteractor::addDefaultNodeStyle() {
    if (!getPluginsOfType(plugins(), "nodestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        _plugins.push_back(createNodeStyle(styleObject));
    }
}

void MyInteractor::addDefaultEdgeStyle() {
    if (!getPluginsOfType(plugins(), "edgestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        _plugins.push_back(createEdgeStyle(styleObject));
    }
}

const bool MyInteractor::isElementNameAlreadyUsed(const QString& elementName) {
    for (MyNetworkElementBase* node : qAsConst(nodes())) {
        if (node->name() == elementName)
            return true;
    }
    for (MyNetworkElementBase* edge : qAsConst(edges())) {
        if (edge->name() == elementName)
            return true;
    }

    return false;
}
