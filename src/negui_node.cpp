#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_style.h"
#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include "negui_customized_common_widgets.h"

#include <QJsonObject>
#include <QGridLayout>

// MyNodeBase

MyNodeBase::MyNodeBase(const QString& name) : MyNetworkElementBase(name) {
    _parentNodeId = "N/A";
    _parentNode = NULL;
    _isSetParentNode = false;
    _isParentNodeLocked = false;
    _endEdgePadding = 0.0;
}

MyNodeBase::ELEMENT_TYPE MyNodeBase::type() {
    return NODE_ELEMENT;
}

void MyNodeBase::connectGraphicsItem() {
    MyNetworkElementBase::connectGraphicsItem();
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForSelectNetworkElement, this, [this] () { emit askForSelectNetworkElement(this); });
    connect(_graphicsItem, SIGNAL(askForUpdateParentNode()), this,  SLOT(updateSelectedNodesParentNode()));
    connect(_graphicsItem, SIGNAL(askForUpdateConnectedEdgesPoints()), this, SLOT(updateConnectedEdgesPoints()));
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SLOT(adjustConnectedEdges()));
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)));
}

void MyNodeBase::addEdge(MyNetworkElementBase* e) {
    if (e)
        _edges.push_back(e);
}

void MyNodeBase::removeEdge(MyNetworkElementBase* e) {
    if (e)
        _edges.removeOne(e);
}

QList<MyNetworkElementBase*>& MyNodeBase::edges() {
    return _edges;
}

void MyNodeBase::updateGraphicsItem() {
    MyNetworkElementBase::updateGraphicsItem();
    updateConnectedEdgesPoints();
}

void MyNodeBase::setSelectedWithColor(const bool& selected) {
    if (selected)
        graphicsItem()->setSelectedWithFillColor(selected);
    else {
        graphicsItem()->setSelectedWithBorderColor(selected);
        graphicsItem()->setSelectedWithFillColor(selected);
    }
}

const bool MyNodeBase::isCuttable() {
    if (!isSelected())
        return false;
    for (MyNetworkElementBase* edge : edges()) {
        if (!edge->isSelected())
            return false;
    }

    return true;
}

void MyNodeBase::updateConnectedEdgesPoints() {
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase*)edge)->updatePoints();
}

void MyNodeBase::updateSelectedNodesParentNode() {
    QList<MyNetworkElementBase*> elements = askForListOfElements();
    for (MyNetworkElementBase* element : elements) {
        if (element->type() == MyNetworkElementBase::NODE_ELEMENT && element->isSelected())
            ((MyNodeBase*)element)->updateParentNode();
    }
}

void MyNodeBase::updateParentNode() {
    MyNetworkElementBase* parentNode = getParentNodeAtPosition(getPosition());
    if (!parentNode || parentNode != _parentNode) {
        unsetParentNode();
        setConnectedNodesParents();
        emit parentNodeIsUpdated();
    }

    if (parentNode && !_parentNode && ((MyClassicNodeStyleBase*)parentNode->style())->isConvertibleToParentCategory(((MyNodeStyleBase*)style())->parentCategories())) {
        ((MyClassicNodeStyleBase*)parentNode->style())->convertToParentCategory();
        setParentNode((MyNodeBase*)parentNode);
        if (askForWhetherControlModifierIsPressed())
            ((MyClassicNodeBase*)parentNode)->adjustExtents();
        updateConnectedEdgesPoints();
        setConnectedNodesParents();
        emit parentNodeIsUpdated();
    }
}

MyNetworkElementBase* MyNodeBase::getParentNodeAtPosition(const QPointF& position) {
    QList<QGraphicsItem *> items = askForItemsAtPosition(position);
    MyNetworkElementBase* parentNode = NULL;
    qint32 parentNodeZValue = INT_MIN;
    for (QGraphicsItem* item : qAsConst(items)) {
        if (item->parentItem()) {
            QList<MyNetworkElementBase*> elements = askForListOfElements();
            for (MyNetworkElementBase* element : elements) {
                if (element->type() == MyNetworkElementBase::NODE_ELEMENT) {
                    if (element->graphicsItem() == item->parentItem() && element != this) {
                        if (item->parentItem()->zValue() > parentNodeZValue) {
                            parentNode = element;
                            parentNodeZValue = item->parentItem()->zValue();
                        }
                    }
                }
            }
        }
    }

    return parentNode;
}

const QString& MyNodeBase::parentNodeId() const {
    return _parentNodeId;
}

void MyNodeBase::setParentNodeId(const QString& parentNodeId) {
    _parentNodeId = parentNodeId;
}

MyNetworkElementBase* MyNodeBase::parentNode() {
    return _parentNode;
}

void MyNodeBase::setParentNode(MyNetworkElementBase* parentNode) {
    _parentNode = parentNode;
    _isSetParentNode = true;
    _parentNodeId = parentNode->name();
    ((MyClassicNodeBase*)_parentNode)->addChildNode(this);
    graphicsItem()->setZValue(calculateZValue());
}

void MyNodeBase::unsetParentNode() {
    if (_parentNode) {
        ((MyClassicNodeBase*)_parentNode)->removeChildNode(this);
        if (askForWhetherControlModifierIsPressed())
            ((MyClassicNodeBase*)_parentNode)->adjustExtents();
    }
    _parentNode = NULL;
    _isSetParentNode = false;
    _parentNodeId = "N/A";
    graphicsItem()->setZValue(calculateZValue());
}

void MyNodeBase::lockParentNode(const bool& locked) {
    _isParentNodeLocked = locked;
}

const QPointF MyNodeBase::getPosition() {
    return getExtents().center();
}

const QRectF MyNodeBase::getExtents() {
    return ((MyNodeSceneGraphicsItemBase*)graphicsItem())->getExtents();
}

const bool MyNodeBase::canBeMoved() {
    if (getSceneMode() == NORMAL_MODE)
        return true;

    return false;
}

const qreal MyNodeBase::endEdgePadding() {
    return  _endEdgePadding;
}

void MyNodeBase::addParentFeaturesToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QString parentLabel = "Parent";
    if (!((MyNodeStyleBase*)style())->parentTitle().isEmpty())
        parentLabel = ((MyNodeStyleBase*)style())->parentTitle();
    contentLayout->addWidget(new MyLabel(parentLabel, parentLabel + " that encloses " + styleCategory()), contentLayout->rowCount(), 0, Qt::AlignLeft);
    QWidget* parentNodeLineEdit = new MyReadOnlyLineEdit(parentNodeId());
    connect(this, &MyNodeBase::parentNodeIsUpdated, parentNodeLineEdit, [this, parentNodeLineEdit] () { ((MyReadOnlyLineEdit*)parentNodeLineEdit)->setText(parentNodeId()); });
    contentLayout->addWidget(parentNodeLineEdit, contentLayout->rowCount() - 1, 1, Qt::AlignRight);
}

const qint32 MyNodeBase::calculateZValue() {
    return calculateNodeZValue() + calculateParentZValue();
}

const qint32 MyNodeBase::calculateParentZValue() {
    qint32 parentZValue = 0;
    MyNetworkElementBase* parent = this;
    while (parent && ((MyNodeBase*)parent)->parentNodeId() != "N/A") {
        parentZValue += 10;
        parent = ((MyNodeBase*)parent)->parentNode();
    }

    return parentZValue;
}

void MyNodeBase::adjustConnectedEdges(const bool& movedByParentNodeMove) {
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase *) edge)->askForAdjustNodePositionToNeighborNodes(movedByParentNodeMove);
}

void MyNodeBase::setConnectedNodesParents() {
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase *) edge)->askForSetNodeParentUsingNeighborNodesParent();
}

void MyNodeBase::read(const QJsonObject &json) {
    /*
    // position
    if (json.contains("position") && json["position"].isObject()) {
        if (json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
            _position = QPointF(json["position"]["x"].toDouble(), json["position"]["y"].toDouble());
        }
    }

    // parent
    if (json.contains("parent") && json["parent"].isString())
        setParentNodeId(json["parent"].toString());

    // style
    if (json.contains("style") && json["style"].isObject())
        style()->read(json["style"].toObject());
    */
}

void MyNodeBase::write(QJsonObject &json) {
    // id
    json["id"] = name();

    QRectF extents = getExtents();
    // position
    QJsonObject positionObject;
    positionObject["x"] = extents.center().x();
    positionObject["y"] = extents.center().y();
    json["position"] = positionObject;

    // dimensions
    QJsonObject dimensionsObject;
    dimensionsObject["width"] = extents.width();
    dimensionsObject["height"] = extents.height();
    json["dimensions"] = dimensionsObject;

    // parent node
    if (parentNodeId() != "N/A")
        json["parent"] = parentNodeId();

    // style
    QJsonObject styleObject;
    style()->write(styleObject);
    json["style"] = styleObject;
}

// MyClassicNodeBase

MyClassicNodeBase::MyClassicNodeBase(const QString& name) : MyNodeBase(name) {
    _areChildNodesLocked = false;
    _endEdgePadding = 5.0;
}

const bool MyClassicNodeBase::isCopyable() {
    if (isSelected())
        return true;

    return false;
}

void MyClassicNodeBase::addChildNode(MyNetworkElementBase* n) {
    if (n)
        _childNodes.push_back(n);
}

void MyClassicNodeBase::removeChildNode(MyNetworkElementBase* n) {
    if (n)
        _childNodes.removeOne(n);
}
QList<MyNetworkElementBase*>& MyClassicNodeBase::childNodes() {
    return _childNodes;
}

void MyClassicNodeBase::lockChildNodes(const bool& locked) {
    _areChildNodesLocked = locked;
}

void MyClassicNodeBase::move(const qreal& dx, const qreal& dy) {
    if (canBeMoved()) {
        ((MyNodeSceneGraphicsItemBase*)(graphicsItem()))->move(dx, dy);
        moveChildNodes(dx, dy);
        adjustParentExtents();
        adjustConnectedEdges();
    }
    updateConnectedEdgesPoints();
    updateFocusedGraphicsItems();
}

void MyClassicNodeBase::moveChildNodes(const qreal& dx, const qreal& dy) {
    for (MyNetworkElementBase* node : qAsConst(childNodes()))
        ((MyNodeBase*)node)->move(dx, dy);
}

void MyClassicNodeBase::adjustParentExtents() {
    if (parentNode() && askForWhetherControlModifierIsPressed())
        ((MyClassicNodeBase*)parentNode())->adjustExtents();
}

const QRectF MyClassicNodeBase::getExtents() {
    ((MyNodeSceneGraphicsItemBase*)_graphicsItem)->updateOriginalPosition();
    if (childNodes().size()) {
        QRectF childExtents = ((MyNodeBase*)childNodes().at(0))->getExtents();
        qreal extentsX = childExtents.x();
        qreal extentsY = childExtents.y();
        qreal extentsWidth = childExtents.width();
        qreal extentsHeight = childExtents.height();
        for (MyNetworkElementBase* childNode : qAsConst(childNodes())) {
            childExtents = ((MyNodeBase*)childNode)->getExtents();
            if (childExtents.x() < extentsX) {
                extentsWidth += extentsX - childExtents.x();
                extentsX = childExtents.x();
            }
            if (childExtents.y() < extentsY) {
                extentsHeight += extentsY - childExtents.y();
                extentsY = childExtents.y();
            }
            if (extentsX + extentsWidth < childExtents.x() + childExtents.width())
                extentsWidth += childExtents.x() + childExtents.width() - extentsX - extentsWidth;
            if (extentsY + extentsHeight < childExtents.y() + childExtents.height())
                extentsHeight +=  childExtents.y() + childExtents.height() - extentsY - extentsHeight;
        }
        return createParentNodeExtentsRectangle(extentsX, extentsY, extentsWidth, extentsHeight);
    }

    return MyNodeBase::getExtents();
}

const QRectF MyClassicNodeBase::createParentNodeExtentsRectangle(qreal extentsX, qreal extentsY, qreal extentsWidth, qreal extentsHeight) {
    qreal minimumWidth = 150.0;
    qreal minimumHeight = 120.0;
    if (childNodes().size() == 1) {
        if (extentsWidth < minimumWidth) {
            extentsX -= 0.5 * (minimumWidth - extentsWidth);
            extentsWidth = minimumWidth;
        }
        if (extentsHeight < minimumHeight) {
            extentsY -= 0.5 * (minimumHeight - extentsHeight);
            extentsHeight = minimumHeight;
        }
    }

    return QRectF(extentsX - 10.0, extentsY - 10.0, extentsWidth + 20.0, extentsHeight + 20.0);
}

void MyClassicNodeBase::adjustExtents() {
    QRectF extents = getExtents();
    QPointF position = getPosition();
    ((MyClassicNodeSceneGraphicsItemBase*)graphicsItem())->move(extents.x() - (position.x() - 0.5 * extents.width()), extents.y() - (position.y() - 0.5 * extents.height()));
    ((MyClassicNodeSceneGraphicsItemBase*)graphicsItem())->updateExtents(extents);
}

const qint32 MyClassicNodeBase::calculateNodeZValue() {
    return ((MyClassicNodeStyleBase*)style())->convertibleParentCategory().isEmpty() ? 7 : 0;
}

const qint32 MyClassicNodeBase::calculateConnectedEdgeZValue() {
    return calculateZValue() - 4;
}

// MySimpleClassicNode

MySimpleClassicNode::MySimpleClassicNode(const QString& name, const qreal& x, const qreal& y) : MyClassicNodeBase(name) {
    _displayName = name;
    _graphicsItem = createGraphicsItem(QPointF(x, y));
    connectGraphicsItem();
}

void MySimpleClassicNode::connectGraphicsItem() {
    MyNodeBase::connectGraphicsItem();
    connect((MyNetworkElementGraphicsItemBase*)_graphicsItem, &MyNetworkElementGraphicsItemBase::askForDisplayName, this, [this] () { return displayName(); });
}

MyNodeBase::NODE_TYPE MySimpleClassicNode::nodeType() {
    return SIMPLE_CLASSIC_NODE;
}

MyNetworkElementGraphicsItemBase* MySimpleClassicNode::createGraphicsItem(const QPointF &position) {
    return createSimpleClassicNodeSceneGraphicsItem(position);
}

const QString& MySimpleClassicNode::displayName() {
    return _displayName;
}

void MySimpleClassicNode::setDisplayName(const QString& displayName) {
    _displayName = displayName;
}

QWidget* MySimpleClassicNode::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    addDisplayNameToFeatureMenu(featureMenu);
    addParentFeaturesToFeatureMenu(featureMenu);
    addSpacerItemToFeatureMenu(featureMenu);
    addChangeShapeStyleButtonToFeatureMenu(featureMenu);

    return featureMenu;
}

void MySimpleClassicNode::addDisplayNameToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    MyParameterBase* plainTextParameter = style()->getParameter(MyShapeStyleBase::SIMPLE_TEXT_SHAPE_STYLE, "plain-text");
    if (plainTextParameter) {
        contentLayout->addWidget(new MyLabel("Display Name", "Name displayed as the representing text of " + styleCategory()), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(plainTextParameter->inputWidget(), contentLayout->rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    }
}

// MyComplexClassicNode

MyComplexClassicNode::MyComplexClassicNode(const QString& name, const qreal& x, const qreal& y) : MyClassicNodeBase(name) {
    _graphicsItem = createGraphicsItem(QPointF(x, y));
    connectGraphicsItem();
}

MyNodeBase::NODE_TYPE MyComplexClassicNode::nodeType() {
    return COMPLEX_CLASSIC_NODE;
}

MyNetworkElementGraphicsItemBase* MyComplexClassicNode::createGraphicsItem(const QPointF &position) {
    return createComplexClassicNodeSceneGraphicsItem(position);
}

QWidget* MyComplexClassicNode::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    addParentFeaturesToFeatureMenu(featureMenu);
    addSpacerItemToFeatureMenu(featureMenu);
    addAddRemoveShapeStyleButtonsToFeatureMenu(featureMenu);

    return featureMenu;
}

// MyCentroidNode

MyCentroidNode::MyCentroidNode(const QString& name, const qreal& x, const qreal& y) : MyNodeBase(name) {
    _isNodePositionConnectedToNeighborNodes = true;
    _graphicsItem = createGraphicsItem(QPointF(x, y));
    connectGraphicsItem();
}

MyNodeBase::NODE_TYPE MyCentroidNode::nodeType() {
    return CENTROID_NODE;
}

void MyCentroidNode::connectGraphicsItem() {
    MyNodeBase::connectGraphicsItem();
    connect(_graphicsItem, SIGNAL(askForGetBezierAdjustLine()), this, SLOT(createBezierAdjustLine()));
    connect(_graphicsItem, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), this, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)));
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SLOT(adjustConnectedBezierCurves()));
    connect(_graphicsItem, SIGNAL(askForConnectNodePositionToNeighborNodes(const bool&)), this, SLOT(connectNodePositionToNeighborNodes(const bool&)));
    connect(_graphicsItem, SIGNAL(askForWhetherNodePositionIsConnectedToNeighborNodes()), this, SLOT(isNodePositionConnectedToNeighborNodes()));
}

MyNetworkElementGraphicsItemBase* MyCentroidNode::createGraphicsItem(const QPointF &position) {
    return createCentroidNodeSceneGraphicsItem(position);
}

const bool MyCentroidNode::isCopyable() {
    if (!isSelected())
        return false;
    for (MyNetworkElementBase* edge : edges()) {
        if (!edge->isCopyable())
            return false;
    }

    return true;
}

void MyCentroidNode::addEdge(MyNetworkElementBase* e) {
    MyNodeBase::addEdge(e);
    connect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes(const bool&)), this, SLOT(adjustNodePositionToNeighborNodes(const bool&)));
    connect(e, SIGNAL(askForSetNodeParentUsingNeighborNodesParent()), this, SLOT(setNodeParentUsingNeighborNodesParent()));
    connect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setSelected(const bool&)));
}

void MyCentroidNode::removeEdge(MyNetworkElementBase* e) {
    MyNodeBase::removeEdge(e);
    disconnect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes(const bool&)), this, SLOT(adjustNodePositionToNeighborNodes(const bool&)));
    disconnect(e, SIGNAL(askForSetNodeParentUsingNeighborNodesParent()), this, SLOT(setNodeParentUsingNeighborNodesParent()));
    disconnect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setSelected(const bool&)));
}

void MyCentroidNode::adjustNodePositionToNeighborNodes(const bool& movedByParentNodeMove) {
    if (!movedByParentNodeMove || !parentNode()) {
        if (isNodePositionConnectedToNeighborNodes() && edges().size()) {
            QPointF oldPosition = getPosition();
            QPointF updatedPosition = getNodeUpdatedPositionUsingConnectedEdges();
            ((MyCentroidNodeSceneGraphicsItem *) graphicsItem())->move((updatedPosition - oldPosition).x(),
                                                                         (updatedPosition - oldPosition).y());
            updateConnectedEdgesPoints();
            updateFocusedGraphicsItems();
            adjustConnectedBezierCurves();
        }
    }
}

void MyCentroidNode::adjustExtents() {

}

void MyCentroidNode::adjustConnectedBezierCurves() {
    if (connectedBezierCurvesNeedsToBeAdjusted())
        emit bezierAdjustLineIsUpdated(createBezierAdjustLine());
}

const bool MyCentroidNode::connectedBezierCurvesNeedsToBeAdjusted() {
    if (edges().size() > 2)
        return true;

    return false;
}

void MyCentroidNode::setNodeParentUsingNeighborNodesParent() {
    MyNetworkElementBase* parenNode = NULL;
    for (MyNetworkElementBase *edge : qAsConst(edges())) {
        if (((MyConnectedToCentroidNodeEdgeBase*)edge)->nonCentroidNodeParent()) {
            if (!parenNode)
                parenNode = ((MyConnectedToCentroidNodeEdgeBase*)edge)->nonCentroidNodeParent();
            else if (parenNode != ((MyConnectedToCentroidNodeEdgeBase*)edge)->nonCentroidNodeParent()) {
                unsetParentNode();
                return;
            }
        }
        else {
            unsetParentNode();
            return;
        }
    }

    setParentNode(parenNode);
}


const QPointF MyCentroidNode::getNodeUpdatedPositionUsingConnectedEdges() {
    QPointF position = QPointF(0.0, 0.0);
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        position += ((MyConnectedToCentroidNodeEdgeBase*)edge)->nonCentroidNodePosition();
    return position / edges().size();
}

const bool MyCentroidNode::isNodePositionConnectedToNeighborNodes() {
    return _isNodePositionConnectedToNeighborNodes;
}

void MyCentroidNode::connectNodePositionToNeighborNodes(const bool& connected) {
    _isNodePositionConnectedToNeighborNodes = connected;
}

void MyCentroidNode::setSelected(const bool& selected) {
    if (selected != isSelected()) {
        MyNodeBase::setSelected(selected);
        for (MyNetworkElementBase *edge : qAsConst(edges()))
            edge->setSelected(selected);
    }
}

void MyCentroidNode::move(const qreal& dx, const qreal& dy) {

}

QWidget* MyCentroidNode::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    addParentFeaturesToFeatureMenu(featureMenu);
    addSpacerItemToFeatureMenu(featureMenu);

    return featureMenu;
}

const bool MyCentroidNode::areAnyOtherElementsSelected() {
    QList<MyNetworkElementBase*> networkElements;
    networkElements.push_back(this);
    for (MyNetworkElementBase* edge : edges())
        networkElements.push_back(edge);
    return askForWhetherAnyOtherElementsAreSelected(networkElements);
}

const QLineF MyCentroidNode::createBezierAdjustLine() {
    QPointF position = getPosition();
    qreal minX = position.x();
    qreal maxX = position.x();
    qreal dx = 0;

    for (MyNetworkElementBase* edge : edges()) {
        QPointF controlPoint = ((MyConnectedToCentroidNodeEdgeBase*)edge)->askForConnectedToCentroidNodeControlPoint();
        qreal maximumAllowedLengthChange = 40.0;
        if (controlPoint.x() < minX) {
            dx = qMin(minX - controlPoint.x(), maximumAllowedLengthChange);
            minX -= dx;
            maxX += dx;
        }
        if (controlPoint.x() > maxX) {
            dx = qMin(controlPoint.x() - maxX, maximumAllowedLengthChange);
            minX -= dx;
            maxX += dx;
        }
    }

    return QLineF(QPointF(minX, position.y()), QPointF(maxX, position.y()));
}

const qint32 MyCentroidNode::calculateNodeZValue() {
    return 5;
}

const qint32 MyCentroidNode::calculateConnectedEdgeZValue() {
    return calculateZValue() - 2;
}