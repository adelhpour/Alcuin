#include "negui_interactor.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_element_builder.h"
#include "negui_element_style_builder.h"
#include "negui_plugin_item_builder.h"
#include "negui_import_tools.h"
#include "negui_autolayout_engines.h"
#include "negui_export_tools.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QJsonArray>

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
    
    // element styles
    _nodeStyle = NULL;
    _edgeStyle = NULL;
    
    // undo stack
    _undoStack = new MyUndoStack(this);
    
    loadPlugins();
    resetNetwork();
};

bool MyInteractor::setImportInterface(ImportInterface* importInterface, const QString &path) {
    if (importInterface) {
        _importInterface = importInterface;
        if (!_importInterface->initialize(path)) {
            readImportInfo(_importInterface->loadItemsInfo());
            _isSetImportInterface = true;
        }
    }
    
    return _isSetImportInterface;
}

ImportInterface* MyInteractor::importInterface() {
    return _importInterface;
}

void MyInteractor::readImportInfo(const QJsonObject &json) {
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyImportTool(toolObject["name"].toString());
                tool->read(toolObject);
                _plugins.push_back(tool);
            }
        }
    }
}

bool MyInteractor::setDataExportInterface(DataExportInterface* dataExportInterface, const QString &path) {
    if (dataExportInterface) {
        _dataExportInterface = dataExportInterface;
        if (!_dataExportInterface->initialize(path)) {
            readDataExportInfo(_dataExportInterface->loadItemsInfo());
            _isSetDataExportInterface = true;
        }
    }
    
    return _isSetDataExportInterface;
}

DataExportInterface* MyInteractor::dataExportInterface() {
    return _dataExportInterface;
}

void MyInteractor::readDataExportInfo(const QJsonObject &json) {
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyDataExportTool(toolObject["name"].toString());
                tool->read(toolObject);
                _plugins.push_back(tool);
            }
        }
    }
}

bool MyInteractor::setPrintExportInterface(PrintExportInterface* printExportInterface, const QString &path) {
    if (printExportInterface) {
        _printExportInterface = printExportInterface;
        if (!_printExportInterface->initialize(path)) {
            readPrintExportInfo(_printExportInterface->loadItemsInfo());
            _isSetPrintExportInterface = true;
        }
    }
    
    return _isSetPrintExportInterface;
}

PrintExportInterface* MyInteractor::printExportInterface() {
    return _printExportInterface;
}

void MyInteractor::readPrintExportInfo(const QJsonObject &json) {
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyPrintExportTool(toolObject["name"].toString());
                tool->read(toolObject);
                _plugins.push_back(tool);
            }
        }
    }
}

QList<MyPluginItemBase*>& MyInteractor::plugins() {
    return _plugins;
}

bool MyInteractor::setElementStyleInterface(ElementStyleInterface* elementStyleInterface, const QString &path) {
    if (elementStyleInterface) {
        _elementStyleInterface = elementStyleInterface;
        if (!_elementStyleInterface->initialize(path)) {
            readElementStylesInfo(_elementStyleInterface->loadItemsInfo());
            _isSetElementStyleInterface = true;
        }
    }
    
    return _isSetElementStyleInterface;
}

ElementStyleInterface* MyInteractor::elementStyleInterface() {
    return _elementStyleInterface;
}

void MyInteractor::readElementStylesInfo(const QJsonObject &json) {
    // element styles
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray stylesArray = json["items"].toArray();
        MyPluginItemBase* style = NULL;
        for (int styleIndex = 0; styleIndex < stylesArray.size(); ++styleIndex) {
            QJsonObject styleObject = stylesArray[styleIndex].toObject();
            if (styleObject.contains("name") && styleObject["name"].isString() && styleObject.contains("type") && styleObject["type"].isString()) {
                MyPluginItemBase* style = createPluginItem(styleObject["name"].toString(), styleObject["type"].toString());
                if (style) {
                    style->read(styleObject);
                    _plugins.push_back(style);
                }
            }
        }
    }
}

bool MyInteractor::setAutoLayoutInterface(AutoLayoutInterface* autoLayoutInterface, const QString &path) {
    if (autoLayoutInterface) {
        _autoLayoutInterface = autoLayoutInterface;
        if (!_autoLayoutInterface->initialize(path)) {
            readAutoLayoutInfo(_autoLayoutInterface->loadItemsInfo());
            _isSetAutoLayoutInterface = true;
        }
    }
    
    return _isSetAutoLayoutInterface;
}

AutoLayoutInterface* MyInteractor::autoLayoutInterface() {
    return _autoLayoutInterface;
}

void MyInteractor::readAutoLayoutInfo(const QJsonObject &json) {
    // engines
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray enginesArray = json["items"].toArray();
        MyPluginItemBase* engine = NULL;
        for (int engineIndex = 0; engineIndex < enginesArray.size(); ++engineIndex) {
            QJsonObject engineObject = enginesArray[engineIndex].toObject();
            if (engineObject.contains("name") && engineObject["name"].isString()) {
                engine = new MyAutoLayoutEngine(engineObject["name"].toString());
                engine->read(engineObject);
                _plugins.push_back(engine);
            }
        }
    }
}

QUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

void MyInteractor::setMode(SceneMode mode) {
    _mode = mode;
    unSetSelectedEdgeStartNode();
}

MyInteractor::SceneMode MyInteractor::mode() {
    return _mode;
}

void MyInteractor::createNetwork(const QJsonObject &json) {
    resetNetwork();
    setNetworkExtents(json);
    addNodes(json);
    addEdges(json);
}

void MyInteractor::resetNetwork() {
    _selectedEdgeStartNode = NULL;
    _isSetSelectedEdgeStartNode = false;
    enableNormalMode();
    clearNodesInfo();
    clearEdgesInfo();
    askForClearScene();
    setNetworkExtents(30.0, 20.0, 840.0, 560.0);
}

void MyInteractor::setNetworkExtents(const QJsonObject& json) {
    if (json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble() && json.contains("dimensions") && json["dimensions"].isObject() && json["dimensions"].toObject().contains("width") && json["dimensions"]["width"].isDouble() && json["dimensions"].toObject().contains("height") && json["dimensions"]["height"].isDouble())
        setNetworkExtents(json["position"]["x"].toDouble(), json["position"]["y"].toDouble(), json["dimensions"]["width"].toDouble(), json["dimensions"]["height"].toDouble());
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
        updateNodeParetns();
    }
}

void MyInteractor::addNode(const QJsonObject &json) {
    MyElementBase* node = createNode(json);
    if (node) {
        MyElementStyleBase* style = createNodeStyle(json);
        if (!style)
            style = getCopyNodeStyle(node->name() + "_style");
        node->setStyle(style);
        QUndoCommand *addNodeCommand = new MyAddNodeCommand(this, node);
        ((MyUndoStack*)undoStack())->addCommand(addNodeCommand);
    }
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
        connect(n->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        emit askForAddGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::addNewNode(const QPointF& position) {
    if (mode() == ADD_NODE_MODE) {
        MyElementBase* _node = createNode(getElementUniqueId(nodes(), nodeStyle()->category()), position.x(), position.y());
        _node->setStyle(getCopyNodeStyle(_node->name() + "_style"));
        QUndoCommand *addNodeCommand = new MyAddNodeCommand(this, _node);
        ((MyUndoStack*)undoStack())->addCommand(addNodeCommand);
    }
}

void MyInteractor::removeNode(MyElementBase* n) {
    if (n && n->isActive()) {
        _nodes.removeOne(n);
        n->setActive(false);
        emit askForRemoveGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::updateNodeParetns() {
    MyElementBase* parentNode = NULL;
    for (MyElementBase *node : qAsConst(nodes())) {
        parentNode = findElement(nodes(), ((MyNode*)node)->parentNodeId());
        if (parentNode)
            ((MyNode*)node)->setParentNode((MyNode*)parentNode);
    }
    
    for (MyElementBase *node : qAsConst(nodes())) {
        if (((MyNode*)node)->childNodes().size()) {
            ((MyNode*)node)->adjustExtentsTochildren();
        }
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
}

MyElementStyleBase* MyInteractor::nodeStyle() {
    return _nodeStyle;
}

MyElementStyleBase* MyInteractor::getCopyNodeStyle(const QString& name) {
    QJsonObject styleObject;
    nodeStyle()->write(styleObject);
    MyElementStyleBase* style = createNodeStyle(name);
    style->read(styleObject);
    return style;
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
    if (edge) {
        MyElementStyleBase* style = createEdgeStyle(json);
        if (!style)
            style = getCopyEdgeStyle(edge->name() + "_style");
        edge->setStyle(style);
        QUndoCommand *addEdgeCommand = new MyAddEdgeCommand(this, edge);
        ((MyUndoStack*)undoStack())->addCommand(addEdgeCommand);
    }
}

void MyInteractor::addEdge(MyElementBase* e) {
    if (e && !edgeExists(((MyEdge*)e)->startNode(), ((MyEdge*)e)->endNode()) && e->setActive(true)) {
        _edges.push_back(e);
        e->updateGraphicsItem();
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(selectEdge(MyElementBase*)));
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(removeItem(MyElementBase*)));
        emit askForAddGraphicsItem(e->graphicsItem());
        if (((MyEdge*)e)->isSetArrowHead())
            emit askForAddGraphicsItem(((MyEdge*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::addNewEdge(MyElementBase* element) {
    if (mode() == ADD_EDGE_MODE) {
        if (!isSetSelectedEdgeStartNode()) {
            setSelectedEdgeStartNode(element);
        }
        else if (selectedEdgeStartNode() != element && !edgeExists(selectedEdgeStartNode(), element)) {
            MyElementBase* _edge = createEdge(getElementUniqueId(edges(), edgeStyle()->category()), selectedEdgeStartNode(), element);
            _edge->setStyle(getCopyEdgeStyle(_edge->name() + "_style"));
            QUndoCommand *addEdgeCommand = new MyAddEdgeCommand(this, _edge);
            ((MyUndoStack*)undoStack())->addCommand(addEdgeCommand);
            unSetSelectedEdgeStartNode();
        }
    }
}

void MyInteractor::removeEdge(MyElementBase* e) {
    if (e && e->isActive()) {
        e->setActive(false);
        _edges.removeOne(e);
        emit askForRemoveGraphicsItem(e->graphicsItem());
        if (((MyEdge*)e)->isSetArrowHead())
            emit askForRemoveGraphicsItem(((MyEdge*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::clearEdgesInfo() {
    while(edges().size())
        delete edges().takeLast();
}

void MyInteractor::setEdgeStyle(MyElementStyleBase* style) {
    if (style)
        _edgeStyle = style;
}

MyElementStyleBase* MyInteractor::edgeStyle() {
    return _edgeStyle;
}

MyElementStyleBase* MyInteractor::getCopyEdgeStyle(const QString& name) {
    QJsonObject styleObject;
    edgeStyle()->write(styleObject);
    MyElementStyleBase* style = createEdgeStyle(name);
    style->read(styleObject);
    return style;
}

void MyInteractor::setSelectedEdgeStartNode(MyElementBase* n) {
    if (n) {
        _selectedEdgeStartNode = n;
        _isSetSelectedEdgeStartNode = true;
        selectedEdgeStartNode()->setSelected(true);
    }
}

void MyInteractor::unSetSelectedEdgeStartNode() {
    if (selectedEdgeStartNode())
        selectedEdgeStartNode()->setSelected(false);
    _selectedEdgeStartNode = NULL;
    _isSetSelectedEdgeStartNode = false;
}

MyElementBase* MyInteractor::selectedEdgeStartNode() {
    return _selectedEdgeStartNode;
}

bool MyInteractor::edgeExists(MyElementBase* n1, MyElementBase* n2) {
    for (MyElementBase *edge : qAsConst(edges())) {
        if ((((MyEdge*)edge)->startNode() == n1 && ((MyEdge*)edge)->endNode() == n2) || (((MyEdge*)edge)->startNode() == n2 && ((MyEdge*)edge)->endNode() == n1)) {
            unSetSelectedEdgeStartNode();
            return true;
        }
    }
    
    return false;
}

void MyInteractor::exportNetworkInfo(QJsonObject &json) {
    // position
    QJsonObject positionObject;
    positionObject["x"] = networkExtents().x();
    positionObject["y"] = networkExtents().y();
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
        QUndoCommand *removeCommand = NULL;
        if (element->type() == MyElementBase::NODE_ELEMENT)
            removeCommand = new MyRemoveNodeCommand(this, element);
        else if (element->type() == MyElementBase::EDGE_ELEMENT)
            removeCommand = new MyRemoveEdgeCommand(this, element);
        
        if (removeCommand)
            ((MyUndoStack*)undoStack())->addCommand(removeCommand);
    }
}

const QList<QString> MyInteractor::selectedNodes() {
    QList<QString> selectedNodesNames;
    for (MyElementBase *node : qAsConst(nodes())) {
        if (node->graphicsItem()->isSelected())
            selectedNodesNames.push_back(node->name());
    }
    
    return selectedNodesNames;
}

const QList<QString> MyInteractor::selectedEdges() {
    QList<QString> selectedEdgesNames;
    for (MyElementBase *edge : qAsConst(edges())) {
        if (edge->graphicsItem()->isSelected())
            selectedEdgesNames.push_back(edge->name());
    }
    
    return selectedEdgesNames;
}

void MyInteractor::enableNormalMode() {
    setMode(NORMAL_MODE);
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
    
    emit askForSetToolTip("Add " + style->category());
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
    
    emit askForSetToolTip("Select Node");
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

void MyInteractor::readFromFile(MyPluginItemBase* importTool) {
    QString fileName = ((MyImportTool*)importTool)->getOpenFileName();
    if (!fileName.isEmpty())
        createNetwork(importInterface()->readGraphInfoFromFile(fileName, importTool->name()));
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    QJsonObject graphInfoObject;
    exportNetworkInfo(graphInfoObject);
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
        QJsonObject graphInfoObject;
        exportNetworkInfo(graphInfoObject);
        autoLayoutInterface()->autoLayout(graphInfoObject, autoLayoutInfoObject);
        createNetwork(graphInfoObject);
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

QList<QToolButton*> MyInteractor::getMenuButtons() {
    QList<QToolButton*> buttons;
    if (getPluginsOfType(plugins(), "importtool").size())
        buttons.push_back(populateImportMenu());
    if (getPluginsOfType(plugins(), "dataexporttool").size() || getPluginsOfType(plugins(), "printexporttool").size())
        buttons.push_back(populateExportMenu());
    buttons.append(populateAddElementMenu());
    buttons.push_back(populateRemoveItemMenu());
    if (getPluginsOfType(plugins(), "autolayoutengine").size())
        buttons.push_back(populateAutoLayoutMenu());
    buttons.push_back(populateUndoActionMenu());
    buttons.push_back(populateRedoActionMenu());
    buttons.push_back(populateResetSceneMenu());
    
    return buttons;
}

QToolButton* MyInteractor::populateImportMenu() {
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

QToolButton* MyInteractor::populateExportMenu() {
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

QList<QToolButton*> MyInteractor::populateAddElementMenu() {
    QList<QToolButton*> buttons;
    if (!getPluginsOfType(plugins(), "nodestyle").size())
        _plugins.push_back(createNodeStyle("Default"));
    if (!getPluginsOfType(plugins(), "edgestyle").size())
        _plugins.push_back(createEdgeStyle("Default"));
    QList<QString> pluginsCategories = getPluginsCategories(plugins());
    for (QString category : pluginsCategories) {
        QList<MyPluginItemBase*> nodeStylesOfCategory = getPluginsOfCategory(getPluginsOfType(plugins(), "nodestyle"), category);
        QList<MyPluginItemBase*> edgeStylesOfCategory = getPluginsOfCategory(getPluginsOfType(plugins(), "edgestyle"), category);
        MyToolButtonMenu* subMenu = new MyToolButtonMenu();
        // node
        if (nodeStylesOfCategory.size())
            subMenu->addAction(createNodeStyleWidgetAction(nodeStylesOfCategory, subMenu));
        if (nodeStylesOfCategory.size() && edgeStylesOfCategory.size())
            subMenu->addSeparator();
        // edge
        if (edgeStylesOfCategory.size())
            subMenu->addAction(createEdgeStyleWidgetAction(edgeStylesOfCategory, subMenu));
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

QToolButton* MyInteractor::populateRemoveItemMenu() {
    MyToolButton* button = new MyToolButton();
    button->setText("Remove");
    button->setToolTip(tr("Remove an item from the network"));
    connect(button, &QToolButton::clicked, this, &MyInteractor::enableRemoveMode);
    return button;
}

QToolButton* MyInteractor::populateAutoLayoutMenu() {
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

QToolButton* MyInteractor::populateUndoActionMenu() {
    QAction* action = undoStack()->createUndoAction(this, tr("Undo"));
    action->setShortcuts(QKeySequence::Undo);
    
    MyToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    return button;
}

QToolButton* MyInteractor::populateRedoActionMenu() {
    QAction* action = undoStack()->createRedoAction(this, tr("Redo"));
    action->setShortcuts(QKeySequence::Redo);
    
    MyToolButton* button = new MyToolButton();
    button->setDefaultAction(action);
    return button;
}

QToolButton* MyInteractor::populateResetSceneMenu() {
    MyToolButton* button = new MyToolButton();
    button->setText("Reset");
    button->setToolTip(tr("Remove all network elements from the scene"));
    connect(button, &QToolButton::clicked, this, &MyInteractor::resetNetwork);
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

// MyToolButton

MyToolButton::MyToolButton(QWidget* parent) : QToolButton(parent) {
    setPopupMode(QToolButton::InstantPopup);
    setStyleSheet("QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

// MyToolButtonMenu

MyToolButtonMenu::MyToolButtonMenu(QWidget* parent) : QMenu(parent) {
    setStyleSheet("QMenu { background-color: white; border-radius: 10px;} ");
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


// MyAddNodeCommand

MyAddNodeCommand::MyAddNodeCommand(MyInteractor* interactor, MyElementBase* node, QUndoCommand* parent) : QUndoCommand(parent) {
    _interactor = interactor;
    _node = node;
}

void MyAddNodeCommand::undo() {
    _interactor->removeNode(_node);
    _interactor->enableNormalMode();
}

void MyAddNodeCommand::redo() {
    _interactor->addNode(_node);
}

// MyRemoveNodeCommand

MyRemoveNodeCommand::MyRemoveNodeCommand(MyInteractor* interactor, MyElementBase* node, QUndoCommand* parent) : QUndoCommand(parent) {
    _interactor = interactor;
    _node = node;
}

void MyRemoveNodeCommand::undo() {
    _interactor->addNode(_node);
    for (MyElementBase *edge : qAsConst(((MyNode*)_node)->edges())) {
        if (((MyEdge*)edge)->isConnectedToNodes())
            _interactor->addEdge(edge);
    }
    _interactor->enableNormalMode();
}

void MyRemoveNodeCommand::redo() {
    _interactor->removeNode(_node);
    for (MyElementBase *edge : qAsConst(((MyNode*)_node)->edges()))
        _interactor->removeEdge(edge);
}

// MyAddEdgeCommand

MyAddEdgeCommand::MyAddEdgeCommand(MyInteractor* interactor, MyElementBase* edge, QUndoCommand* parent) : QUndoCommand(parent) {
    _interactor = interactor;
    _edge = edge;
}

void MyAddEdgeCommand::undo() {
    ((MyEdge*)_edge)->connectToNodes(false);
    _interactor->removeEdge(_edge);
    _interactor->enableNormalMode();
}

void MyAddEdgeCommand::redo() {
    ((MyEdge*)_edge)->connectToNodes(true);
    _interactor->addEdge(_edge);
}

// MyRemoveEdgeCommand

MyRemoveEdgeCommand::MyRemoveEdgeCommand(MyInteractor* interactor, MyElementBase* edge, QUndoCommand* parent) : QUndoCommand(parent) {
    _interactor = interactor;
    _edge = edge;
}

void MyRemoveEdgeCommand::undo() {
    ((MyEdge*)_edge)->connectToNodes(true);
    _interactor->addEdge(_edge);
    _interactor->enableNormalMode();
}

void MyRemoveEdgeCommand::redo() {
    ((MyEdge*)_edge)->connectToNodes(false);
    _interactor->removeEdge(_edge);
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

QString getElementUniqueId(QList<MyElementBase*> elements, const QString& defaultIdSection) {
    QString name;
    qreal k = 0;
    bool isSimilarIdFound = true;
    while(isSimilarIdFound) {
        name = defaultIdSection + "_" + QString::number(k);
        isSimilarIdFound = false;
        for (MyElementBase *element : qAsConst(elements)) {
            if (element->name() == name) {
                isSimilarIdFound = true;
                break;
            }
        }
        ++k;
    }
    
    return name;
}
