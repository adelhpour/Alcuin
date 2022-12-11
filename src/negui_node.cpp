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
    _isParentNodeLocked = false;
    _areChildNodesLocked = false;
    _position = QPointF(x, y);
    _graphicsItem = createNodeSceneGraphicsItem(position());
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(displayFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForDeparent()), this,  SLOT(deparent()));
    connect(_graphicsItem, SIGNAL(askForReparent()), this, SLOT(reparent()));
    connect(_graphicsItem, SIGNAL(askForResetPosition()), this, SLOT(resetPosition()));
}

MyNode::~MyNode() {
    delete _graphicsItem;
}

MyNode::ELEMENT_TYPE MyNode::type() {
    return NODE_ELEMENT;
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

bool MyNode::isConnectableTo(MyElementBase* n) {
    return style()->isConnectableTo(n->style()->category());
}

void MyNode::updateGraphicsItem() {
    MyElementBase::updateGraphicsItem();
    resetPosition();
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
    _parentNode = parentNode;
    _isSetParentNode = true;
    _parentNodeId = parentNode->name();
    ((MyNode*)_parentNode)->addChildNode(this);
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

void MyNode::deparent() {
    if (_parentNode) {
        ((MyNode*)_parentNode)->removeChildNode(this);
        ((MyNode*)_parentNode)->adjustExtents();
    }
    _parentNode = NULL;
    _isSetParentNode = false;
    _parentNodeId = "N/A";
}

void MyNode::reparent() {
    MyElementBase* parentNode = askForParentNodeAtPosition(this, position());
    deparent();
    if (parentNode && parentNode->style()->isConvertibleToParentCategory(style()->parentCategories())) {
        parentNode->style()->convertToParentCategory();
        setParentNode((MyNode*)parentNode);
        graphicsItem()->setZValue(calculateZValue());
        ((MyNode*)parentNode)->adjustExtents();
        resetPosition();
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
            ((MyNode*)parentNode())->adjustExtents();
    }
    else
        lockParentNode(false);
    
    // edges
    for (MyElementBase *edge : qAsConst(edges()))
        ((MyEdge*)edge)->updatePoints();
}

void MyNode::resetPosition() {
    setPosition(((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents().center());
}

const QPointF MyNode::position() const {
    return _position;
}

const QRectF MyNode::getExtents() {
    if (childNodes().size()) {
        QRectF childExtents = ((MyNode*)childNodes().at(0))->getExtents();
        qreal extentsX = childExtents.x();
        qreal extentsY = childExtents.y();
        qreal extentsWidth = childExtents.width();
        qreal extentsHeight = childExtents.height();
        for (MyElementBase* childNode : qAsConst(childNodes())) {
            childExtents = ((MyNode*)childNode)->getExtents();
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
    
    return ((MyNodeSceneGraphicsItem*)graphicsItem())->getExtents();
}

void MyNode::adjustExtents() {
    QRectF extents = getExtents();
    lockChildNodes(true);
    ((MyNodeSceneGraphicsItem*)graphicsItem())->moveBy(extents.x() - (position().x() - 0.5 * extents.width()), extents.y() - (position().y() - 0.5 * extents.height()));
    ((MyNodeSceneGraphicsItem*)graphicsItem())->updateExtents(extents);
    ((MyNodeSceneGraphicsItem*)graphicsItem())->adjustOriginalPosition();
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
    if (json.contains("style") && json["style"].isObject())
        style()->read(json["style"].toObject());
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
