#include "negui_interactor.h"
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
#include "negui_customized_common_widgets.h"


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
    
    loadPlugins();
    resetNetwork();
    _stageInfo = exportNetworkInfo();
    enableNormalMode();
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

void MyInteractor::createChangeStageCommand() {
    QJsonObject currentStageInfo = exportNetworkInfo();
    if (undoStack()->count() > undoStack()->index()) {
        const QUndoCommand* indexCommand = undoStack()->command(undoStack()->index());
        _stageInfo = ((MyChangeStageCommand*)indexCommand)->previousStageInfo();
    }
    if (currentStageInfo != _stageInfo) {
        MyChangeStageCommand* changeStageCommand = new MyChangeStageCommand(_stageInfo, currentStageInfo);
        ((MyUndoStack*)undoStack())->addCommand(changeStageCommand);
        connect(changeStageCommand, SIGNAL(askForCreateNetwork(const QJsonObject&)), this, SLOT(createNetwork(const QJsonObject&)));
        _stageInfo = currentStageInfo;
    }
}

void MyInteractor::setMode(SceneMode mode) {
    _mode = mode;
}

MyInteractor::SceneMode MyInteractor::mode() {
    return _mode;
}

void MyInteractor::createNetwork(const QJsonObject& json) {
    resetNetwork();
    setNetworkExtents(json);
    addNodes(json);
    addEdges(json);
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

QList<MyElementBase*>& MyInteractor::nodes() {
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
    MyElementBase* node = createNode(json);
    if (node)
        addNode(node);
}

void MyInteractor::addNode(MyElementBase* n) {
    if (n && !n->isActive()) {
        _nodes.push_back(n);
        n->setActive(true);
        n->updateGraphicsItem();
        connect(n, SIGNAL(askForParentNodeAtPosition(MyElementBase*, const QPointF&)), this, SLOT(parentNodeAtPosition(MyElementBase*, const QPointF&)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(selectNode(MyElementBase*)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(addNewEdge(MyElementBase*)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(removeItem(MyElementBase*)));
        connect(n, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(n, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
        connect(n->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForClearFocusedGraphicsItems()), this, SLOT(clearElementsFocusedGraphicsItems()));
        emit askForAddGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::addNewNode(const QPointF& position) {
    if (mode() == ADD_NODE_MODE) {
        MyElementBase* node = createNode(getElementUniqueName(nodes(), nodeStyle()->category()), getCopyNodeStyle(getElementUniqueName(nodes(), nodeStyle()->category()) + "_style", nodeStyle()), position.x(), position.y());
        addNode(node);
        createChangeStageCommand();
    }
}

void MyInteractor::removeNode(MyElementBase* n) {
    if (n && n->isActive()) {
        _nodes.removeOne(n);
        n->setActive(false);
        emit askForRemoveGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::updateNodeParents() {
    MyElementBase* parentNode = NULL;
    for (MyElementBase *node : qAsConst(nodes())) {
        parentNode = findElement(nodes(), ((MyNodeBase*)node)->parentNodeId());
        if (parentNode)
            ((MyNodeBase*)node)->setParentNode((MyNodeBase*)parentNode);
    }
}

MyElementBase* MyInteractor::parentNodeAtPosition(MyElementBase* currentNode, const QPointF& position) {
    QList<QGraphicsItem *> items = askForItemsAtPosition(position);
    MyElementBase* parentNode = NULL;
    qint32 parentNodeZValue = INT_MIN;
    for (QGraphicsItem* item : qAsConst(items)) {
        if (item->parentItem()) {
            for (MyElementBase* node : qAsConst(nodes())) {
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

void MyInteractor::setNodeStyle(MyElementStyleBase* style) {
    if (style)
        _nodeStyle = style;
    else
        _nodeStyle = NULL;
}

MyElementStyleBase* MyInteractor::nodeStyle() {
    return _nodeStyle;
}

QList<MyElementBase*>& MyInteractor::edges() {
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
    MyElementBase* edge = createEdge(json, findStartNode(nodes(), json), findEndNode(nodes(), json));
    if (edge)
        addEdge(edge);
}

void MyInteractor::addEdge(MyElementBase* e) {
    if (e && !edgeExists(((MyEdgeBase*)e)->startNode(), ((MyEdgeBase*)e)->endNode()) && e->setActive(true)) {
        _edges.push_back(e);
        e->updateGraphicsItem();
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(selectEdge(MyElementBase*)));
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(removeItem(MyElementBase*)));
        connect(e, SIGNAL(askForCreateChangeStageCommand()), this, SLOT(createChangeStageCommand()));
        connect(e, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
        connect(e->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForClearFocusedGraphicsItems()), this, SLOT(clearElementsFocusedGraphicsItems()));
        emit askForAddGraphicsItem(e->graphicsItem());
        if (((MyEdgeBase*)e)->isSetArrowHead())
            emit askForAddGraphicsItem(((MyEdgeBase*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::addNewEdge(MyElementBase* element) {
    if (mode() == ADD_EDGE_MODE) {
        if (!_newEdgeBuilder) {
            if (edgeStyle()->type() == "templatestyle") {
                _newEdgeBuilder = new MyNewTemplateBuilder(edgeStyle());
                connect((MyNewTemplateBuilder*)_newEdgeBuilder, &MyNewTemplateBuilder::askForAddNode, this, [this] (MyElementBase* node) { this->addNode(node); });
                connect((MyNewTemplateBuilder*)_newEdgeBuilder, &MyNewTemplateBuilder::askForNodeUniqueName, this, [this] (MyElementStyleBase* nodeStyle) { return getElementUniqueName(this->nodes(), nodeStyle->category()); });
            }
            else
                _newEdgeBuilder = new MyNewEdgeBuilder(edgeStyle());
            connect((MyNewEdgeBuilderBase*)_newEdgeBuilder, &MyNewEdgeBuilderBase::askForAddEdge, this, [this] (MyElementBase* edge) { this->addEdge(edge); });
            connect((MyNewEdgeBuilderBase*)_newEdgeBuilder, &MyNewEdgeBuilderBase::askForEdgeUniqueName, this, [this] (MyElementStyleBase* edgeStyle) { return getElementUniqueName(this->edges(), edgeStyle->category()); });
        }
        ((MyNewEdgeBuilderBase*)_newEdgeBuilder)->build(element);
        emit askForSetToolTip(((MyNewEdgeBuilderBase*)_newEdgeBuilder)->toolTipText());
        if (((MyNewEdgeBuilderBase*)_newEdgeBuilder)->isNewEdgeBuilt()) {
            deleteNewEdgeBuilder();
            createChangeStageCommand();
        }
    }
}

void MyInteractor::removeEdge(MyElementBase* e) {
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

void MyInteractor::setEdgeStyle(MyElementStyleBase* style) {
    if (style)
        _edgeStyle = style;
    else
        _edgeStyle = NULL;
}

MyElementStyleBase* MyInteractor::edgeStyle() {
    return _edgeStyle;
}

void MyInteractor::deleteNewEdgeBuilder() {
    for (MyElementBase* selectedNode : selectedNodes())
        selectedNode->setSelected(false);
    if (_newEdgeBuilder)
        _newEdgeBuilder->deleteLater();
    _newEdgeBuilder = NULL;
}

bool MyInteractor::edgeExists(MyElementBase* n1, MyElementBase* n2) {
    for (MyElementBase *edge : qAsConst(edges())) {
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
    for (MyElementBase *node : qAsConst(nodes())) {
        QJsonObject nodeObject;
        node->write(nodeObject);
        nodesArray.append(nodeObject);
    }
    json["nodes"] = nodesArray;
    
    // edges
    QJsonArray edgesArray;
    for (MyElementBase *edge : qAsConst(edges())) {
        QJsonObject edgeObject;
        edge->write(edgeObject);
        edgesArray.append(edgeObject);
    }
    json["edges"] = edgesArray;
    
    return json;
}

void MyInteractor::selectNode(MyElementBase* element) {
    if (mode() == SELECT_NODE_MODE) {
        if (!element->isSelected())
            element->setSelected(true);
        else
            element->setSelected(false);
    }
}

void MyInteractor::selectEdge(MyElementBase* element) {
    if (mode() == SELECT_EDGE_MODE) {
        if (!element->isSelected())
            element->setSelected(true);
        else
            element->setSelected(false);
    }
}

void MyInteractor::removeItem(MyElementBase* element) {
    if (mode() == REMOVE_MODE) {
        if (element->type() == MyElementBase::NODE_ELEMENT) {
            removeNode(element);
            for (MyElementBase *edge : qAsConst(((MyNodeBase*)element)->edges()))
                removeEdge(edge);
        }
        else if (element->type() == MyElementBase::EDGE_ELEMENT) {
            ((MyEdgeBase*)element)->connectToNodes(false);
            removeEdge(element);
        }
        createChangeStageCommand();
    }
}

const QList<MyElementBase*> MyInteractor::selectedNodes() {
    QList<MyElementBase*> selectedNodesList;
    for (MyElementBase *node : qAsConst(nodes())) {
        if (node->isSelected())
            selectedNodesList.push_back(node);
    }
    
    return selectedNodesList;
}

const QList<MyElementBase*> MyInteractor::selectedEdges() {
    QList<MyElementBase*> selectedEdgesList;
    for (MyElementBase *edge : qAsConst(edges())) {
        if (edge->isSelected())
            selectedEdgesList.push_back(edge);
    }
    
    return selectedEdgesList;
}

void MyInteractor::enableNormalMode() {
    setMode(NORMAL_MODE);
    setNodeStyle(NULL);
    setEdgeStyle(NULL);
    deleteNewEdgeBuilder();
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableNormalMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableNormalMode();
    
    emit askForSetToolTip("");
}

void MyInteractor::enableAddNodeMode(MyPluginItemBase* style) {
    enableNormalMode();
    setMode(ADD_NODE_MODE);
    setNodeStyle(dynamic_cast<MyElementStyleBase*>(style));
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableAddNodeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableAddNodeMode();
    
    emit askForSetToolTip(((MyElementStyleBase*)style)->toolTipText());
}

void MyInteractor::enableSelectNodeMode(const QString& nodeCategory) {
    enableNormalMode();
    setMode(SELECT_NODE_MODE);
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableSelectNodeMode();
    
    emit askForSetToolTip("Select " + nodeCategory + " nodes");
}

void MyInteractor::enableAddEdgeMode(MyPluginItemBase* style) {
    enableNormalMode();
    setMode(ADD_EDGE_MODE);
    setEdgeStyle(dynamic_cast<MyElementStyleBase*>(style));
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableAddEdgeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableAddEdgeMode();
    
    emit askForSetToolTip(((MyElementStyleBase*)style)->toolTipText());
}

void MyInteractor::enableSelectEdgeMode(const QString& edgeCategory) {
    enableNormalMode();
    setMode(SELECT_EDGE_MODE);
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableSelectEdgeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();
    
    emit askForSetToolTip("Select " + edgeCategory + " edges");
}

void MyInteractor::enableRemoveMode() {
    enableNormalMode();
    setMode(REMOVE_MODE);
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableRemoveMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableRemoveMode();
    
    emit askForSetToolTip("Select Item");
}

void MyInteractor::clearElementsFocusedGraphicsItems() {
    for (MyElementBase *node : qAsConst(nodes()))
        node->graphicsItem()->clearFocusedGraphicsItems();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->graphicsItem()->clearFocusedGraphicsItems();
}

void MyInteractor::displaySelectionArea(const QPointF& position) {
    if (mode() == NORMAL_MODE) {
        if (!_selectionAreaGraphicsItem) {
            //for (MyElementBase* node : qAsConst(nodes()))
                //node->setSelected(false);
            _selectionAreaGraphicsItem = new MySelectionAreaGraphicsItem(position);
            emit askForAddGraphicsItem(_selectionAreaGraphicsItem);
        }
        ((MySelectionAreaGraphicsItem*)_selectionAreaGraphicsItem)->updateExtents(position);
        setSelectionAreaCoveredNodesSelected();
    }
}

void MyInteractor::setSelectionAreaCoveredNodesSelected() {
    QList<QGraphicsItem *> selectedItems = _selectionAreaGraphicsItem->collidingItems();
    for (MyElementBase* node : qAsConst(nodes())) {
        node->setSelected(false);
        for (QGraphicsItem* item : qAsConst(selectedItems)) {
            if (item->parentItem()) {
                if (node->graphicsItem() == item->parentItem())
                    node->setSelected(true);
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
    QString fileName = ((MyImportTool*)importTool)->getOpenFileName();
    if (!fileName.isEmpty()) {
        createNetwork(importInterface()->readGraphInfoFromFile(fileName, importTool->name()));
        emit askForResetScale();
        createChangeStageCommand();
        enableNormalMode();
    }
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    QJsonObject graphInfoObject = exportNetworkInfo();
    QString fileName = ((MyExportToolBase*)exportTool)->getSaveFileName();
    if (!fileName.isEmpty()) {
        ((MyDataExportTool*)exportTool)->readCompatibilityInfo(dataExportInterface()->checkForGraphInfoCompatibiliy(graphInfoObject, exportTool->name()));
        
        if (((MyDataExportTool*)exportTool)->isInfoCompatible()) {
            dataExportInterface()->writeGraphInfoToFile(graphInfoObject, fileName, exportTool->name());
        }
        ((MyDataExportTool*)exportTool)->showMessages();
    }
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

QList<QToolButton*> MyInteractor::getToolBarMenuButtons() {
    QList<QToolButton*> buttons;
    if (getPluginsOfType(plugins(), "importtool").size())
        buttons.push_back(createImportMenuButton());
    if (getPluginsOfType(plugins(), "dataexporttool").size() || getPluginsOfType(plugins(), "printexporttool").size())
        buttons.push_back(createExportMenuButton());
    if (getPluginsOfType(plugins(), "autolayoutengine").size())
        buttons.push_back(createAutoLayoutMenuButton());
    buttons.push_back(createUndoActionMenuButton());
    buttons.push_back(createRedoActionMenuButton());
    buttons.push_back(createResetSceneMenuButton());
    
    return buttons;
}

QToolButton* MyInteractor::getNormalModeButton() {
    return new MyToolButton();
}

QToolButton* MyInteractor::getSelectModeButton() {
    return new MyToolButton();
}

QList<QToolButton*> MyInteractor::getAddModeButtons() {
    return createAddElementMenuButtons();
}

QToolButton* MyInteractor::getRemoveModeButton() {
    return createRemoveElementMenuButton();
}

QToolButton* MyInteractor::createImportMenuButton() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* importWidgetAction = new MyWidgetAction(subMenu);
    importWidgetAction->setItems(getPluginsOfType(plugins(), "importtool"));
    connect(importWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(readFromFile(MyPluginItemBase*)));
    subMenu->addAction(importWidgetAction);
    button->setText("Import");
    button->setToolTip(tr("Import from a file"));
    button->setMenu(subMenu);
    return button;
}

QToolButton* MyInteractor::createExportMenuButton() {
    QList<MyPluginItemBase*> dataExportPlugins = getPluginsOfType(plugins(), "dataexporttool");
    QList<MyPluginItemBase*> printExportPlugins = getPluginsOfType(plugins(), "printexporttool");
    
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    
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
    
    button->setText("Export");
    button->setToolTip(tr("Export to a file"));
    button->setMenu(subMenu);
    return button;
}

QList<QToolButton*> MyInteractor::createAddElementMenuButtons() {
    QList<QToolButton*> buttons;
    if (!getPluginsOfType(plugins(), "nodestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        _plugins.push_back(createNodeStyle(styleObject));
    }
    if (!getPluginsOfType(plugins(), "edgestyle").size()) {
        QJsonObject styleObject;
        styleObject["name"] = "Default";
        _plugins.push_back(createEdgeStyle(styleObject));
    }
    QList<QString> pluginsCategories = getPluginsCategories(plugins());
    for (QString category : pluginsCategories) {
        QList<MyPluginItemBase*> nodeStylesOfCategory = getPluginsOfCategory(getPluginsOfType(plugins(), "nodestyle"), category);
        QList<MyPluginItemBase*> edgeStylesOfCategory = getPluginsOfCategory(getPluginsOfType(plugins(), "edgestyle"), category);
        QList<MyPluginItemBase*> templateStylesOfCategory = getPluginsOfCategory(getPluginsOfType(plugins(), "templatestyle"), category);
        MyToolButtonMenu* subMenu = new MyToolButtonMenu();
        // node
        if (nodeStylesOfCategory.size())
            subMenu->addAction(createNodeStyleWidgetAction(nodeStylesOfCategory, subMenu));
        if (nodeStylesOfCategory.size() && (edgeStylesOfCategory.size() || templateStylesOfCategory.size()))
            subMenu->addSeparator();
        // edge
        if (edgeStylesOfCategory.size())
            subMenu->addAction(createEdgeStyleWidgetAction(edgeStylesOfCategory, subMenu));
        if (edgeStylesOfCategory.size() && templateStylesOfCategory.size())
            subMenu->addSeparator();
        // template
        if (templateStylesOfCategory.size())
            subMenu->addAction(createEdgeStyleWidgetAction(templateStylesOfCategory, subMenu));
        buttons.push_back(createPluginItemToolButton(subMenu, category));
    }
    
    return buttons;
}

QToolButton* MyInteractor::createPluginItemToolButton(QMenu* subMenu, const QString& text) {
    QToolButton* button = new MyToolButton();
    button->setText(text);
    button->setToolTip("Add " + text + " to the network");
    button->setMenu(subMenu);
    return button;
}


QWidgetAction* MyInteractor::createNodeStyleWidgetAction(QList<MyPluginItemBase*> nodeStyles, QWidget* parent) {
    MyWidgetAction* widgetAction = new MyWidgetAction(parent);
    widgetAction->setItems(nodeStyles);
    connect(widgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddNodeMode(MyPluginItemBase*)));
    return widgetAction;
}

QWidgetAction* MyInteractor::createEdgeStyleWidgetAction(QList<MyPluginItemBase*> edgeStyles, QWidget* parent) {
    MyWidgetAction* widgetAction = new MyWidgetAction(parent);
    widgetAction->setItems(edgeStyles);
    connect(widgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
    return widgetAction;
}

QToolButton* MyInteractor::createRemoveElementMenuButton() {
    MyToolButton* button = new MyToolButton();
    button->setText("Remove");
    button->setToolTip(tr("Remove an item from the network"));
    connect(button, &QToolButton::clicked, this, &MyInteractor::enableRemoveMode);
    return button;
}

QToolButton* MyInteractor::createAutoLayoutMenuButton() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* autoLayoutWidgetAction = new MyWidgetAction(subMenu);
    autoLayoutWidgetAction->setItems((getPluginsOfType(plugins(), "autolayoutengine")));
    connect(autoLayoutWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(autoLayout(MyPluginItemBase*)));
    subMenu->addAction(autoLayoutWidgetAction);
    button->setText("AutoLayout");
    button->setToolTip(tr("Automatically set the position of the nodes and edges"));
    button->setMenu(subMenu);
    return button;
}

QToolButton* MyInteractor::createUndoActionMenuButton() {
    QAction* action = undoStack()->createUndoAction(this, tr("Undo"));
    action->setShortcuts(QKeySequence::Undo);
    
    MyToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    return button;
}

QToolButton* MyInteractor::createRedoActionMenuButton() {
    QAction* action = undoStack()->createRedoAction(this, tr("Redo"));
    action->setShortcuts(QKeySequence::Redo);
    
    MyToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    return button;
}

QToolButton* MyInteractor::createResetSceneMenuButton() {
    MyToolButton* button = new MyToolButton();
    button->setText("Reset");
    button->setToolTip(tr("Remove all network elements from the scene"));
    connect(button, &QToolButton::clicked, this, &MyInteractor::resetNetwork);
    connect(button, &QToolButton::clicked, this, &MyInteractor::askForResetScale);
    return button;
}

// MyUndoStack

MyUndoStack::MyUndoStack(QObject *parent) : QUndoStack(parent) {
    
}

void MyUndoStack::addCommand(QUndoCommand* command) {
    push(command);
}

void MyUndoStack::clear() {
    QUndoStack::clear();
}

// MyWidgetAction

MyWidgetAction::MyWidgetAction(QObject* parent) : QWidgetAction(parent) {
    
}

void MyWidgetAction::setItems(QList<MyPluginItemBase*> items) {
    setDefaultWidget(createItemPreviewWidget(items));
}

QWidget* MyWidgetAction::createItemPreviewWidget(QList<MyPluginItemBase*> items) {
    QWidget* itemWidget = new QWidget();
    QVBoxLayout* itemWidgetLayoutContent = new QVBoxLayout();
    QPushButton* itemPreviewButton = NULL;
    
    QList<QString> itemsSubCategories = getPluginsSubCategories(items);
    if (itemsSubCategories.size()) {
        for (QString subCategory : itemsSubCategories) {
            itemWidgetLayoutContent->addWidget(new MyLabel(subCategory), itemWidgetLayoutContent->count());
            QList<MyPluginItemBase*> itemsOfSubCategory = getPluginsOfSubCategory(items, subCategory);
            for (MyPluginItemBase* itemOfSubCategory : itemsOfSubCategory)
                itemWidgetLayoutContent->addWidget(createItemPreviewButton(itemOfSubCategory), itemWidgetLayoutContent->count());
        }
    }
    else {
        for (MyPluginItemBase* item : items)
            itemWidgetLayoutContent->addWidget(createItemPreviewButton(item), itemWidgetLayoutContent->count());
        
    }
    
    itemWidget->setLayout(itemWidgetLayoutContent);
    itemWidget->setStyleSheet("QWidget { background-color: white; border-radius: 10px;}");
    
    return itemWidget;
}

QPushButton* MyWidgetAction::createItemPreviewButton(MyPluginItemBase* item) {
    QPushButton* itemPreviewButton = new MyItemPreviewButton(item);
    connect(itemPreviewButton, &QPushButton::clicked, this, [this, item] () { emit itemIsChosen(item); ((MyToolButtonMenu*)(this->parent()))->close(); });
    
    return itemPreviewButton;
}

// MyItemPreviewButton

MyItemPreviewButton::MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent) : QPushButton(parent) {
    setCheckable(true);
    setToolTip(item->name());
    
    if (item->icon().isNull()) {
        setStyleSheet("QPushButton { border : no-border; text-align : left} QPushButton:hover { color: darkgray}");
        setText(item->name());
    }
    else {
        setStyleSheet("QPushButton { border : no-border;} QPushButton:hover { background-color: darkgray}");
        setIcon(item->icon());
        setIconSize(item->iconSize());
    }
}

// MyChangeStageCommand

MyChangeStageCommand::MyChangeStageCommand(const QJsonObject& previousStageInfo, const QJsonObject& currentStageInfo, QUndoCommand* parent) : QUndoCommand(parent) {
    _previousStageInfo = previousStageInfo;
    _currentStageInfo = currentStageInfo;
}

void MyChangeStageCommand::undo() {
    emit askForCreateNetwork(_previousStageInfo);
}

void MyChangeStageCommand::redo() {
    emit askForCreateNetwork(_currentStageInfo);
}

const QJsonObject& MyChangeStageCommand::previousStageInfo() {
    return _previousStageInfo;
}

const QJsonObject& MyChangeStageCommand::currentStageInfo() {
    return _currentStageInfo;
}

MyElementBase* findElement(QList<MyElementBase*> elements, const QString& name) {
    for (MyElementBase *element : qAsConst(elements)) {
        if (element->name() == name)
            return element;
    }
    
    return NULL;
}

MyElementBase* findStartNode(QList<MyElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("start") && json["start"].isObject() && json["start"].toObject().contains("node") && json["start"]["node"].isString())
        return findElement(nodes, json["start"]["node"].toString());
    
    return NULL;
}

MyElementBase* findEndNode(QList<MyElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("end") && json["end"].isObject() && json["end"].toObject().contains("node") && json["end"]["node"].isString())
        return findElement(nodes, json["end"]["node"].toString());
    
    return NULL;
}

QString getElementUniqueName(QList<MyElementBase*> elements, const QString& defaultNameSection) {
    QString name;
    qreal k = 0;
    bool isSimilarNameFound = true;
    while(isSimilarNameFound) {
        name = defaultNameSection + "_" + QString::number(k);
        isSimilarNameFound = false;
        for (MyElementBase *element : qAsConst(elements)) {
            if (element->name() == name) {
                isSimilarNameFound = true;
                break;
            }
        }
        ++k;
    }
    
    return name;
}

MyElementStyleBase* getCopyNodeStyle(const QString& name, MyElementStyleBase* nodeStyle) {
    QJsonObject styleObject;
    nodeStyle->write(styleObject);
    styleObject["name"] = name;
    return createNodeStyle(styleObject);
}

MyElementStyleBase* getCopyEdgeStyle(const QString& name, MyElementStyleBase* edgeStyle) {
    QJsonObject styleObject;
    edgeStyle->write(styleObject);
    styleObject["name"] = name;
    return createEdgeStyle(styleObject);
}
