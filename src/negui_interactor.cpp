#include "negui_interactor.h"
#include "negui_file_manager.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_builder.h"
#include "negui_edge_builder.h"
#include "negui_new_edge_builder.h"
#include "negui_selection_area_graphics_item.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"
#include "negui_plugin_item_builder.h"
#include "negui_import_tools.h"
#include "negui_autolayout_engines.h"
#include "negui_export_tools.h"
#include "negui_customized_interactor_widgets.h"
#include "negui_decorate_menu_buttons.h"

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
    _selectionAreaGraphicsItem = NULL;

    // plugins
    loadPlugins();

    // file manager
    setFileManager();

    // network
    resetNetwork();
    _stageInfo = exportNetworkInfo();
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

bool MyInteractor::setImportInterface(ImportInterface* importInterface, const QString &path) {
    if (importInterface) {
        _importInterface = importInterface;
        if (!_importInterface->initialize(path)) {
            readPluginItemsInfo(_importInterface->loadItemsInfo());
            _isSetImportInterface = true;
        }
    }
    
    return _isSetImportInterface;
}

ImportInterface* MyInteractor::importInterface() {
    return _importInterface;
}

bool MyInteractor::setDataExportInterface(DataExportInterface* dataExportInterface, const QString &path) {
    if (dataExportInterface) {
        _dataExportInterface = dataExportInterface;
        if (!_dataExportInterface->initialize(path)) {
            readPluginItemsInfo(_dataExportInterface->loadItemsInfo());
            _isSetDataExportInterface = true;
        }
    }
    
    return _isSetDataExportInterface;
}

DataExportInterface* MyInteractor::dataExportInterface() {
    return _dataExportInterface;
}

bool MyInteractor::setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &path) {
    if (printExportInterface) {
        _printExportInterface = printExportInterface;
        if (!_printExportInterface->initialize(path)) {
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

bool MyInteractor::setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &path) {
    if (elementStyleInterface) {
        _elementStyleInterface = elementStyleInterface;
        if (!_elementStyleInterface->initialize(path)) {
            readPluginItemsInfo(_elementStyleInterface->loadItemsInfo());
            _isSetElementStyleInterface = true;
        }
    }
    
    return _isSetElementStyleInterface;
}

ElementStyleInterface* MyInteractor::elementStyleInterface() {
    return _elementStyleInterface;
}

bool MyInteractor::setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &path) {
    if (autoLayoutInterface) {
        _autoLayoutInterface = autoLayoutInterface;
        if (!_autoLayoutInterface->initialize(path)) {
            readPluginItemsInfo(_autoLayoutInterface->loadItemsInfo());
            _isSetAutoLayoutInterface = true;
        }
    }
    
    return _isSetAutoLayoutInterface;
}

AutoLayoutInterface* MyInteractor::autoLayoutInterface() {
    return _autoLayoutInterface;
}

QUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

QObject* MyInteractor::fileManager() {
    return _fileManager;
}

void MyInteractor::createChangeStageCommand() {
    QJsonObject currentStageInfo = exportNetworkInfo();
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

void MyInteractor::setSceneMode(SceneMode sceneMode) {
    MySceneModeElementBase::setSceneMode(sceneMode);
    emit modeIsSet(getSceneModeAsString());
}

void MyInteractor::createNetwork(const QJsonObject& json) {
    resetNetwork();
    setNetworkExtents(json);
    addNodes(json);
    addEdges(json);
}

void MyInteractor::setNewNetworkCanvas() {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
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
    resetCanvas();
    resetNetwork();
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
    setNetworkExtents(30.0, 20.0, 840.0, 560.0);
}

void MyInteractor::setNetworkExtents(const QJsonObject& json) {
    if (json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble() && json.contains("dimensions") && json["dimensions"].isObject() && json["dimensions"].toObject().contains("width") && json["dimensions"]["width"].isDouble() && json["dimensions"].toObject().contains("height") && json["dimensions"]["height"].isDouble())
        setNetworkExtents(json["position"]["x"].toDouble() - 0.5 * json["dimensions"]["width"].toDouble(), json["position"]["y"].toDouble() - 0.5 * json["dimensions"]["height"].toDouble(), json["dimensions"]["width"].toDouble(), json["dimensions"]["height"].toDouble());
}

void MyInteractor::setNetworkExtents(qreal x, qreal y, qreal width, qreal height) {
    qreal minWidth = 840.0;
    qreal minHeight = 560.0;
    if (width < minWidth) {
        x -= 0.5 * (minWidth - width);
        width = minWidth;
    }
    
    if (height < minHeight) {
        y -= 0.5 * (minHeight - height);
        height = minHeight;
    }
    
    _networkExtents = QRectF(x, y, width, height);
    emit askForSetSceneRect(x, y, width, height);
}

const QRectF& MyInteractor::networkExtents() {
    return _networkExtents;
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
        connect(n, SIGNAL(askForParentNodeAtPosition(MyNetworkElementBase*, const QPointF&)), this, SLOT(parentNodeAtPosition(MyNetworkElementBase*, const QPointF&)));
        connect(n, SIGNAL(elementObject(MyNetworkElementBase*)), this, SLOT(selectElement(MyNetworkElementBase*)));
        connect(n, SIGNAL(elementObject(MyNetworkElementBase*)), this, SLOT(addNewEdge(MyNetworkElementBase*)));
        connect(n, SIGNAL(elementObject(MyNetworkElementBase*)), this, SLOT(removeElement(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(n, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
        connect(n, SIGNAL(askForCopyNetworkElementStyle(MyNetworkElementStyleBase*)), this, SLOT(setCopiedNodeStyle(MyNetworkElementStyleBase*)));
        connect(n, SIGNAL(askForPasteNetworkElementStyle(MyNetworkElementBase*)), this, SLOT(pasteCopiedNodeStyle(MyNetworkElementBase*)));
        connect(n, SIGNAL(askForWhetherCopiedElementStyleIsSet()), this, SLOT(isSetCopiedNodeStyle()));
        connect(n->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForClearFocusedGraphicsItems()), this, SLOT(clearElementsFocusedGraphicsItems()));
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

MyNetworkElementBase* MyInteractor::parentNodeAtPosition(MyNetworkElementBase* currentNode, const QPointF& position) {
    QList<QGraphicsItem *> items = askForItemsAtPosition(position);
    MyNetworkElementBase* parentNode = NULL;
    qint32 parentNodeZValue = INT_MIN;
    for (QGraphicsItem* item : qAsConst(items)) {
        if (item->parentItem()) {
            for (MyNetworkElementBase* node : qAsConst(nodes())) {
                if (node->graphicsItem() == item->parentItem() && node != currentNode) {
                    if (item->parentItem()->zValue() > parentNodeZValue) {
                        parentNode = node;
                        parentNodeZValue = item->parentItem()->zValue();
                    }
                }
            }
        }
    }
    
    return parentNode;
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
    MyNetworkElementBase* edge = createEdge(json, findStartNode(nodes(), json), findEndNode(nodes(), json));
    if (edge)
        addEdge(edge);
}

void MyInteractor::addEdge(MyNetworkElementBase* e) {
    if (e && !edgeExists(((MyEdgeBase*)e)->startNode(), ((MyEdgeBase*)e)->endNode()) && e->setActive(true)) {
        _edges.push_back(e);
        e->updateGraphicsItem();
        connect(e, SIGNAL(elementObject(MyNetworkElementBase*)), this, SLOT(selectElement(MyNetworkElementBase*)));
        connect(e, SIGNAL(elementObject(MyNetworkElementBase*)), this, SLOT(removeElement(MyNetworkElementBase*)));
        connect(e, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(e, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
        connect(e, SIGNAL(askForCopyNetworkElementStyle(MyNetworkElementStyleBase*)), this, SLOT(setCopiedEdgeStyle(MyNetworkElementStyleBase*)));
        connect(e, SIGNAL(askForPasteNetworkElementStyle(MyNetworkElementBase*)), this, SLOT(pasteCopiedEdgeStyle(MyNetworkElementBase*)));
        connect(e, SIGNAL(askForWhetherCopiedElementStyleIsSet()), this, SLOT(isSetCopiedEdgeStyle()));
        connect(e->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForClearFocusedGraphicsItems()), this, SLOT(clearElementsFocusedGraphicsItems()));
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
        e->setActive(false);
        _edges.removeOne(e);
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

void MyInteractor::deleteNewEdgeBuilder() {
    for (MyNetworkElementBase* selectedNode : selectedNodes())
        selectedNode->setSelected(false);
    if (_newEdgeBuilder)
        _newEdgeBuilder->deleteLater();
    _newEdgeBuilder = NULL;
}

bool MyInteractor::edgeExists(MyNetworkElementBase* n1, MyNetworkElementBase* n2) {
    for (MyNetworkElementBase *edge : qAsConst(edges())) {
        if ((((MyEdgeBase*)edge)->startNode() == n1 && ((MyEdgeBase*)edge)->endNode() == n2) || (((MyEdgeBase*)edge)->startNode() == n2 && ((MyEdgeBase*)edge)->endNode() == n1)) {
            return true;
        }
    }
    
    return false;
}

QJsonObject MyInteractor::exportNetworkInfo() {
    QJsonObject json;
    
    // position
    QJsonObject positionObject;
    positionObject["x"] = networkExtents().x() + 0.5 * networkExtents().width();
    positionObject["y"] = networkExtents().y() + 0.5 * networkExtents().height();
    json["position"] = positionObject;
    
    // dimensions
    QJsonObject dimensionsObject;
    dimensionsObject["width"] = networkExtents().width();
    dimensionsObject["height"] = networkExtents().height();
    json["dimensions"] = dimensionsObject;
    
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

void MyInteractor::selectElements(const bool& selected) {
    selectNodes(selected);
    selectEdges(selected);
}

void MyInteractor::selectNodes(const bool& selected) {
    for (MyNetworkElementBase* node : qAsConst(nodes()))
        node->setSelected(selected);
}

void MyInteractor::selectEdges(const bool& selected) {
    for (MyNetworkElementBase* edge : qAsConst(edges()))
        edge->setSelected(selected);
}

void MyInteractor::selectElement(MyNetworkElementBase* element) {
    if (getSceneMode() == NORMAL_MODE) {
        if (!askForWhetherShiftModifierIsPressed())
            selectElements(false);
        if (!element->isSelected())
            element->setSelected(true);
        else
            element->setSelected(false);
    }
}

void MyInteractor::removeElement(MyNetworkElementBase* element) {
    if (getSceneMode() == REMOVE_MODE) {
        if (element->type() == MyNetworkElementBase::NODE_ELEMENT) {
            removeNode(element);
            for (MyNetworkElementBase *edge : qAsConst(((MyNodeBase*)element)->edges()))
                removeEdge(edge);
        }
        else if (element->type() == MyNetworkElementBase::EDGE_ELEMENT) {
            ((MyEdgeBase*)element)->connectToNodes(false);
            removeEdge(element);
        }
        createChangeStageCommand();
    }
}

const QList<MyNetworkElementBase*> MyInteractor::selectedNodes() {
    QList<MyNetworkElementBase*> selectedNodesList;
    for (MyNetworkElementBase *node : qAsConst(nodes())) {
        if (node->isSelected())
            selectedNodesList.push_back(node);
    }
    
    return selectedNodesList;
}

const bool MyInteractor::isAnyNodesSelected() {
    if (selectedNodes().size())
        return true;

    return false;
}

const QList<MyNetworkElementBase*> MyInteractor::selectedEdges() {
    QList<MyNetworkElementBase*> selectedEdgesList;
    for (MyNetworkElementBase *edge : qAsConst(edges())) {
        if (edge->isSelected())
            selectedEdgesList.push_back(edge);
    }
    
    return selectedEdgesList;
}

const bool MyInteractor::isAnyEdgesSelected() {
    if (selectedEdges().size())
        return true;

    return false;
}

void MyInteractor::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    setNodeStyle(NULL);
    setCopiedNodeStyle(NULL);
    setEdgeStyle(NULL);
    setCopiedEdgeStyle(NULL);
    deleteNewEdgeBuilder();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableNormalMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableNormalMode();
    
    emit askForSetToolTip("");
}

void MyInteractor::enableAddNodeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddNodeMode();
    setNodeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableAddNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableAddNodeMode();
    
    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
}

void MyInteractor::enableAddEdgeMode(MyPluginItemBase* style) {
    enableNormalMode();
    MySceneModeElementBase::enableAddEdgeMode();
    setEdgeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableAddEdgeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableAddEdgeMode();

    emit askForSetToolTip(((MyNetworkElementStyleBase*)style)->toolTipText());
}

void MyInteractor::enableSelectMode(const QString& elementCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();

    emit askForSetToolTip("Select" + elementCategory);
}

void MyInteractor::enableSelectNodeMode(const QString& nodeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectNodeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectNodeMode();
    
    emit askForSetToolTip("Select " + nodeCategory + " nodes");
}

void MyInteractor::enableSelectEdgeMode(const QString& edgeCategory) {
    enableNormalMode();
    MySceneModeElementBase::enableSelectEdgeMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableSelectEdgeMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();
    
    emit askForSetToolTip("Select " + edgeCategory + " edges");
}

void MyInteractor::enableRemoveMode() {
    enableNormalMode();
    MySceneModeElementBase::enableRemoveMode();
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->enableRemoveMode();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->enableRemoveMode();
    
    emit askForSetToolTip("Select Item");
}

void MyInteractor::clearElementsFocusedGraphicsItems() {
    for (MyNetworkElementBase *node : qAsConst(nodes()))
        node->graphicsItem()->clearFocusedGraphicsItems();
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->graphicsItem()->clearFocusedGraphicsItems();
}

void MyInteractor::displaySelectionArea(const QPointF& position) {
    if (getSceneMode() == NORMAL_MODE) {
        if (!askForWhetherShiftModifierIsPressed())
            selectElements(false);
        createSelectionAreaGraphicsItem(position);
        selectSelectionAreaCoveredNodes();
        selectSelectionAreaCoveredEdges();
    }
}

void MyInteractor::createSelectionAreaGraphicsItem(const QPointF& position) {
    if (!_selectionAreaGraphicsItem) {
        _selectionAreaGraphicsItem = new MySelectionAreaGraphicsItem(position);
        emit askForAddGraphicsItem(_selectionAreaGraphicsItem);
    }
    ((MySelectionAreaGraphicsItem*)_selectionAreaGraphicsItem)->updateExtents(position);
}

void MyInteractor::selectSelectionAreaCoveredNodes() {
    QList<QGraphicsItem *> selectedItems = _selectionAreaGraphicsItem->collidingItems();
    for (MyNetworkElementBase* node : qAsConst(nodes())) {
        for (QGraphicsItem* item : qAsConst(selectedItems)) {
            if (item->parentItem()) {
                if (node->graphicsItem() == item->parentItem())
                    node->setSelected(true);
            }
        }
    }
}

void MyInteractor::selectSelectionAreaCoveredEdges() {
    QList<QGraphicsItem *> selectedItems = _selectionAreaGraphicsItem->collidingItems();
    for (MyNetworkElementBase* edge : qAsConst(edges())) {
        for (QGraphicsItem* item : qAsConst(selectedItems)) {
            if (item->parentItem()) {
                if (edge->graphicsItem() == item->parentItem())
                    edge->setSelected(true);
            }
        }
    }
}

void MyInteractor::clearSelectionArea() {
    if (_selectionAreaGraphicsItem) {
        askForRemoveGraphicsItem(_selectionAreaGraphicsItem);
        delete _selectionAreaGraphicsItem;
        _selectionAreaGraphicsItem = NULL;
    }
}

void MyInteractor::readFromFile(MyPluginItemBase* importTool) {
    if (((MyFileManager*)fileManager())->canSaveCurrentNetwork())
        saveCurrentNetwork();
    QString fileName = ((MyImportTool*)importTool)->getOpenFileName();
    if (!fileName.isEmpty()) {
        createNetwork(importInterface()->readGraphInfoFromFile(fileName, importTool->name()));
        resetCanvas();
        ((MyFileManager*)fileManager())->setCurrentExportToolCompatibleWithImportTool(importTool);
        ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
        ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
    }
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName(((MyFileManager*)fileManager())->currentFileName());
    if (!fileName.isEmpty())
        writeDataToFile(exportTool, fileName);
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool, const QString& fileName) {
    QJsonObject graphInfoObject = exportNetworkInfo();
    ((MyDataExportTool*)exportTool)->readCompatibilityInfo(dataExportInterface()->checkForGraphInfoCompatibiliy(graphInfoObject, exportTool->name()));
    if (((MyDataExportTool*)exportTool)->isInfoCompatible())
        dataExportInterface()->writeGraphInfoToFile(graphInfoObject, fileName, exportTool->name());
    ((MyDataExportTool*)exportTool)->showMessages();
    ((MyFileManager*)fileManager())->setCurrentExportTool(exportTool);
    ((MyFileManager*)fileManager())->setCurrentFileName(fileName);
    ((MyFileManager*)fileManager())->setLastSavedFileName(fileName);
}

void MyInteractor::writeFigureToFile(MyPluginItemBase* exportTool) {
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName();
    if (!fileName.isEmpty()) {
        emit askForExportFigure(fileName, ((MyPrintExportTool*)exportTool)->outputFormat());
    }
}

void MyInteractor::autoLayout(MyPluginItemBase* autoLayoutEngine) {
    if (!((MyAutoLayoutEngine*)autoLayoutEngine)->takeParameters()) {
        QJsonObject autoLayoutInfoObject;
        autoLayoutEngine->write(autoLayoutInfoObject);
        QJsonObject graphInfoObject = exportNetworkInfo();
        autoLayoutInterface()->autoLayout(graphInfoObject, autoLayoutInfoObject);
        createNetwork(graphInfoObject);
        createChangeStageCommand();
        enableNormalMode();
    }
}

void MyInteractor::loadPlugins() {
    QDir pluginsDir(QCoreApplication::applicationDirPath());
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
                setImportInterface(qobject_cast<ImportInterface *>(plugin), pluginsDir.path());
            // data export interface
            else if (qobject_cast<DataExportInterface *>(plugin))
                setDataExportInterface(qobject_cast<DataExportInterface *>(plugin), pluginsDir.path());
            // print export interface
            else if (qobject_cast<PrintExportInterface *>(plugin))
                setPrintExportInterface(qobject_cast<PrintExportInterface *>(plugin), pluginsDir.path());
            // element style interface
            else if (qobject_cast<ElementStyleInterface *>(plugin))
                setElementStyleInterface(qobject_cast<ElementStyleInterface *>(plugin), pluginsDir.path());
            // auto layout interface
            else if (qobject_cast<AutoLayoutInterface *>(plugin))
                setAutoLayoutInterface(qobject_cast<AutoLayoutInterface *>(plugin), pluginsDir.path());
        }
    }
}

void MyInteractor::setFileManager() {
    _fileManager = new MyFileManager(getPluginsOfType(plugins(), "importtool"), getPluginsOfType(plugins(), "dataexporttool"));
    connect(_fileManager, SIGNAL(currentFileNameIsUpdated(const QString&)), this, SIGNAL(currentFileNameIsUpdated(const QString&)));
}

QList<QToolButton*> MyInteractor::getToolBarMenuButtons() {
    QList<QToolButton*> buttons;
    if (getPluginsOfType(plugins(), "importtool").size())
        buttons.push_back(createImportMenuButton());
    if (getPluginsOfType(plugins(), "dataexporttool").size() || getPluginsOfType(plugins(), "printexporttool").size()) {
        buttons.push_back(createExportMenuButton());
        buttons.push_back(createSaveMenuButton());
    }
    if (getPluginsOfType(plugins(), "autolayoutengine").size())
        buttons.push_back(createAutoLayoutMenuButton());
    buttons.push_back(createUndoActionMenuButton());
    buttons.push_back(createRedoActionMenuButton());
    buttons.push_back(createResetSceneMenuButton());
    
    return buttons;
}

QToolButton* MyInteractor::getNormalModeButton() {
    return createNormalModeMenuButton();
}

QList<QToolButton*> MyInteractor::getAddModeButtons() {
    return createAddElementMenuButtons();
}

QToolButton* MyInteractor::getRemoveModeButton() {
    return createRemoveElementMenuButton();
}

QToolButton* MyInteractor::createNormalModeMenuButton() {
    QToolButton* button = new MyModeToolButton("Normal");
    connect(button, SIGNAL(clicked()), this, SLOT(enableNormalMode()));
    return button;
}

QToolButton* MyInteractor::createImportMenuButton() {
    QToolButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* importWidgetAction = new MyWidgetAction(subMenu);
    importWidgetAction->setItems(getPluginsOfType(plugins(), "importtool"));
    connect(importWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(readFromFile(MyPluginItemBase*)));
    subMenu->addAction(importWidgetAction);
    button->setMenu(subMenu);
    decorateImportButton(button);
    return button;
}

QToolButton* MyInteractor::createExportMenuButton() {
    QList<MyPluginItemBase*> dataExportPlugins = getPluginsOfType(plugins(), "dataexporttool");
    QList<MyPluginItemBase*> printExportPlugins = getPluginsOfType(plugins(), "printexporttool");
    
    QToolButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    
    // data export
    if (dataExportPlugins.size()) {
        MyWidgetAction* dataExportWidgetAction = new MyWidgetAction(subMenu);
        dataExportWidgetAction->setItems((getPluginsOfType(plugins(), "dataexporttool")));
        connect(dataExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(writeDataToFile(MyPluginItemBase*)));
        subMenu->addAction(dataExportWidgetAction);
    }
    
    if (dataExportPlugins.size() && dataExportPlugins.size())
        subMenu->addSeparator();
    
    // print export
    if (dataExportPlugins.size()) {
        MyWidgetAction* printExportWidgetAction = new MyWidgetAction(subMenu);
        printExportWidgetAction->setItems((getPluginsOfType(plugins(), "printexporttool")));
        connect(printExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(writeFigureToFile(MyPluginItemBase*)));
        subMenu->addAction(printExportWidgetAction);
    }

    button->setMenu(subMenu);
    decorateExportButton(button);
    return button;
}

QToolButton* MyInteractor::createSaveMenuButton() {
    QToolButton* button = new MyToolButton();
    connect(button, SIGNAL(clicked()), this, SLOT(saveCurrentNetwork()));
    decorateSaveButton(button);
    return button;
}

QList<QToolButton*> MyInteractor::createAddElementMenuButtons() {
    addDefaultNodeStyle();
    addDefaultEdgeStyle();
    return createElementStyleButtons();
}

QList<QToolButton*> MyInteractor::createElementStyleButtons() {
    QList<QToolButton*> elementStyleButtons;
    for (QString category : getPluginsCategories(plugins()))
        elementStyleButtons.push_back(createPluginItemToolButton(createCategoryMenu(getPluginsOfCategory(getPluginsOfType(plugins(), "nodestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins(), "edgestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins(), "templatestyle"), category)), category));

    return elementStyleButtons;
}

QMenu* MyInteractor::createCategoryMenu(QList<MyPluginItemBase*> nodeStylesOfCategory, QList<MyPluginItemBase*> edgeStylesOfCategory, QList<MyPluginItemBase*> templateStylesOfCategory) {
    QMenu* menu = new MyToolButtonCategoryMenu();
    // node
    if (nodeStylesOfCategory.size()) {
        QWidgetAction* nodeStyleWidgetAction = createElementStyleWidgetAction(nodeStylesOfCategory, menu);
        connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddNodeMode(MyPluginItemBase*)));
        connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction(nodeStyleWidgetAction);
    }
    if (nodeStylesOfCategory.size() && (edgeStylesOfCategory.size() || templateStylesOfCategory.size()))
        menu->addSeparator();
    // edge
    if (edgeStylesOfCategory.size()) {
        QWidgetAction* edgeStyleWidgetAction = createElementStyleWidgetAction(edgeStylesOfCategory, menu);
        connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction(edgeStyleWidgetAction);
    }
    if (edgeStylesOfCategory.size() && templateStylesOfCategory.size())
        menu->addSeparator();
    // template
    if (templateStylesOfCategory.size()) {
        QWidgetAction* templateStyleWidgetAction = createElementStyleWidgetAction(templateStylesOfCategory, menu);
        connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction(templateStyleWidgetAction);
    }

    return menu;
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

QToolButton* MyInteractor::createPluginItemToolButton(QMenu* subMenu, const QString& text) {
    QToolButton* button = new MyToolButton();
    button->setText(text);
    button->setToolTip("Add " + text + " to the network");
    button->setMenu(subMenu);
    connect(subMenu, SIGNAL(menuItemIsChosen()), button, SIGNAL(menuItemIsChosen()));
    return button;
}

QWidgetAction* MyInteractor::createElementStyleWidgetAction(QList<MyPluginItemBase*> elementStyles, QWidget* parent) {
    MyWidgetAction* widgetAction = new MyWidgetAction(parent);
    widgetAction->setItems(elementStyles);
    return widgetAction;
}

QToolButton* MyInteractor::createRemoveElementMenuButton() {
    QToolButton* button = new MyModeToolButton("Remove");
    connect(button, SIGNAL(clicked()), this, SLOT(enableRemoveMode()));
    return button;
}

QToolButton* MyInteractor::createAutoLayoutMenuButton() {
    QToolButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* autoLayoutWidgetAction = new MyWidgetAction(subMenu);
    autoLayoutWidgetAction->setItems((getPluginsOfType(plugins(), "autolayoutengine")));
    connect(autoLayoutWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(autoLayout(MyPluginItemBase*)));
    subMenu->addAction(autoLayoutWidgetAction);
    button->setMenu(subMenu);
    decorateAutoLayoutButton(button);
    return button;
}

QToolButton* MyInteractor::createUndoActionMenuButton() {
    QAction* action = undoStack()->createUndoAction(this, tr("Undo"));
    
    QToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    decorateUndoActionButton(button);
    connect(undoStack(), &MyUndoStack::indexChanged, this, [this, button] () { decorateUndoActionButton(button); });
    connect(undoStack(), &MyUndoStack::canUndoChanged, this, [this, button] () { decorateUndoActionButton(button); });
    connect(undoStack(), &MyUndoStack::canRedoChanged, this, [this, button] () { decorateUndoActionButton(button); });
    return button;
}

QToolButton* MyInteractor::createRedoActionMenuButton() {
    QAction* action = undoStack()->createRedoAction(this, tr("Redo"));
    
    QToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    decorateRedoActionButton(button);
    connect(undoStack(), &MyUndoStack::indexChanged, this, [this, button] () { decorateRedoActionButton(button); });
    connect(undoStack(), &MyUndoStack::canUndoChanged, this, [this, button] () { decorateRedoActionButton(button); });
    connect(undoStack(), &MyUndoStack::canRedoChanged, this, [this, button] () { decorateRedoActionButton(button); });
    return button;
}

QToolButton* MyInteractor::createResetSceneMenuButton() {
    QToolButton* button = new MyToolButton();
    connect(button, SIGNAL(clicked()), this, SLOT(setNewNetworkCanvas()));
    decorateResetSceneButton(button);
    return button;
}

QString getElementUniqueName(QList<MyNetworkElementBase*> elements, const QString& defaultNameSection) {
    QString name;
    qreal k = 0;
    bool isSimilarNameFound = true;
    while(isSimilarNameFound) {
        name = defaultNameSection + "_" + QString::number(k);
        isSimilarNameFound = false;
        for (MyNetworkElementBase *element : qAsConst(elements)) {
            if (element->name() == name) {
                isSimilarNameFound = true;
                break;
            }
        }
        ++k;
    }

    return name;
}

MyNetworkElementBase* findElement(QList<MyNetworkElementBase*> elements, const QString& name) {
    for (MyNetworkElementBase *element : qAsConst(elements)) {
        if (element->name() == name)
            return element;
    }
    
    return NULL;
}

MyNetworkElementBase* findStartNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("start") && json["start"].isObject() && json["start"].toObject().contains("node") && json["start"]["node"].isString())
        return findElement(nodes, json["start"]["node"].toString());
    
    return NULL;
}

MyNetworkElementBase* findEndNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("end") && json["end"].isObject() && json["end"].toObject().contains("node") && json["end"]["node"].isString())
        return findElement(nodes, json["end"]["node"].toString());
    
    return NULL;
}

MyNetworkElementStyleBase* getCopyNodeStyle(const QString& name, MyNetworkElementStyleBase* nodeStyle) {
    QJsonObject styleObject;
    nodeStyle->write(styleObject);
    styleObject["name"] = name;
    return createNodeStyle(styleObject);
}

MyNetworkElementStyleBase* getCopyEdgeStyle(const QString& name, MyNetworkElementStyleBase* edgeStyle) {
    QJsonObject styleObject;
    edgeStyle->write(styleObject);
    styleObject["name"] = name;
    return createEdgeStyle(styleObject);
}