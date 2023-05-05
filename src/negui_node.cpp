#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_style.h"
#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include <QJsonObject>

// MyNodeBase

MyNodeBase::MyNodeBase(const QString& name, const qreal& x, const qreal& y) : MyElementBase(name) {
    _parentNodeId = "N/A";
    _parentNode = NULL;
    _isSetParentNode = false;
    _isParentNodeLocked = false;
    _position = QPointF(x, y);
    _endEdgePadding = 0.0;
}

MyNodeBase::~MyNodeBase() {
    delete _graphicsItem;
}

MyNodeBase::ELEMENT_TYPE MyNodeBase::type() {
    return NODE_ELEMENT;
}

void MyNodeBase::connectGraphicsItem() {
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(createFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForDeparent()), this,  SLOT(deparent()));
    connect(_graphicsItem, SIGNAL(askForReparent()), this, SLOT(reparent()));
    connect(_graphicsItem, SIGNAL(askForResetPosition()), this, SLOT(resetPosition()));
    connect(_graphicsItem, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
}

void MyNodeBase::addEdge(MyElementBase* e) {
    if (e)
        _edges.push_back(e);
}

void MyNodeBase::removeEdge(MyElementBase* e) {
    if (e)
        _edges.removeOne(e);
}

QList<MyElementBase*>& MyNodeBase::edges() {
    return _edges;
}

void MyNodeBase::updateGraphicsItem() {
    MyElementBase::updateGraphicsItem();
    resetPosition();
}

void MyNodeBase::setSelected(const bool& selected) {
    MyElementBase::setSelected(selected);
    if (selected)
        graphicsItem()->setSelectedWithFill(selected);
    else {
        graphicsItem()->setSelectedWithStroke(selected);
        graphicsItem()->setSelectedWithFill(selected);
    }
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
    MyElementBase* parentNode = askForParentNodeAtPosition(this, position());
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
    for (MyElementBase *edge : qAsConst(edges()))
        ((MyEdgeBase*)edge)->updatePoints();
}

const QString& MyNodeBase::parentNodeId() const {
    return _parentNodeId;
}

void MyNodeBase::setParentNodeId(const QString& parentNodeId) {
    _parentNodeId = parentNodeId;
}

MyElementBase* MyNodeBase::parentNode() {
    return _parentNode;
}

void MyNodeBase::setParentNode(MyElementBase* parentNode) {
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
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
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
    MyElementBase* parent = this;
    while (parent && ((MyNodeBase*)parent)->parentNodeId() != "N/A") {
        incrementZValue += 4;
        parent = ((MyNodeBase*)parent)->parentNode();
    }

    return incrementZValue;
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

MyElementGraphicsItemBase* MyClassicNode::createGraphicsItem(const QPointF &position) {
    return createClassicNodeSceneGraphicsItem(position);
}

void MyClassicNode::addChildNode(MyElementBase* n) {
    if (n) {
        _childNodes.push_back(n);
        updateChildNodesMobility();
    }
}

void MyClassicNode::removeChildNode(MyElementBase* n) {
    if (n) {
        _childNodes.removeOne(n);
        updateChildNodesMobility();
    }
}

QList<MyElementBase*>& MyClassicNode::childNodes() {
    return _childNodes;
}

void MyClassicNode::lockChildNodes(const bool& locked) {
    _areChildNodesLocked = locked;
}

void MyClassicNode::updateChildNodesMobility() {
    if (childNodes().size() > 1) {
        for (MyElementBase* node : qAsConst(childNodes()))
            node->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    else if (childNodes().size() == 1)
        childNodes().first()->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyClassicNode::setPosition(const QPointF& position) {
    // move child nodes
    if (!areChildNodesLocked()) {
        for (MyElementBase* node : qAsConst(childNodes())) {
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
        for (MyElementBase* childNode : qAsConst(childNodes())) {
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
    _graphicsItem = createGraphicsItem(position());
    connectGraphicsItem();
}

MyNodeBase::NODE_TYPE MyCentroidNode::nodeType() {
    return CENTROID_NODE;
}

MyElementGraphicsItemBase* MyCentroidNode::createGraphicsItem(const QPointF &position) {
    return createCentroidNodeSceneGraphicsItem(position);
}

void MyCentroidNode::addEdge(MyElementBase* e) {
    MyNodeBase::addEdge(e);
    connect(e, SIGNAL(askForAdjustConnectedEdges(const QPointF&)), this, SLOT(adjustConnectedEdges(const QPointF&)));
}

void MyCentroidNode::removeEdge(MyElementBase* e) {
    MyNodeBase::removeEdge(e);
    disconnect(e, SIGNAL(askForAdjustConnectedEdges(const QPointF&)), this, SLOT(adjustConnectedEdges(const QPointF&)));
}

void MyCentroidNode::adjustConnectedEdges(const QPointF& updatedPoint) {
    emit controlBezierLineIsUpdated(createControlBezierLine(updatedPoint));
}

const QLineF MyCentroidNode::createControlBezierLine(const QPointF& updatedPoint) {
    return adjustControlBezierLine(QLineF(updatedPoint, 2 * position() - updatedPoint));
}

const QLineF MyCentroidNode::adjustControlBezierLine(const QLineF& controlBezierLine) {
    // x
    qreal adjustedStartPointX = getAdjustControlBezierLineStartPointX(controlBezierLine);
    qreal adjustedEndPointX = getAdjustControlBezierLineEndPointX(controlBezierLine);

    qreal adjustmentLengthX = getControlBezierLineAdjustmentLengthX(adjustedStartPointX, adjustedEndPointX);
    adjustedStartPointX -= 0.5 * adjustmentLengthX;
    adjustedEndPointX += 0.5 * adjustmentLengthX;

    // y
    qreal adjustedStartPointY = getAdjustControlBezierLineStartPointY(controlBezierLine);
    qreal adjustedEndPointY = getAdjustControlBezierLineEndPointY(controlBezierLine);

    qreal adjustmentLengthY = getControlBezierLineAdjustmentLengthY(adjustedStartPointY, adjustedEndPointY);
    adjustedStartPointX -= 0.5 * adjustmentLengthY;
    adjustedEndPointX += 0.5 * adjustmentLengthY;

    return QLineF(adjustedStartPointX, adjustedStartPointY, adjustedEndPointX, adjustedEndPointY);
}

const qreal MyCentroidNode::getAdjustControlBezierLineStartPointX(const QLineF& controlBezierLine) {
    if (controlBezierLine.p1().x() < controlBezierLine.p2().x())
        return controlBezierLine.p1().x();
    else
        return controlBezierLine.p2().x();
}

const qreal MyCentroidNode::getAdjustControlBezierLineEndPointX(const QLineF& controlBezierLine) {
    if (controlBezierLine.p1().x() < controlBezierLine.p2().x())
        return controlBezierLine.p2().x();
    else
        return controlBezierLine.p1().x();
}

const qreal MyCentroidNode::getControlBezierLineAdjustmentLengthX(const qreal& adjustedStartPointX, const qreal& adjustedEndPointX) {
    qreal minimumLength = 5.0;
    qreal maximumLength = 100.0;

    if ((adjustedEndPointX - adjustedStartPointX) < minimumLength)
        return minimumLength - (adjustedEndPointX - adjustedStartPointX);
    else if ((adjustedEndPointX - adjustedStartPointX) > maximumLength)
        return -((adjustedEndPointX - adjustedStartPointX) - maximumLength);

    return 0.0;
}

const qreal MyCentroidNode::getAdjustControlBezierLineStartPointY(const QLineF& controlBezierLine) {
    if (controlBezierLine.p1().y() < controlBezierLine.p2().y())
        return controlBezierLine.p1().y();
    else
        return controlBezierLine.p2().y();
}

const qreal MyCentroidNode::getAdjustControlBezierLineEndPointY(const QLineF& controlBezierLine) {
    if (controlBezierLine.p1().y() < controlBezierLine.p2().y())
        return controlBezierLine.p2().y();
    else
        return controlBezierLine.p1().y();
}

const qreal MyCentroidNode::getControlBezierLineAdjustmentLengthY(const qreal& adjustedStartPointY, const qreal& adjustedEndPointY) {
    qreal minimumLength = 5.0;
    qreal maximumLength = 100.0;

    if ((adjustedEndPointY - adjustedStartPointY) < minimumLength)
        return minimumLength - (adjustedEndPointY - adjustedStartPointY);
    else if ((adjustedEndPointY - adjustedStartPointY) > maximumLength)
        return -((adjustedEndPointY - adjustedStartPointY) - maximumLength);

    return 0.0;
}