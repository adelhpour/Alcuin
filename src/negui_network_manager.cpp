#include "negui_network_manager.h"
#include "negui_new_edge_builder.h"
#include "negui_copied_network_elements_paster.h"
#include "negui_network_element_selector.h"
#include "negui_network_element_mover.h"
#include "negui_network_element_aligner.h"
#include "negui_network_element_aligner_builder.h"
#include "negui_node.h"
#include "negui_node_builder.h"
#include "negui_edge.h"
#include "negui_edge_builder.h"
#include "negui_multi_network_element_feature_menu.h"

#include <QJsonArray>

MyNetworkManager::MyNetworkManager() {
    _newEdgeBuilder = NULL;
    setNetworkElementSelector();
    connect(this, &MyNetworkManager::networkElementsSelectedStatusIsChanged, this, [this] () {
        emit elementsCuttableStatusChanged(areSelectedElementsCuttable());
        emit elementsCopyableStatusChanged(areSelectedElementsCopyable());
        updateFeatureMenu();
    });
    resetNetwork();
}

void MyNetworkManager::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    resetCopiedNetworkElements();
    setCopiedNode(NULL);
    setNodeStyle(NULL);
    setCopiedNodeStyle(NULL);
    setEdgeStyle(NULL);
    setCopiedEdgeStyle(NULL);
    ((MyNetworkElementSelector*)_networkElementSelector)->enableNormalMode();
    deleteNewEdgeBuilder();
    for (MyNetworkElementBase *node : nodes())
        node->enableNormalMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableNormalMode();
}

void MyNetworkManager::enableAddNodeMode(MyPluginItemBase* style) {
    MySceneModeElementBase::enableAddNodeMode();
    setNodeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    ((MyNetworkElementSelector*)_networkElementSelector)->enableAddNodeMode();
    for (MyNetworkElementBase *node : nodes())
        node->enableAddNodeMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableAddNodeMode();
}

void MyNetworkManager::enableAddEdgeMode(MyPluginItemBase* style) {
    MySceneModeElementBase::enableAddEdgeMode();
    setEdgeStyle(dynamic_cast<MyNetworkElementStyleBase*>(style));
    ((MyNetworkElementSelector*)_networkElementSelector)->enableAddEdgeMode();
    for (MyNetworkElementBase *node : nodes())
        node->enableAddEdgeMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableAddEdgeMode();
}

void MyNetworkManager::enableSelectMode() {
    MySceneModeElementBase::enableSelectMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectMode();
    for (MyNetworkElementBase *node : nodes())
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableSelectEdgeMode();
}

void MyNetworkManager::enableSelectNodeMode() {
    enableNormalMode();
    MySceneModeElementBase::enableSelectNodeMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectNodeMode();
    for (MyNetworkElementBase *node : nodes())
        node->enableSelectNodeMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableSelectNodeMode();
}

void MyNetworkManager::enableSelectEdgeMode() {
    enableNormalMode();
    MySceneModeElementBase::enableSelectEdgeMode();
    ((MyNetworkElementSelector*)_networkElementSelector)->enableSelectEdgeMode();
    for (MyNetworkElementBase *node : nodes())
        node->enableSelectEdgeMode();
    for (MyNetworkElementBase *edge : edges())
        edge->enableSelectEdgeMode();
}

QList<MyNetworkElementBase*> MyNetworkManager::nodes() {
    return _nodes;
}

QJsonArray MyNetworkManager::listOfNodes() {
    QJsonArray listOfNodes;
    for (MyNetworkElementBase* node : nodes())
        listOfNodes.append(node->name());

    return listOfNodes;
}

const qreal MyNetworkManager::numberOfNodes() {
    return nodes().size();
}

QList<MyNetworkElementBase*> MyNetworkManager::edges() {
    return _edges;
}

QJsonArray MyNetworkManager::listOfEdges() {
    QJsonArray listOfEdges;
    for (MyNetworkElementBase* edge : edges())
        listOfEdges.append(edge->name());

    return listOfEdges;
}

const qreal MyNetworkManager::numberOfEdges() {
    return edges().size();
}

void MyNetworkManager::clearNodesInfo() {
    while(nodes().size())
        delete nodes().takeLast();
}

void MyNetworkManager::clearEdgesInfo() {
    while(edges().size())
        delete edges().takeLast();
}

MyNetworkElementStyleBase* MyNetworkManager::nodeStyle() {
    return _nodeStyle;
}

void MyNetworkManager::setNodeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _nodeStyle = style;
    else
        _nodeStyle = NULL;
}

void MyNetworkManager::setCopiedNodeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _copiedNodeStyle = style;
    else
        _copiedNodeStyle = NULL;
}

MyNetworkElementStyleBase* MyNetworkManager::copiedNodeStyle() {
    return _copiedNodeStyle;
}

bool MyNetworkManager::isSetCopiedNodeStyle() {
    if (_copiedNodeStyle)
        return true;

    return false;
}

void MyNetworkManager::pasteCopiedNodeStyle(MyNetworkElementBase* element) {
    element->updateStyle(getCopyNodeStyle(element->style()->name(), _copiedNodeStyle)->shapeStyles());
    element->updateGraphicsItem();
    askForCreateChangeStageCommand();
}

MyNetworkElementStyleBase* MyNetworkManager::edgeStyle() {
    return _edgeStyle;
}

void MyNetworkManager::setEdgeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _edgeStyle = style;
    else
        _edgeStyle = NULL;
}

void MyNetworkManager::setCopiedEdgeStyle(MyNetworkElementStyleBase* style) {
    if (style)
        _copiedEdgeStyle = style;
    else
        _copiedEdgeStyle = NULL;
}

MyNetworkElementStyleBase* MyNetworkManager::copiedEdgeStyle() {
    return _copiedEdgeStyle;
}

const bool MyNetworkManager::isSetCopiedEdgeStyle() {
    if (_copiedEdgeStyle)
        return true;

    return false;
}

void MyNetworkManager::pasteCopiedEdgeStyle(MyNetworkElementBase* element) {
    element->updateStyle(getCopyEdgeStyle(element->style()->name(), _copiedEdgeStyle)->shapeStyles());
    element->updateGraphicsItem();
    askForCreateChangeStageCommand();
}

void MyNetworkManager::setCutNode(MyNetworkElementBase* node) {
    setCopiedNode(node);
    removeNode(node);
}

void MyNetworkManager::setCopiedNode(MyNetworkElementBase* node) {
    resetCopiedNetworkElements();
    if (node)
        _copiedNetworkElements.push_back(node);
}

QList<MyNetworkElementBase*> MyNetworkManager::copiedNetworkElements() {
    return _copiedNetworkElements;
}

const bool MyNetworkManager::areAnyElementsSelected() {
    if (getSelectedNodes().size() || getSelectedEdges().size())
        return true;

    return false;
}

void MyNetworkManager::copySelectedNetworkElements() {
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

void MyNetworkManager::cutSelectedNetworkElements() {
    copySelectedNetworkElements();
    if (copiedNetworkElements().size()) {
        for (MyNetworkElementBase* selectedEdge : getSelectedEdges())
            removeEdge(selectedEdge);
        for (MyNetworkElementBase* selectedNode : getSelectedNodes())
            removeNode(selectedNode);
    }
}

void MyNetworkManager::pasteCopiedNetworkElements(const QPointF& position) {
    MyCopiedNetworkElementsPaster* copiedNetworkElementsPaster = new MyCopiedNetworkElementsPaster(copiedNetworkElements(), position);
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForAddNode, this, [this] (MyNetworkElementBase* node) { this->addNode(node); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForNodeUniqueName, this, [this] (MyNetworkElementStyleBase* nodeStyle) { return getElementUniqueName(this->nodes(), nodeStyle->category()); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForAddEdge, this, [this] (MyNetworkElementBase* edge) { this->addEdge(edge); });
    connect(copiedNetworkElementsPaster, &MyCopiedNetworkElementsPaster::askForEdgeUniqueName, this, [this] (MyNetworkElementStyleBase* edgeStyle) { return getElementUniqueName(this->edges(), edgeStyle->category()); });
    copiedNetworkElementsPaster->paste();
    copiedNetworkElementsPaster->deleteLater();
    askForCreateChangeStageCommand();
}


void MyNetworkManager::resetCopiedNetworkElements() {
    _copiedNetworkElements.clear();
    emit pasteElementsStatusChanged(false);
}

void MyNetworkManager::setNetworkElementSelector() {
    _networkElementSelector = new MyNetworkElementSelector();
    connect(_networkElementSelector, SIGNAL(askForWhetherShiftModifierIsPressed()), this, SIGNAL(askForWhetherShiftModifierIsPressed()));
    connect((MyNetworkElementSelector*)_networkElementSelector, &MyNetworkElementSelector::askForNodes, this, [this] () { return nodes(); });
    connect((MyNetworkElementSelector*)_networkElementSelector, &MyNetworkElementSelector::askForEdges, this, [this] () { return edges(); });
    connect(_networkElementSelector, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
    connect(_networkElementSelector, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
}

void MyNetworkManager::changeElementSelectionsStatus(MyNetworkElementBase* element) {
    ((MyNetworkElementSelector*)_networkElementSelector)->changeElementSelectionsStatus(element);
    emit networkElementsSelectedStatusIsChanged();
}

const bool MyNetworkManager::areSelectedElementsCopyable() {
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

const bool MyNetworkManager::areSelectedElementsCuttable() {
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

const bool MyNetworkManager::areSelectedElementsAlignable() {
    if (getSelectedNodes().size() > 1)
        return true;

    return false;
}

const bool MyNetworkManager::areAnyElementsCopied() {
    if (copiedNetworkElements().size())
        return true;

    return false;
}

const QList<MyNetworkElementBase*> MyNetworkManager::getSelectedElements() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedElements();
}

const QList<MyNetworkElementBase*> MyNetworkManager::getSelectedNodes() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedNodes();
}

const QList<MyNetworkElementBase*> MyNetworkManager::getSelectedEdges() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getSelectedEdges();
}

void MyNetworkManager::createNetwork(const QJsonObject& json) {
    resetNetwork();
    setBackgroundColor(json);
    addNodes(json);
    addEdges(json);
}

void MyNetworkManager::resetNetworkCanvas() {
    resetNetwork();
    resetCanvas();
}

void MyNetworkManager::resetNetwork() {
    clearNodesInfo();
    clearEdgesInfo();
    emit askForClearScene();
}

void MyNetworkManager::resetCanvas() {
    emit askForResetScale();
    askForEnableNormalMode();
    askForClearUndoStack();
}

void MyNetworkManager::setBackgroundColor(const QJsonObject &json) {
    if (json.contains("background-color") && json["background-color"].isString())
        askForSetNetworkBackgroundColor(json["background-color"].toString());
}

void MyNetworkManager::addNodes(const QJsonObject &json) {
    if (json.contains("nodes") && json["nodes"].isArray()) {
        QJsonArray nodesArray = json["nodes"].toArray();
        for (int nodeIndex = 0; nodeIndex < nodesArray.size(); ++nodeIndex)
            addNode(nodesArray[nodeIndex].toObject());
        updateNodeParents();
    }
}

void MyNetworkManager::addNode(const QJsonObject &json) {
    MyNetworkElementBase* node = createNode(json);
    if (node)
        addNode(node);
}

void MyNetworkManager::addNode(const QPointF& position) {
    if (getSceneMode() == ADD_NODE_MODE) {
        MyNetworkElementBase* node = createNode(getElementUniqueName(nodes(), nodeStyle()->category()), getCopyNodeStyle(getElementUniqueName(nodes(), nodeStyle()->category()) + "_style", nodeStyle()), position.x(), position.y());
        addNode(node);
        askForCreateChangeStageCommand();
    }
}

void MyNetworkManager::addNode(MyNetworkElementBase* n) {
    if (n && !n->isActive()) {
        _nodes.push_back(n);
        n->setActive(true);
        n->updateGraphicsItem();
        connect(n, &MyNetworkElementBase::askForSelectNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { changeElementSelectionsStatus(networkElement); });
        connect(n, &MyNetworkElementBase::askForSelectNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { addNewEdge(networkElement); });
        connect(n, &MyNetworkElementBase::askForDeleteNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { deleteNode(networkElement); });
        connect(n, &MyNetworkElementBase::askForListOfElements, this, [this] () { return nodes() + edges(); } );
        connect(n, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
        connect(n, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
        connect(n, SIGNAL(askForEnableFeatureMenuDisplay()), this, SIGNAL(askForEnableFeatureMenuDisplay()));
        connect(n, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()));
        connect(n, &MyNetworkElementBase::askForDisplayFeatureMenu, this, [this] (MyNetworkElementBase* networkElement) {
            selectNetworkElements(false);
            setNetworkElementSelected(networkElement->name(), true);
            emit askForEnableFeatureMenuDisplay();
            updateFeatureMenu(); });
        connect(n, &MyNetworkElementBase::askForCheckWhetherNetworkElementNameIsAlreadyUsed, this, [this] (const QString& elementName) { return isElementNameAlreadyUsed(elementName); });
        connect(n, &MyNetworkElementBase::askForCopyNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { setCopiedNode(networkElement); });
        connect(n, &MyNetworkElementBase::askForCutNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { setCutNode(networkElement); });
        connect(n, &MyNetworkElementBase::askForCopyNetworkElementStyle, this, [this] (MyNetworkElementStyleBase* style) { setCopiedNodeStyle(style); });
        connect(n, &MyNetworkElementBase::askForPasteNetworkElementStyle, this, [this] (MyNetworkElementBase* networkElement) { pasteCopiedNodeStyle(networkElement); });
        connect(n, &MyNetworkElementBase::askForWhetherElementStyleIsCopied, this, [this] () { return isSetCopiedNodeStyle(); } );
        connect(n, &MyNetworkElementBase::askForWhetherAnyOtherElementsAreSelected, this, [this] (QList<MyNetworkElementBase*> networkElements) {
            return ((MyNetworkElementSelector*)_networkElementSelector)->areAnyOtherElementsSelected(networkElements); });
        connect(n, SIGNAL(askForIconsDirectoryPath()), this, SIGNAL(askForIconsDirectoryPath()));
        connect(n, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SLOT(moveSelectedNetworkElements(const QPointF&)));
        connect(n, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), this, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)));
        connect(n, SIGNAL(askForWhetherControlModifierIsPressed()), this, SIGNAL(askForWhetherControlModifierIsPressed()));
        connect(n->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(n->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        connect(this, SIGNAL(askForAdjustConnectedEdgesOfNodes()), n, SLOT(adjustConnectedEdges()));
        connect(this, SIGNAL(askForAdjustExtentsOfNodes()), n, SLOT(adjustExtents()));
        emit askForAddGraphicsItem(n->graphicsItem());
    }
}

void MyNetworkManager::deleteNode(const QString& nodeName) {
    MyNetworkElementBase *node = findElement(nodes(), nodeName);
    if (node)
        deleteNode(node);
}

void MyNetworkManager::deleteNode(MyNetworkElementBase* node) {
    for (MyNetworkElementBase *edge : ((MyNodeBase*)node)->edges()) {
        ((MyNodeBase*)node)->removeEdge(edge);
        removeEdge(edge);
    }
    removeNode(node);
    askForCreateChangeStageCommand();
}

void MyNetworkManager::removeNode(MyNetworkElementBase* n) {
    if (n && n->isActive()) {
        _nodes.removeOne(n);
        n->setActive(false);
        n->setSelected(false);
        emit askForRemoveGraphicsItem(n->graphicsItem());
    }
}

void MyNetworkManager::updateNodeParents() {
    MyNetworkElementBase* parentNode = NULL;
    for (MyNetworkElementBase *node : nodes()) {
        parentNode = findElement(nodes(), ((MyNodeBase*)node)->parentNodeId());
        if (parentNode)
            ((MyNodeBase*)node)->setParentNode((MyNodeBase*)parentNode);
    }
}

void MyNetworkManager::addEdges(const QJsonObject &json) {
    if (json.contains("edges") && json["edges"].isArray()) {
        QJsonArray edgesArray = json["edges"].toArray();
        for (int edgeIndex = 0; edgeIndex < edgesArray.size(); ++edgeIndex)
            addEdge(edgesArray[edgeIndex].toObject());
    }
}

void MyNetworkManager::addEdge(const QJsonObject &json) {
    MyNetworkElementBase* edge = createEdge(json, findSourceNode(nodes(), json), findTargetNode(nodes(), json));
    if (edge)
        addEdge(edge);
}

void MyNetworkManager::addEdge(MyNetworkElementBase* e) {
    if (e && !edgeExists(((MyEdgeBase*)e)->sourceNode(), ((MyEdgeBase*)e)->targetNode()) && e->setActive(true)) {
        _edges.push_back(e);
        e->updateGraphicsItem();
        connect(e, &MyNetworkElementBase::askForSelectNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { changeElementSelectionsStatus(networkElement); });
        connect(e, &MyNetworkElementBase::askForDeleteNetworkElement, this, [this] (MyNetworkElementBase* networkElement) { deleteEdge(networkElement); });
        connect(e, &MyNetworkElementBase::askForListOfElements, this, [this] () { return nodes() + edges(); } );
        connect(e, SIGNAL(askForItemsAtPosition(const QPointF&)), this, SIGNAL(askForItemsAtPosition(const QPointF&)));
        connect(e, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
        connect(e, SIGNAL(askForEnableFeatureMenuDisplay()), this, SIGNAL(askForEnableFeatureMenuDisplay()));
        connect(e, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()), this, SIGNAL(askForCurrentlyBeingDisplayedFeatureMenu()));
        connect(e, &MyNetworkElementBase::askForDisplayFeatureMenu, this, [this] (MyNetworkElementBase* networkElement) {
            selectNetworkElements(false);
            setNetworkElementSelected(networkElement->name(), true);
            emit askForEnableFeatureMenuDisplay();
            updateFeatureMenu(); });
        connect(e, &MyNetworkElementBase::askForCheckWhetherNetworkElementNameIsAlreadyUsed, this, [this] (const QString& elementName) { return isElementNameAlreadyUsed(elementName); });
        connect(e, &MyNetworkElementBase::askForCopyNetworkElementStyle, this, [this] (MyNetworkElementStyleBase* style) { setCopiedEdgeStyle(style); });
        connect(e, &MyNetworkElementBase::askForPasteNetworkElementStyle, this, [this] (MyNetworkElementBase* networkElement) { pasteCopiedEdgeStyle(networkElement); });
        connect(e, &MyNetworkElementBase::askForWhetherAnyOtherElementsAreSelected, this, [this] (QList<MyNetworkElementBase*> networkElements) {
            return ((MyNetworkElementSelector*)_networkElementSelector)->areAnyOtherElementsSelected(networkElements); });
        connect(e, &MyNetworkElementBase::askForWhetherElementStyleIsCopied, this, [this] () { return isSetCopiedEdgeStyle(); } );
        connect(e, SIGNAL(askForIconsDirectoryPath()), this, SIGNAL(askForIconsDirectoryPath()));
        connect(e->graphicsItem(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForAddGraphicsItem(QGraphicsItem*)));
        connect(e->graphicsItem(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), this, SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)));
        emit askForAddGraphicsItem(e->graphicsItem());
        if (((MyEdgeBase*)e)->arrowHeadGraphicsItem())
            emit askForAddGraphicsItem(((MyEdgeBase*)e)->arrowHeadGraphicsItem());
    }
}

void MyNetworkManager::addNewEdge(QList<QString> sourceNodes, QList<QString> targetNodes) {
    MyNetworkElementBase* node = NULL;
    for (QString nodeName : sourceNodes + targetNodes) {
        node = findElement(nodes(), nodeName);
        if (!node)
            break;
        addNewEdge(node);
    }
}

void MyNetworkManager::addNewEdge(MyNetworkElementBase* element) {
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
            askForCreateChangeStageCommand();
        }
    }
}

void MyNetworkManager::removeEdge(MyNetworkElementBase* e) {
    if (e && e->isActive()) {
        _edges.removeOne(e);
        e->setActive(false);
        e->setSelected(false);
        emit askForRemoveGraphicsItem(e->graphicsItem());
        if (((MyEdgeBase*)e)->arrowHeadGraphicsItem())
            emit askForRemoveGraphicsItem(((MyEdgeBase*)e)->arrowHeadGraphicsItem());
    }
}

void MyNetworkManager::deleteEdge(const QString& edgeName) {
    MyNetworkElementBase *edge = findElement(edges(), edgeName);
    if (edge)
        deleteEdge(edge);
}

void MyNetworkManager::deleteEdge(MyNetworkElementBase* edge) {
    removeEdge(edge);
    askForCreateChangeStageCommand();
}

void MyNetworkManager::moveSelectedNetworkElements(const QPointF& movedDistance) {
    MyNetworkElementMoverBase* nodeMover = new MyNodeMover(getSelectedNodes());
    nodeMover->move(movedDistance.x(), movedDistance.y());
    nodeMover->deleteLater();
}

void MyNetworkManager::deleteSelectedNetworkElements() {
    for (MyNetworkElementBase* selectedNode : getSelectedNodes()) {
        for (MyNetworkElementBase *edge : ((MyNodeBase*)selectedNode)->edges()) {
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
    askForCreateChangeStageCommand();
}

void MyNetworkManager::alignSelectedNetworkElements(const QString& alignType) {
    MyNetworkElementAlignerBase* networkElementAligner = createNetworkElementAligner(getSelectedNodes(), getSelectedEdges(), alignType);
    if (networkElementAligner) {
        networkElementAligner->align();
        networkElementAligner->deleteLater();
        askForCreateChangeStageCommand();
    }
}

bool MyNetworkManager::isElementNameAlreadyUsed(const QString& elementName) {
    for (MyNetworkElementBase* node : nodes()) {
        if (node->name() == elementName)
            return true;
    }
    for (MyNetworkElementBase* edge : edges()) {
        if (edge->name() == elementName)
            return true;
    }

    return false;
}

void MyNetworkManager::deleteNewEdgeBuilder() {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNodes(false);
    if (_newEdgeBuilder)
        _newEdgeBuilder->deleteLater();
    _newEdgeBuilder = NULL;
}

bool MyNetworkManager::edgeExists(MyNetworkElementBase* n1, MyNetworkElementBase* n2) {
    for (MyNetworkElementBase *edge : edges()) {
        if ((((MyEdgeBase*)edge)->sourceNode() == n1 && ((MyEdgeBase*)edge)->targetNode() == n2) || (((MyEdgeBase*)edge)->sourceNode() == n2 && ((MyEdgeBase*)edge)->targetNode() == n1)) {
            return true;
        }
    }

    return false;
}

QJsonObject MyNetworkManager::getNetworkElementsAndColorInfo() {
    QJsonObject json;

    // background color
    json["background-color"] = askForNetworkBackgroundColor();

    // nodes
    QJsonArray nodesArray;
    for (MyNetworkElementBase *node : nodes()) {
        QJsonObject nodeObject;
        node->write(nodeObject);
        nodesArray.append(nodeObject);
    }
    json["nodes"] = nodesArray;

    // edges
    QJsonArray edgesArray;
    for (MyNetworkElementBase *edge : edges()) {
        QJsonObject edgeObject;
        edge->write(edgeObject);
        edgesArray.append(edgeObject);
    }
    json["edges"] = edgesArray;

    return json;
}

QJsonObject MyNetworkManager::exportNetworkInfo() {
    QJsonObject json = getNetworkElementsAndColorInfo();

    QRectF networkExtents = askForItemsBoundingRect();
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

void MyNetworkManager::selectNetworkElements(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNetworkElements(selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::selectNetworkElementsOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNetworkElementsOfCategory(category, selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::selectNodes(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNodes(selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::selectNodesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectNodesOfCategory(category, selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::selectEdges(const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectEdges(selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::selectEdgesOfCategory(const bool& selected, const QString& category) {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectEdgesOfCategory(category, selected);
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::setNetworkElementSelected(const QString& networkElementName, const bool& selected) {
    ((MyNetworkElementSelector*)_networkElementSelector)->setNetworkElementSelected(networkElementName, selected);
    emit networkElementsSelectedStatusIsChanged();
}

MyNetworkElementBase* MyNetworkManager::getOneSingleSelectedElement() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyNetworkManager::getOneSingleSelectedNode() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

MyNetworkElementBase* MyNetworkManager::getOneSingleSelectedEdge() {
    return ((MyNetworkElementSelector*)_networkElementSelector)->getOneSingleSelectedElement();
}

void MyNetworkManager::displaySelectionArea(const QPointF& position) {
    ((MyNetworkElementSelector*)_networkElementSelector)->displaySelectionArea(position);
}

void MyNetworkManager::createSelectionAreaGraphicsItem(const QPointF& position) {
    ((MyNetworkElementSelector*)_networkElementSelector)->createSelectionAreaGraphicsItem(position);
}

void MyNetworkManager::selectSelectionAreaCoveredNodes() {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectSelectionAreaCoveredNodes();
}

void MyNetworkManager::selectSelectionAreaCoveredEdges() {
    ((MyNetworkElementSelector*)_networkElementSelector)->selectSelectionAreaCoveredEdges();
}

void MyNetworkManager::clearSelectionArea() {
    ((MyNetworkElementSelector*)_networkElementSelector)->clearSelectionArea();
    emit networkElementsSelectedStatusIsChanged();
}

void MyNetworkManager::updateFeatureMenu() {
    if (askForWhetherFeatureMenuCanBeDisplayed()) {
        if (getOneSingleSelectedNode())
            askForDisplayFeatureMenu(getOneSingleSelectedNode()->createFeatureMenu());
        else if (getOneSingleSelectedEdge())
            askForDisplayFeatureMenu(getOneSingleSelectedEdge()->createFeatureMenu());
        else if (getSelectedElements().size()) {
            QWidget* multiNetworkElementFeatureMenu = new MyMultiNetworkElementFeatureMenu(getSelectedElements(), askForIconsDirectoryPath());
            connect(multiNetworkElementFeatureMenu, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
            askForDisplayFeatureMenu(multiNetworkElementFeatureMenu);
        }
        else
            askForDisplayNullFeatureMenu();
    }
}
