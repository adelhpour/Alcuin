#include "negui_interactor.h"

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
    
    // node style interface
    _nodeStyleInterface = NULL;
    _isSetNodeStyleInterface = false;
    _nodeStyle = NULL;
    _nodeStyles.push_back(new MyNodeStyle("Default"));
    
    // edge style interface
    _edgeStyleInterface = NULL;
    _isSetEdgeStyleInterface = false;
    _edgeStyle = NULL;
    _edgeStyles.push_back(new MyEdgeStyle("Default"));
    
    // autolayout interface
    _autoLayoutInterface = NULL;
    _isSetAutoLayoutInterface = false;
    
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

QList<MyPluginItemBase*>& MyInteractor::importTools() {
    return _importTools;
}

void MyInteractor::readImportInfo(const QJsonObject &json) {
    clearImportInfo();
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyImportTool(toolObject["name"].toString());
                tool->read(toolObject);
                _importTools.push_back(tool);
            }
        }
    }
}

void MyInteractor::clearImportInfo() {
    while(_importTools.size())
        delete (MyImportTool*)_importTools.takeLast();
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

QList<MyPluginItemBase*>& MyInteractor::dataExportTools() {
    return _dataExportTools;
}

void MyInteractor::readDataExportInfo(const QJsonObject &json) {
    clearDataExportInfo();
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyDataExportTool(toolObject["name"].toString());
                connect((MyDataExportTool*)tool, SIGNAL(elementSelectionStarted()), this, SLOT(enableNormalMode()));
                connect((MyDataExportTool*)tool, SIGNAL(askForNodeSelection(const QString&)), this, SLOT(enableSelectNodeMode(const QString&)));
                connect((MyDataExportTool*)tool, SIGNAL(askForSelectedNodes()), this, SLOT(selectedNodes()));
                connect((MyDataExportTool*)tool, SIGNAL(askForEdgeSelection(const QString&)), this, SLOT(enableSelectEdgeMode(const QString&)));
                connect((MyDataExportTool*)tool, SIGNAL(askForSelectedEdges()), this, SLOT(selectedEdges()));
                connect((MyDataExportTool*)tool, SIGNAL(elementSelectionFinished(MyPluginItemBase*)), this, SLOT(writeDataToFile(MyPluginItemBase*)));
                tool->read(toolObject);
                _dataExportTools.push_back(tool);
            }
        }
    }
}

void MyInteractor::clearDataExportInfo() {
    while(_dataExportTools.size())
        delete (MyDataExportTool*)_dataExportTools.takeLast();
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

QList<MyPluginItemBase*>& MyInteractor::printExportTools() {
    return _printExportTools;
}

void MyInteractor::readPrintExportInfo(const QJsonObject &json) {
    clearPrintExportInfo();
    // tools
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray toolsArray = json["items"].toArray();
        MyPluginItemBase* tool = NULL;
        for (int toolIndex = 0; toolIndex < toolsArray.size(); ++toolIndex) {
            QJsonObject toolObject = toolsArray[toolIndex].toObject();
            if (toolObject.contains("name") && toolObject["name"].isString()) {
                tool = new MyPrintExportTool(toolObject["name"].toString());
                tool->read(toolObject);
                _printExportTools.push_back(tool);
            }
        }
    }
}

void MyInteractor::clearPrintExportInfo() {
    while(_printExportTools.size())
        delete (MyPrintExportTool*)_printExportTools.takeLast();
}

bool MyInteractor::setNodeStyleInterface(NodeStyleInterface* nodeStyleInterface, const QString &path) {
    if (nodeStyleInterface) {
        _nodeStyleInterface = nodeStyleInterface;
        if (!_nodeStyleInterface->initialize(path)) {
            readNodeStylesInfo(_nodeStyleInterface->loadItemsInfo());
            _isSetNodeStyleInterface = true;
        }
    }
    
    return _isSetNodeStyleInterface;
}

NodeStyleInterface* MyInteractor::nodeStyleInterface() {
    return _nodeStyleInterface;
}

QList<MyPluginItemBase*>& MyInteractor::nodeStyles() {
    return _nodeStyles;
}

void MyInteractor::readNodeStylesInfo(const QJsonObject &json) {
    clearNodeStylesInfo();
    // node styles
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray stylesArray = json["items"].toArray();
        MyPluginItemBase* style = NULL;
        for (int styleIndex = 0; styleIndex < stylesArray.size(); ++styleIndex) {
            QJsonObject styleObject = stylesArray[styleIndex].toObject();
            if (styleObject.contains("name") && styleObject["name"].isString()) {
                style = new MyNodeStyle(styleObject["name"].toString());
                style->read(styleObject);
                _nodeStyles.push_back(style);
            }
        }
    }
}

void MyInteractor::clearNodeStylesInfo() {
    while(_nodeStyles.size() > 1)
        delete (MyElementStyleBase*)_nodeStyles.takeLast();
}

bool MyInteractor::setEdgeStyleInterface(EdgeStyleInterface* edgeStyleInterface, const QString &path) {
    if (edgeStyleInterface) {
        _edgeStyleInterface = edgeStyleInterface;
        if (!_edgeStyleInterface->initialize(path)) {
            readEdgeStylesInfo(_edgeStyleInterface->loadItemsInfo());
            _isSetEdgeStyleInterface = true;
        }
    }
    
    return _isSetEdgeStyleInterface;
}

EdgeStyleInterface* MyInteractor::edgeStyleInterface() {
    return _edgeStyleInterface;
}

QList<MyPluginItemBase*>& MyInteractor::edgeStyles() {
    return _edgeStyles;
}

void MyInteractor::readEdgeStylesInfo(const QJsonObject &json) {
    clearEdgeStylesInfo();
    // edge styles
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray stylesArray = json["items"].toArray();
        MyPluginItemBase* style = NULL;
        for (int styleIndex = 0; styleIndex < stylesArray.size(); ++styleIndex) {
            QJsonObject styleObject = stylesArray[styleIndex].toObject();
            if (styleObject.contains("name") && styleObject["name"].isString()) {
                style = new MyEdgeStyle(styleObject["name"].toString());
                style->read(styleObject);
                _edgeStyles.push_back(style);
            }
        }
    }
}

void MyInteractor::clearEdgeStylesInfo() {
    while(_edgeStyles.size() > 1)
        delete (MyElementStyleBase*)_edgeStyles.takeLast();
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

QList<MyPluginItemBase*>& MyInteractor::autoLayoutEngines() {
    return _autoLayoutEngines;
}

void MyInteractor::readAutoLayoutInfo(const QJsonObject &json) {
    clearAutoLayoutInfo();
    // engines
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray enginesArray = json["items"].toArray();
        MyPluginItemBase* engine = NULL;
        for (int engineIndex = 0; engineIndex < enginesArray.size(); ++engineIndex) {
            QJsonObject engineObject = enginesArray[engineIndex].toObject();
            if (engineObject.contains("name") && engineObject["name"].isString()) {
                engine = new MyAutoLayoutEngine(engineObject["name"].toString());
                engine->read(engineObject);
                _autoLayoutEngines.push_back(engine);
            }
        }
    }
}

void MyInteractor::clearAutoLayoutInfo() {
    while(_autoLayoutEngines.size())
        delete (MyAutoLayoutEngine*)_autoLayoutEngines.takeLast();
}

MyUndoStack* MyInteractor::undoStack() {
    return _undoStack;
}

void MyInteractor::setMode(SceneMode mode) {
    _mode = mode;
    unSetSelectedEdgeStartNode();
}

MyInteractor::SceneMode MyInteractor::mode() {
    return _mode;
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

QString MyInteractor::getNodeUniqueId() {
    QString name;
    qreal k = 0;
    bool isSimilarIdFound = true;
    while(isSimilarIdFound) {
        name = "Node_" + QString::number(k);
        isSimilarIdFound = false;
        for (MyElementBase *node : qAsConst(nodes())) {
            if (node->name() == name) {
                isSimilarIdFound = true;
                break;
            }
        }
        ++k;
    }
    
    return name;
}

MyElementBase* MyInteractor::findNode(const QString& name) {
    for (MyElementBase *node : qAsConst(nodes())) {
        if (node->name() == name)
            return node;
    }
    
    return NULL;
}

void MyInteractor::addNode(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isString() && json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
        MyElementBase* _node = new MyNode(json["id"].toString(), json["position"]["x"].toDouble(), json["position"]["y"].toDouble());
        if (json.contains("parent") && json["parent"].isString())
           ((MyNode*)_node)->setParentNodeId(json["parent"].toString());
        MyElementStyleBase* _style = NULL;
        if (json.contains("style") && json["style"].isObject() && json["style"].toObject().contains("name") && json["style"]["name"].isString()) {
            _style = new MyNodeStyle(json["style"]["name"].toString());
            _style->read(json["style"].toObject());
        }
        if (!_style)
            _style = getCopyNodeStyle(_node->name() + "_style");
        _node->setStyle(_style);
        QUndoCommand *addNodeCommand = new MyAddNodeCommand(this, _node);
        undoStack()->addCommand(addNodeCommand);
    }
}

void MyInteractor::addNode(MyElementBase* n) {
    if (n && !((MyNode*)n)->isActive()) {
        _nodes.push_back(n);
        ((MyNode*)n)->setActive(true);
        connect(n, SIGNAL(askForParentNodeAtPosition(MyElementBase*, const QPointF&)), this, SLOT(parentNodeAtPosition(MyElementBase*, const QPointF&)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(selectNode(MyElementBase*)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(addNewEdge(MyElementBase*)));
        connect(n, SIGNAL(elementObject(MyElementBase*)), this, SLOT(removeItem(MyElementBase*)));
        emit askForAddGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::removeNode(MyElementBase* n) {
    if (n && ((MyNode*)n)->isActive()) {
        _nodes.removeOne(n);
        ((MyNode*)n)->setActive(false);
        emit askForRemoveGraphicsItem(n->graphicsItem());
    }
}

void MyInteractor::updateNodeParetns() {
    MyElementBase* parentNode = NULL;
    for (MyElementBase *node : qAsConst(nodes())) {
        parentNode = findNode(((MyNode*)node)->parentNodeId());
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
    MyElementStyleBase* style = new MyNodeStyle(name);
    style->read(styleObject);
    return style;
}

QList<MyElementBase*>& MyInteractor::edges() {
    return _edges;
}

QString MyInteractor::getEdgeUniqueId() {
    QString name;
    qreal k = 0;
    bool isSimilarIdFound = true;
    while(isSimilarIdFound) {
        name = "Edge_" + QString::number(k);
        isSimilarIdFound = false;
        for (MyElementBase *edge : qAsConst(edges())) {
            if (edge->name() == name) {
                isSimilarIdFound = true;
                break;
            }
        }
        ++k;
    }
    
    return name;
}

MyElementBase* MyInteractor::findEdge(const QString& name) {
    for (MyElementBase *edge : qAsConst(edges())) {
        if (edge->name() == name)
            return edge;
    }
    
    return NULL;
}

void MyInteractor::addEdge(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isString() && json.contains("start") && json["start"].isObject() && json["start"].toObject().contains("node") && json["start"]["node"].isString() && findNode(json["start"]["node"].toString()) && json.contains("end") && json["end"].isObject() && json["end"].toObject().contains("node") && json["end"]["node"].isString() && findNode(json["end"]["node"].toString())) {
        MyElementBase* _edge = new MyEdge(json["id"].toString(), (MyNode*)findNode(json["start"]["node"].toString()), (MyNode*)findNode(json["end"]["node"].toString()));
        MyElementStyleBase* _style = NULL;
        if (json.contains("style") && json["style"].isObject() && json["style"].toObject().contains("name") && json["style"]["name"].isString()) {
            _style = new MyEdgeStyle(json["style"]["name"].toString());
            _style->read(json["style"].toObject());
        }
        if (!_style)
            _style = getCopyEdgeStyle(_edge->name() + "_style");
        _edge->setStyle(_style);
        QUndoCommand *addEdgeCommand = new MyAddEdgeCommand(this, _edge);
        undoStack()->addCommand(addEdgeCommand);
    }
}

void MyInteractor::addEdge(MyElementBase* e) {
    if (e && !edgeExists(((MyEdge*)e)->startNode(), ((MyEdge*)e)->endNode()) && ((MyEdge*)e)->setActive(true)) {
        _edges.push_back(e);
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(selectEdge(MyElementBase*)));
        connect(e, SIGNAL(elementObject(MyElementBase*)), this, SLOT(removeItem(MyElementBase*)));
        emit askForAddGraphicsItem(e->graphicsItem());
        if (((MyEdge*)e)->isSetArrowHead())
            emit askForAddGraphicsItem(((MyEdge*)e)->arrowHead()->graphicsItem());
    }
}

void MyInteractor::removeEdge(MyElementBase* e) {
    if (e && ((MyEdge*)e)->isActive()) {
        ((MyEdge*)e)->setActive(false);
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
    MyElementStyleBase* style = new MyEdgeStyle(name);
    style->read(styleObject);
    return style;
}

void MyInteractor::setSelectedEdgeStartNode(MyElementBase* n) {
    if (n) {
        _selectedEdgeStartNode = n;
        _isSetSelectedEdgeStartNode = true;
        ((MyNode*)selectedEdgeStartNode())->setSelected(true);
    }
}

void MyInteractor::unSetSelectedEdgeStartNode() {
    if (selectedEdgeStartNode())
        ((MyNode*)selectedEdgeStartNode())->setSelected(false);
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

QList<QToolButton*> MyInteractor::getMenuButtons() {
    QList<QToolButton*> buttons;
    
    if (!importTools().isEmpty())
        buttons.push_back(populateImportMenu());
    if (dataExportTools().size() || printExportTools().size())
        buttons.push_back(populateExportMenu());
    buttons.push_back(populateAddNodeMenu());
    buttons.push_back(populateAddEdgeMenu());
    buttons.push_back(populateRemoveItemMenu());
    if (!autoLayoutEngines().isEmpty())
        buttons.push_back(populateAutoLayoutMenu());
    buttons.push_back(populateUndoActionMenu());
    buttons.push_back(populateRedoActionMenu());
    buttons.push_back(populateResetSceneMenu());
    
    return buttons;
}

void MyInteractor::createNetwork(const QJsonObject &json) {
    resetNetwork();
    
    // position and dimensions
    if (json.contains("position") && json["position"].isObject() && json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble() && json.contains("dimensions") && json["dimensions"].isObject() && json["dimensions"].toObject().contains("width") && json["dimensions"]["width"].isDouble() && json["dimensions"].toObject().contains("height") && json["dimensions"]["height"].isDouble())
        setNetworkExtents(json["position"]["x"].toDouble(), json["position"]["y"].toDouble(), json["dimensions"]["width"].toDouble(), json["dimensions"]["height"].toDouble());
    
    // nodes
    if (json.contains("nodes") && json["nodes"].isArray()) {
        QJsonArray nodesArray = json["nodes"].toArray();
        for (int nodeIndex = 0; nodeIndex < nodesArray.size(); ++nodeIndex)
            addNode(nodesArray[nodeIndex].toObject());
        updateNodeParetns();
    }
    
    // edges
    if (json.contains("edges") && json["edges"].isArray()) {
        QJsonArray edgesArray = json["edges"].toArray();
        for (int edgeIndex = 0; edgeIndex < edgesArray.size(); ++edgeIndex)
            addEdge(edgesArray[edgeIndex].toObject());
    }
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

void MyInteractor::addNewNode(const QPointF& position) {
    if (mode() == ADD_NODE_MODE) {
        MyElementBase* _node = new MyNode(getNodeUniqueId(), position.x(), position.y());
        _node->setStyle(getCopyNodeStyle(_node->name() + "_style"));
        QUndoCommand *addNodeCommand = new MyAddNodeCommand(this, _node);
        undoStack()->addCommand(addNodeCommand);
    }
    else
        enableNormalMode();
}

void MyInteractor::selectNode(MyElementBase* element) {
    if (mode() == SELECT_NODE_MODE) {
        if (!element->isSelected())
            element->setSelected(true);
        else
            element->setSelected(false);
    }
}

void MyInteractor::addNewEdge(MyElementBase* element) {
    if (mode() == ADD_EDGE_MODE) {
        if (!isSetSelectedEdgeStartNode()) {
            setSelectedEdgeStartNode(element);
        }
        else if (selectedEdgeStartNode() != element && !edgeExists(selectedEdgeStartNode(), element)) {
            MyElementBase* _edge = new MyEdge(getEdgeUniqueId(), (MyNode*)selectedEdgeStartNode(), (MyNode*)element);
            _edge->setStyle(getCopyEdgeStyle(_edge->name() + "_style"));
            QUndoCommand *addEdgeCommand = new MyAddEdgeCommand(this, _edge);
            undoStack()->addCommand(addEdgeCommand);
            unSetSelectedEdgeStartNode();
        }
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
            undoStack()->addCommand(removeCommand);
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
    
    emit askForSetToolTip("Add Node");
}

void MyInteractor::enableSelectNodeMode(const QString& nodeType) {
    enableNormalMode();
    setMode(SELECT_NODE_MODE);
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableSelectNodeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableSelectNodeMode();
    
    emit askForSetToolTip("Select " + nodeType + " nodes");
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

void MyInteractor::enableSelectEdgeMode(const QString& edgeType) {
    enableNormalMode();
    setMode(SELECT_EDGE_MODE);
    for (MyElementBase *node : qAsConst(nodes()))
        node->enableSelectEdgeMode();
    for (MyElementBase *edge : qAsConst(edges()))
        edge->enableSelectEdgeMode();
    
    emit askForSetToolTip("Select " + edgeType + " edges");
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

void MyInteractor::annotateExportData(MyPluginItemBase* exportTool) {
    connect(this, SIGNAL(enterKeyIsPressed()), (MyDataExportTool*)exportTool, SLOT(annotateSelectedElements()));
    QJsonObject graphInfoObject;
    exportNetworkInfo(graphInfoObject);
    ((MyDataExportTool*)exportTool)->annotateElementsWithTypes(graphInfoObject);
}

void MyInteractor::writeDataToFile(MyPluginItemBase* exportTool) {
    disconnect(this, SIGNAL(enterKeyIsPressed()), (MyDataExportTool*)exportTool, SLOT(annotateSelectedElements()));
    QJsonObject graphInfoObject = ((MyDataExportTool*)exportTool)->getGraphInfoObject();
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
            // node style interface
            else if (qobject_cast<NodeStyleInterface *>(plugin))
                setNodeStyleInterface(qobject_cast<NodeStyleInterface *>(plugin), pluginsDir.path());
            // edge style interface
            else if (qobject_cast<EdgeStyleInterface *>(plugin))
                setEdgeStyleInterface(qobject_cast<EdgeStyleInterface *>(plugin), pluginsDir.path());
            // auto layout interface
            else if (qobject_cast<AutoLayoutInterface *>(plugin))
                setAutoLayoutInterface(qobject_cast<AutoLayoutInterface *>(plugin), pluginsDir.path());
        }
    }
}

QToolButton* MyInteractor::populateImportMenu() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* importWidgetAction = new MyWidgetAction(subMenu);
    importWidgetAction->setItems(importTools());
    connect(importWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(readFromFile(MyPluginItemBase*)));
    subMenu->addAction(importWidgetAction);
    button->setText("Import");
    button->setToolTip(tr("Import from a file"));
    button->setMenu(subMenu);
    return button;
}

QToolButton* MyInteractor::populateExportMenu() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    
    // data export
    if (dataExportTools().size()) {
        MyWidgetAction* dataExportWidgetAction = new MyWidgetAction(subMenu);
        dataExportWidgetAction->setItems(dataExportTools());
        connect(dataExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(annotateExportData(MyPluginItemBase*)));
        subMenu->addAction(dataExportWidgetAction);
    }
    
    if (dataExportTools().size() && printExportTools().size())
        subMenu->addSeparator();
    
    // print export
    if (printExportTools().size()) {
        MyWidgetAction* printExportWidgetAction = new MyWidgetAction(subMenu);
        printExportWidgetAction->setItems(printExportTools());
        connect(printExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(writeFigureToFile(MyPluginItemBase*)));
        subMenu->addAction(printExportWidgetAction);
    }
    
    button->setText("Export");
    button->setToolTip(tr("Export to a file"));
    button->setMenu(subMenu);
    return button;
}

QToolButton* MyInteractor::populateAddNodeMenu() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* addNodeWidgetAction = new MyWidgetAction(subMenu);
    addNodeWidgetAction->setItems(nodeStyles());
    connect(addNodeWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddNodeMode(MyPluginItemBase*)));
    subMenu->addAction(addNodeWidgetAction);
    button->setText("Node");
    button->setToolTip(tr("Add a node to the network"));
    button->setMenu(subMenu);
    return button;
}

QToolButton* MyInteractor::populateAddEdgeMenu() {
    MyToolButton* button = new MyToolButton();
    MyToolButtonMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* addEdgeWidgetAction = new MyWidgetAction(subMenu);
    addEdgeWidgetAction->setItems(edgeStyles());
    connect(addEdgeWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), this, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
    subMenu->addAction(addEdgeWidgetAction);
    button->setText("Edge");
    button->setToolTip(tr("Add an edge to the network"));
    button->setMenu(subMenu);
    return button;
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
    autoLayoutWidgetAction->setItems(autoLayoutEngines());
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
    for (MyEdge *edge : qAsConst(((MyNode*)_node)->edges()))
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
