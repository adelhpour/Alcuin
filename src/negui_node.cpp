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
    connect(_graphicsItem, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SLOT(adjustConnectedEdges()));
    connect((MyNodeSceneGraphicsItemBase*)_graphicsItem, &MyNodeSceneGraphicsItemBase::positionChangedByMouseMoveEvent, this, [this] (const QPointF& movedDistance) { positionChangedByMouseMoveEvent(this, movedDistance); });
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

void MyNodeBase::deparent() {
    if (_parentNode) {
        ((MyComplexClassicNode*)_parentNode)->removeChildNode(this);
        ((MyComplexClassicNode*)_parentNode)->adjustExtents();
    }
    _parentNode = NULL;
    _isSetParentNode = false;
    _parentNodeId = "N/A";
}

void MyNodeBase::reparent() {
    MyNetworkElementBase* parentNode = askForParentNodeAtPosition(this, position());
    deparent();
    if (parentNode && ((MyComplexClassicNodeStyle*)parentNode->style())->isConvertibleToParentCategory(((MyNodeStyleBase*)style())->parentCategories())) {
        ((MyComplexClassicNodeStyle*)parentNode->style())->convertToParentCategory();
        setParentNode((MyNodeBase*)parentNode);
        ((MyComplexClassicNode*)parentNode)->adjustExtents();
        resetPosition();
    }
}

void MyNodeBase::setPosition(const QPointF& position) {
    // position
    _position = position;

    // adjust parent extents
    if (!isParentNodeLocked()) {
        if (parentNode())
            ((MyComplexClassicNode*)parentNode())->adjustExtents();
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
    ((MyComplexClassicNode*)_parentNode)->addChildNode(this);
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

void MyNodeBase::addParentFeaturesToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QString parentLabel = "Parent";
    if (!((MyNodeStyleBase*)style())->parentTitle().isEmpty())
        parentLabel = ((MyNodeStyleBase*)style())->parentTitle();
    contentLayout->addWidget(new MyLabel(parentLabel), contentLayout->rowCount(), 0, Qt::AlignLeft);
    contentLayout->addWidget(new MyReadOnlyLineEdit(parentNodeId()), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
}

void MyNodeBase::addSpacerItemToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
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

// MyClassicNodeBase

MyClassicNodeBase::MyClassicNodeBase(const QString& name, const qreal& x, const qreal& y) : MyNodeBase(name, x, y) {
    _areChildNodesLocked = false;
    _endEdgePadding = 5.0;
}

const bool MyClassicNodeBase::isCopyable() {
    if (isSelected())
        return true;

    return false;
}

void MyClassicNodeBase::addChildNode(MyNetworkElementBase* n) {
    if (n) {
        _childNodes.push_back(n);
        updateChildNodesMobility();
    }
}

void MyClassicNodeBase::removeChildNode(MyNetworkElementBase* n) {
    if (n) {
        _childNodes.removeOne(n);
        updateChildNodesMobility();
    }
}

QList<MyNetworkElementBase*>& MyClassicNodeBase::childNodes() {
    return _childNodes;
}

void MyClassicNodeBase::lockChildNodes(const bool& locked) {
    _areChildNodesLocked = locked;
}

void MyClassicNodeBase::updateChildNodesMobility() {
    if (childNodes().size() > 1) {
        for (MyNetworkElementBase* node : qAsConst(childNodes()))
            node->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    else if (childNodes().size() == 1)
        childNodes().first()->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyClassicNodeBase::setPosition(const QPointF& position) {
    // move child nodes
    if (!areChildNodesLocked()) {
        for (MyNetworkElementBase* node : qAsConst(childNodes())) {
            ((MyNodeBase*)node)->lockParentNode(true);
            ((MyComplexClassicNodeSceneGraphicsItem*)node->graphicsItem())->moveBy((position - _position).x(), (position - _position).y());
        }
    }
    else
        lockChildNodes(false);

    MyNodeBase::setPosition(position);
}

const QRectF MyClassicNodeBase::getExtents() {
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
    qreal minimumWidth = 400.0;
    qreal minimumHeight = 300.0;
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
    lockChildNodes(true);
    ((MyComplexClassicNodeSceneGraphicsItem*)graphicsItem())->moveBy(extents.x() - (position().x() - 0.5 * extents.width()), extents.y() - (position().y() - 0.5 * extents.height()));
    ((MyComplexClassicNodeSceneGraphicsItem*)graphicsItem())->updateExtents(extents);
    ((MyComplexClassicNodeSceneGraphicsItem*)graphicsItem())->adjustOriginalPosition();
}

// MySimpleClassicNode

MySimpleClassicNode::MySimpleClassicNode(const QString& name, const qreal& x, const qreal& y) : MyClassicNodeBase(name, x, y) {
    _displayName = name;
    _graphicsItem = createGraphicsItem(position());
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
        contentLayout->addWidget(new MyLabel("Display Name"), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(plainTextParameter->inputWidget(), contentLayout->rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    }
}

void MySimpleClassicNode::addChangeShapeStyleButtonToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QWidget* shapeStylesButtons = ((MySimpleClassicNodeStyle*)style())->shapeStylesButtons();
    connect(shapeStylesButtons, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)));
    contentLayout->addWidget(shapeStylesButtons, contentLayout->rowCount(), 0, 1, 2, Qt::AlignRight);
}

// MyComplexClassicNode

MyComplexClassicNode::MyComplexClassicNode(const QString& name, const qreal& x, const qreal& y) : MyClassicNodeBase(name, x, y) {
    _graphicsItem = createGraphicsItem(position());
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

void MyComplexClassicNode::addAddRemoveShapeStyleButtonsToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QWidget* shapeStylesButtons = ((MyComplexClassicNodeStyle*)style())->shapeStylesButtons();
    connect(shapeStylesButtons, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)));
    connect(shapeStylesButtons, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)));
    connect(featureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), shapeStylesButtons, SLOT(setRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(shapeStylesButtons, contentLayout->rowCount(), 0, 1, 2, Qt::AlignRight);
}

// MyCentroidNode

MyCentroidNode::MyCentroidNode(const QString& name, const qreal& x, const qreal& y) : MyNodeBase(name, x, y) {
    _isNodePositionConnectedToNeighborNodes = true;
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
        if (!edge->isSelected())
            return false;
    }

    return true;
}

void MyCentroidNode::addEdge(MyNetworkElementBase* e) {
    MyNodeBase::addEdge(e);
    connect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes()), this, SLOT(adjustNodePositionToNeighborNodes()));
    connect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setSelected(const bool&)));
}

void MyCentroidNode::removeEdge(MyNetworkElementBase* e) {
    MyNodeBase::removeEdge(e);
    disconnect(e, SIGNAL(askForAdjustNodePositionToNeighborNodes()), this, SLOT(adjustNodePositionToNeighborNodes()));
    disconnect(e, SIGNAL(askForSetConnectedElementsSelected(const bool&)), this, SLOT(setSelected(const bool&)));
}

void MyCentroidNode::adjustNodePositionToNeighborNodes() {
    if (isNodePositionConnectedToNeighborNodes() && edges().size()) {
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

QWidget* MyCentroidNode::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    addParentFeaturesToFeatureMenu(featureMenu);
    addSpacerItemToFeatureMenu(featureMenu);

    return featureMenu;
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