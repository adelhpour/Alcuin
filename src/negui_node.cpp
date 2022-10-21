#include "negui_node.h"
#include "negui_edge.h"
#include "negui_element_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include <QJsonObject>

// MyNode

MyNode::MyNode(const QString& name, const qreal& x, const qreal& y) : MyElementBase(name) {
    _parentNodeId = "N/A";
    _parentNode = NULL;
    _isSetParentNode = false;
    _style = NULL;
    _isActive = false;
    _isParentNodeLocked = false;
    _areChildNodesLocked = false;
    _position = QPointF(x, y);
    _graphicsItem = createNodeSceneGraphicsItem(position());
    enableNormalMode();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(askForDeparent()), this,  SLOT(deparent()));
    connect(_graphicsItem, SIGNAL(askForReparent()), this, SLOT(reparent()));
    connect(_graphicsItem, SIGNAL(positionChanged(QPointF)), this, SLOT(setPosition(QPointF)));
    connect(_graphicsItem, SIGNAL(extentsModified()), this, SLOT(resetPosition()));
    connect(_graphicsItem, SIGNAL(askForElementFeatureMenu()), this, SLOT(getFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForSetShapeStyles(QList<MyShapeStyleBase*>)), this, SLOT(setShapeStyles(QList<MyShapeStyleBase*>)));
}

MyNode::~MyNode() {
    delete _graphicsItem;
}

MyNode::ELEMENT_TYPE MyNode::type() {
    return NODE_ELEMENT;
}

const QString MyNode::typeAsString() const {
    return "Node";
}

void MyNode::addEdge(MyElementBase* e) {
    if (e)
        _edges.push_back(e);
}

void MyNode::removeEdge(MyElementBase* e) {
    if (e)
        _edges.removeOne(e);
}

QList<MyElementBase*>& MyNode::edges() {
    return _edges;
}

void MyNode::setSelected(const bool& selected) {
    if (selected)
        graphicsItem()->setSelectedWithFill(selected);
    else {
        graphicsItem()->setSelectedWithStroke(selected);
        graphicsItem()->setSelectedWithFill(selected);
    }
}

const QString& MyNode::parentNodeId() const {
    return _parentNodeId;
}

void MyNode::setParentNodeId(const QString& parentNodeId) {
    _parentNodeId = parentNodeId;
}

MyElementBase* MyNode::parentNode() {
    return _parentNode;
}

void MyNode::setParentNode(MyElementBase* parentNode) {
    if (parentNode) {
        _parentNode = parentNode;
        _isSetParentNode = true;
        _parentNodeId = parentNode->name();
        ((MyNode*)_parentNode)->addChildNode(this);
    }
    else {
        if (_parentNode) {
            ((MyNode*)_parentNode)->removeChildNode(this);
            ((MyNode*)_parentNode)->adjustExtentsTochildren();
        }
        _parentNode = NULL;
        _isSetParentNode = false;
        _parentNodeId = "N/A";
    }
}

void MyNode::lockParentNode(const bool& locked) {
    _isParentNodeLocked = locked;
}

void MyNode::addChildNode(MyElementBase* n) {
    if (n) {
        _childNodes.push_back(n);
        updateChildNodesMobility();
    }
}

void MyNode::removeChildNode(MyElementBase* n) {
    if (n) {
        _childNodes.removeOne(n);
        updateChildNodesMobility();
    }
}

QList<MyElementBase*>& MyNode::childNodes() {
    return _childNodes;
}

void MyNode::lockChildNodes(const bool& locked) {
    _areChildNodesLocked = locked;
}

void MyNode::updateChildNodesMobility() {
    if (childNodes().size() > 1) {
        for (MyElementBase* node : qAsConst(childNodes()))
            node->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
    else if (childNodes().size() == 1)
        childNodes().first()->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNode::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    MyElementBase::setShapeStyles(shapeStyles);
    resetPosition();
}

void MyNode::deparent() {
    setParentNode(NULL);
}

void MyNode::reparent() {
    MyElementBase* parentNode = askForParentNodeAtPosition(this, position());
    setParentNode(NULL);
    if (parentNode) {
        setParentNode((MyNode*)parentNode);
        graphicsItem()->setZValue(calculateZValue());
        ((MyNode*)parentNode)->adjustExtentsTochildren();
    }
}

void MyNode::setPosition(const QPointF& position) {
    // move child nodes
    if (!areChildNodesLocked()) {
        for (MyElementBase* node : qAsConst(childNodes())) {
            ((MyNode*)node)->lockParentNode(true);
            ((MyNodeSceneGraphicsItem*)node->graphicsItem())->moveBy((position - _position).x(), (position - _position).y());
        }
    }
    else
        lockChildNodes(false);
    
    // position
    _position = position;
    
    // adjust parent extents
    if (!isParentNodeLocked()) {
        if (parentNode())
            ((MyNode*)parentNode())->adjustExtentsTochildren();
    }
    else
        lockParentNode(false);
    
    // edges
    for (MyElementBase *edge : qAsConst(edges()))
        ((MyEdge*)edge)->updatePoints();
}

void MyNode::resetPosition() {
    setPosition(position());
}

const QPointF MyNode::position() const {
    return _position;
}

const QRectF MyNode::getExtents() {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    if (childNodes().size()) {
        QRectF childExtents;
        for (MyElementBase* childNode : qAsConst(childNodes())) {
            childExtents = ((MyNode*)childNode)->getExtents();
            if (childExtents.x() < extents.x())
                extents.setX(childExtents.x());
            if (childExtents.y() < extents.y())
                extents.setY(childExtents.y());
            if (extents.x() + extents.width() < childExtents.x() + childExtents.width())
                extents.setWidth(extents.width() + (childExtents.x() + childExtents.width() - extents.x() - extents.width()));
            if (extents.y() + extents.height() < childExtents.y() + childExtents.height())
                extents.setHeight(extents.height() + (childExtents.y() + childExtents.height() - extents.y() - extents.height()));
        }
    }
    else
        return QRectF(position().x() - 0.5 * ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().width(), position().y() - 0.5 * ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().height(), ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().width(), ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().height());
    
    return QRectF(extents.x() - 10.0, extents.y() - 10.0, extents.width() + 20.0, extents.height() + 20.0);
}

void MyNode::adjustExtentsTochildren() {
    QRectF extents = getExtents();
    lockChildNodes(true);
    ((MyNodeSceneGraphicsItem*)graphicsItem())->moveBy(extents.x() - (position().x() - 0.5 * ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().width()), extents.y() - (position().y() - 0.5 * ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().height()));
    ((MyNodeSceneGraphicsItem*)graphicsItem())->updateExtents(extents);
}

void MyNode::enableNormalMode() {
    MyElementBase::enableNormalMode();
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void MyNode::enableAddNodeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNode::enableSelectNodeMode() {
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNode::enableAddEdgeMode() {
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNode::enableSelectEdgeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNode::enableRemoveMode() {
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
}

QWidget* MyNode::getFeatureMenu() {
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // parent
    contentLayout->addWidget(new MyLabel("Parent"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(parentNodeId()), contentLayout->rowCount() - 1, 1);
    
    return featureMenu;
}

const qint32 MyNode::calculateZValue() {
    qint32 incrementZValue = 2;
    MyElementBase* parent = this;
    while (parent && ((MyNode*)parent)->parentNodeId() != "N/A") {
        incrementZValue += 4;
        parent = ((MyNode*)parent)->parentNode();
    }
    
    return incrementZValue;
}

void MyNode::read(const QJsonObject &json) {
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
    if (json.contains("style") && json["style"].isObject()) {
        style()->read(json["style"].toObject());
        updateGraphicsItem();
    }
}

void MyNode::write(QJsonObject &json) {
    // id
    json["id"] = name();
    
    // position
    QJsonObject positionObject;
    positionObject["x"] = position().x();
    positionObject["y"] = position().y();
    json["position"] = positionObject;
    
    // dimensions
    QJsonObject dimensionsObject;
    dimensionsObject["width"] = getExtents().width();
    dimensionsObject["height"] = getExtents().height();
    json["dimensions"] = dimensionsObject;
    
    // parent node
    if (parentNodeId() != "N/A")
        json["parent"] = parentNodeId();
    
    // style
    QJsonObject styleObject;
    style()->write(styleObject);
    json["style"] = styleObject;
}
