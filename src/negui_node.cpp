#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_style.h"
#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include "negui_customized_common_widgets.h"

#include <QJsonObject>
#include <QGridLayout>

// MyNodeBase

MyNodeBase::MyNodeBase(const QString& name, const qreal& x, const qreal& y) : MyNetworkElementBase(name) {
    _parentNodeId = "N/A";
    _parentNode = NULL;
    _isSetParentNode = false;
    _isParentNodeLocked = false;
    _position = QPointF(x, y);
    _endEdgePadding = 0.0;
}

MyNodeBase::ELEMENT_TYPE MyNodeBase::type() {
    return NODE_ELEMENT;
}

void MyNodeBase::connectGraphicsItem() {
    MyNetworkElementBase::connectGraphicsItem();
    connect(_graphicsItem, SIGNAL(askForDeparent()), this,  SLOT(deparent()));
    connect(_graphicsItem, SIGNAL(askForReparent()), this, SLOT(reparent()));
    connect(_graphicsItem, SIGNAL(askForResetPosition()), this, SLOT(resetPosition()));
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent()), this, SLOT(adjustConnectedEdges()));
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
    resetPosition();
}

void MyNodeBase::setSelectedWithColor(const bool& selected) {
    if (selected)
        graphicsItem()->setSelectedWithFill(selected);
    else {
        graphicsItem()->setSelectedWithStroke(selected);
        graphicsItem()->setSelectedWithFill(selected);
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

void MyNodeBase::deparent() {
    if (_parentNode) {
        ((MyClassicNode*)_parentNode)->removeChildNode(this);
        ((MyClassicNode*)_parentNode)->adjustExtents();
    }
    _parentNode = NULL;
    _isSetParentNode = false;
    _parentNodeId = "N/A";
}

void MyNodeBase::reparent() {
    MyNetworkElementBase* parentNode = askForParentNodeAtPosition(this, position());
    deparent();
    if (parentNode && ((MyClassicNodeStyle*)parentNode->style())->isConvertibleToParentCategory(((MyNodeStyleBase*)style())->parentCategories())) {
        ((MyClassicNodeStyle*)parentNode->style())->convertToParentCategory();
        setParentNode((MyNodeBase*)parentNode);
        ((MyClassicNode*)parentNode)->adjustExtents();
        resetPosition();
    }
}

void MyNodeBase::setPosition(const QPointF& position) {
    // position
    _position = position;

    // adjust parent extents
    if (!isParentNodeLocked()) {
        if (parentNode())
            ((MyClassicNode*)parentNode())->adjustExtents();
    }
    else
        lockParentNode(false);

    // edges
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase*)edge)->updatePoints();
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
    ((MyClassicNode*)_parentNode)->addChildNode(this);
    graphicsItem()->setZValue(calculateZValue());
}

void MyNodeBase::lockParentNode(const bool& locked) {
    _isParentNodeLocked = locked;
}

void MyNodeBase::resetPosition() {
    setPosition(((MyNodeSceneGraphicsItemBase*)graphicsItem())->getExtents().center());
}

const QPointF MyNodeBase::position() const {
    return _position;
}

const QRectF MyNodeBase::getExtents() {
    return ((MyNodeSceneGraphicsItemBase*)graphicsItem())->getExtents();
}

const qreal MyNodeBase::endEdgePadding() {
    return  _endEdgePadding;
}

QWidget* MyNodeBase::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // parent
    contentLayout->addWidget(new MyLabel("Parent"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(parentNodeId()), contentLayout->rowCount() - 1, 1);

    // spacer
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);

    return featureMenu;
}

const qint32 MyNodeBase::calculateZValue() {
    qint32 incrementZValue = 2;
    MyNetworkElementBase* parent = this;
    while (parent && ((MyNodeBase*)parent)->parentNodeId() != "N/A") {
        incrementZValue += 4;
        parent = ((MyNodeBase*)parent)->parentNode();
    }

    return incrementZValue;
}

void MyNodeBase::adjustConnectedEdges() {
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase *) edge)->askForAdjustNodePositionToNeighborNodes();
}

void MyNodeBase::read(const QJsonObject &json) {
    // position
    if (json.contains("position") && json["position"].isObject()) {
        if (json["position"].toObject().contains("x") && json["position"]["x"].isDouble() && json["position"].toObject().contains("y") && json["position"]["y"].isDouble()) {
            setPosition(QPointF(json["position"]["x"].toDouble(), json["position"]["y"].toDouble()));
        }
    }

    // parent
    if (json.contains("parent") && json["parent"].isString())
        setParentNodeId(json["parent"].toString());

    // style
    if (json.contains("style") && json["style"].isObject())
        style()->read(json["style"].toObject());
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

// MyClassicNode

MyClassicNode::MyClassicNode(const QString& name, const qreal& x, const qreal& y) : MyNodeBase(name, x, y) {
    _areChildNodesLocked = false;
    _graphicsItem = createGraphicsItem(position());
    connectGraphicsItem();
    _endEdgePadding = 5.0;
}

MyNodeBase::NODE_TYPE MyClassicNode::nodeType() {
    return CLASSIC_NODE;
}

MyNetworkElementGraphicsItemBase* MyClassicNode::createGraphicsItem(const QPointF &position) {
    return createClassicNodeSceneGraphicsItem(position);
}

const bool MyClassicNode::isCopyable() {
    if (isSelected())
        return true;

    return false;
}

void MyClassicNode::addChildNode(MyNetworkElementBase* n) {
    if (n) {
        _childNodes.push_back(n);
        updateChildNodesMobility();
    }
}

void MyClassicNode::removeChildNode(MyNetworkElementBase* n) {
    if (n) {
        _childNodes.removeOne(n);
        updateChildNodesMobility();
    }
}

QList<MyNetworkElementBase*>& MyClassicNode::childNodes() {
    return _childNodes;
}

void MyClassicNode::lockChildNodes(const bool& locked) {
    _areChildNodesLocked = locked;
}

void MyClassicNode::updateChildNodesMobility() {
    if (childNodes().size() > 1) {
        for (MyNetworkElementBase* node : qAsConst(childNodes()))
            node->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    else if (childNodes().size() == 1)
        childNodes().first()->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyClassicNode::setPosition(const QPointF& position) {
    // move child nodes
    if (!areChildNodesLocked()) {
        for (MyNetworkElementBase* node : qAsConst(childNodes())) {
            ((MyNodeBase*)node)->lockParentNode(true);
            ((MyClassicNodeSceneGraphicsItem*)node->graphicsItem())->moveBy((position - _position).x(), (position - _position).y());
        }
    }
    else
        lockChildNodes(false);

    MyNodeBase::setPosition(position);
}

const QRectF MyClassicNode::getExtents() {
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
        return QRectF(extentsX - 10.0, extentsY - 10.0, extentsWidth + 20.0, extentsHeight + 20.0);
    }

    return MyNodeBase::getExtents();
}

void MyClassicNode::adjustExtents() {
    QRectF extents = getExtents();
    lockChildNodes(true);
    ((MyClassicNodeSceneGraphicsItem*)graphicsItem())->moveBy(extents.x() - (position().x() - 0.5 * extents.width()), extents.y() - (position().y() - 0.5 * extents.height()));
    ((MyClassicNodeSceneGraphicsItem*)graphicsItem())->updateExtents(extents);
    ((MyClassicNodeSceneGraphicsItem*)graphicsItem())->adjustOriginalPosition();
}

QWidget* MyClassicNode::getFeatureMenu() {
    QWidget* featureMenu = MyNodeBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // add remove buttons
    QWidget* addRemoveShapeStylesButtons = ((MyClassicNodeStyle*)style())->addRemoveShapeStylesButtons();
    connect(addRemoveShapeStylesButtons, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)));
    connect(addRemoveShapeStylesButtons, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)));
    connect(featureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), addRemoveShapeStylesButtons, SLOT(setRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(addRemoveShapeStylesButtons, contentLayout->rowCount(), 1);

    return featureMenu;
}

// MyCentroidNode

MyCentroidNode::MyCentroidNode(const QString& name, const qreal& x, const qreal& y) : MyNodeBase(name, x, y) {
    _doesNodePositionDependOnNeighboringNodes = true;
    _graphicsItem = createGraphicsItem(position());
    connectGraphicsItem();
}

MyNodeBase::NODE_TYPE MyCentroidNode::nodeType() {
    return CENTROID_NODE;
}

void MyCentroidNode::connectGraphicsItem() {
    MyNodeBase::connectGraphicsItem();
    connect(_graphicsItem, SIGNAL(askForGetBezierAdjustLine()), this, SLOT(createBezierAdjustLine()));
    connect(_graphicsItem, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), this, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)));
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent()), this, SLOT(adjustConnectedBezierCurves()));
}

MyNetworkElementGraphicsItemBase* MyCentroidNode::createGraphicsItem(const QPointF &position) {
    return createCentroidNodeSceneGraphicsItem(position);
}

const bool MyCentroidNode::isCopyable() {
    if (!isSelected())
        return false;
    for (MyNetworkElementBase* edge : edges()) {
        if (!edge->isSelected())
            return false;
    }

    return true;
}

void MyCentroidNode::addEdge(MyNetworkElementBase* e) {
    MyNodeBase::addEdge(e);
    connect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes()), this, SLOT(adjustNodePositionToNeighborNodes()));
    connect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setConnectedElementsSelected(const bool&)));
}

void MyCentroidNode::removeEdge(MyNetworkElementBase* e) {
    MyNodeBase::removeEdge(e);
    disconnect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes()), this, SLOT(adjustNodePositionToNeighborNodes()));
    disconnect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setConnectedElementsSelected(const bool&)));
}

void MyCentroidNode::adjustNodePositionToNeighborNodes() {
    if (_doesNodePositionDependOnNeighboringNodes && edges().size()) {
        QPointF updatedPosition = getNodeUpdatedPositionUsingConnectedEdges();
        ((MyCentroidNodeSceneGraphicsItem*)graphicsItem())->moveBy((updatedPosition - _position).x(), (updatedPosition - _position).y());
        adjustConnectedBezierCurves();
    }
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

const QPointF MyCentroidNode::getNodeUpdatedPositionUsingConnectedEdges() {
    QPointF position = QPointF(0.0, 0.0);
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        position += ((MyEdgeBase*)edge)->middlePosition();
    return position /= edges().size();
}

void MyCentroidNode::disconnectNodePositionFromNeighborNodes() {
    _doesNodePositionDependOnNeighboringNodes = false;
}

void MyCentroidNode::setConnectedElementsSelected(const bool& isSelected) {
    setSelected(isSelected);
    for (MyNetworkElementBase *edge : qAsConst(edges()))
        edge->setSelected(isSelected);
}

const QLineF MyCentroidNode::createBezierAdjustLine() {
    QPointF startPoint = position();
    QPointF endPoint = position();
    qreal dx = 0;
    qreal dy = 0;

    for (MyNetworkElementBase* edge : edges()) {
        QPointF controlPoint = ((MyConnectedToCentroidNodeEdgeBase*)edge)->askForConnectedToCentroidNodeControlPoint();
        qreal maximumAllowedLengthChange = 40.0;
        if (controlPoint.x() < startPoint.x()) {
            dx = qMin(startPoint.x() - controlPoint.x(), maximumAllowedLengthChange);
            startPoint.setX(startPoint.x() - dx);
            endPoint.setX(endPoint.x() + dx);
        }
        if (controlPoint.y() < startPoint.y()) {
            dy = qMin(startPoint.y() - controlPoint.y(), maximumAllowedLengthChange);
            startPoint.setY(startPoint.y() - dy);
            endPoint.setY(endPoint.y() + dy);
        }
        if (controlPoint.x() > endPoint.x()) {
            dx = qMin(controlPoint.x() - endPoint.x(), maximumAllowedLengthChange);
            startPoint.setX(startPoint.x() - dx);
            endPoint.setX(endPoint.x() + dx);
        }
        if (controlPoint.y() > endPoint.y()) {
            dy = qMin(controlPoint.y() - endPoint.y(), maximumAllowedLengthChange);
            startPoint.setY(startPoint.y() - dy);
            endPoint.setY(endPoint.y() + dy);
        }
    }

    return QLineF(startPoint, endPoint);
}