#include "negui_edge.h"

// MyEdge

MyEdge::MyEdge(const QString& name, MyNode* startNode, MyNode* endNode) : MyElementBase(name) {
    _arrowHead = NULL;
    _style = NULL;
    _isSetArrowHead = false;
    _isActive = false;
    _isConnectedToNodes = false;
    _graphicsItem = new MyEdgeSceneGraphicsItem();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(askForElementFeatureMenu()), this, SLOT(getFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForSetShapeStyles(QList<MyShapeStyleBase*>)), this, SLOT(setShapeStyles(QList<MyShapeStyleBase*>)));
    enableNormalMode();
    if (startNode && endNode) {
        _startNode = startNode;
        _startNode->addEdge(this);
        _endNode = endNode;
        _endNode->addEdge(this);
    }
}

MyEdge::~MyEdge() {
    delete _graphicsItem;
    
    if (isSetArrowHead())
        delete _arrowHead;
}

MyEdge::ELEMENT_TYPE MyEdge::type() {
    return EDGE_ELEMENT;
};

const QString MyEdge::typeAsString() const {
    return "Edge";
}

MyNode* MyEdge::startNode() {
    return _startNode;
}

MyNode* MyEdge::endNode() {
    return _endNode;
}

void MyEdge::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    MyElementBase::setShapeStyles(shapeStyles);
    updatePoints();
}

void MyEdge::updateGraphicsItem() {
    MyElementBase::updateGraphicsItem();
    setArrowHead();
}

void MyEdge::setSelected(const bool& selected) {
    graphicsItem()->setSelectedWithStroke(selected);
    if (!selected)
        graphicsItem()->setSelectedWithFill(selected);
        
    if (isSetArrowHead()) {
        arrowHead()->graphicsItem()->setSelectedWithStroke(selected);
        
        if (!selected)
            arrowHead()->graphicsItem()->setSelectedWithFill(selected);
    }
}

MyArrowHead* MyEdge::arrowHead() {
    return _arrowHead;
}

void MyEdge::setArrowHead() {
    if (isSetArrowHead()) {
        delete _arrowHead;
        _isSetArrowHead = false;
    }
            
    if (((MyEdgeStyle*)style())->arrowHeadStyle() && ((MyEdgeStyle*)style())->arrowHeadStyle()->shapeStyles().size()) {
        _arrowHead = new MyArrowHead(name() + "_ArrowHead", this);
        _arrowHead->setStyle(((MyEdgeStyle*)style())->arrowHeadStyle());
        _isSetArrowHead = true;
    }
}

bool MyEdge::setActive(const bool& active) {
    if (active && startNode() && startNode()->isActive() && endNode() && endNode()->isActive()) {
        updatePoints();
        return _isActive = true;
    }
    
    return _isActive = false;
}

bool MyEdge::connectToNodes(const bool& connect) {
    return _isConnectedToNodes = connect;
}

void MyEdge::updatePoints() {
    QPointF startPosition = getStartPosition();
    QPointF endPosition = getEndPosition();

    ((MyEdgeSceneGraphicsItem*)graphicsItem())->setLine(QLineF(startPosition.x(), startPosition.y(), endPosition.x(), endPosition.y()));
    
    if (isSetArrowHead())
        ((MyArrowHeadSceneGraphicsItem*)arrowHead()->graphicsItem())->update(endPosition, ((MyEdgeSceneGraphicsItem*)graphicsItem())->getEndSlope());
}

const QPointF MyEdge::getStartPosition() {
    if (startNode() && endNode()) {
        QPointF startPosition = startNode()->position();
        QPointF endPosition = endNode()->position();
        qreal startRadius = 0.5 * qMax(startNode()->getExtents().width(), startNode()->getExtents().height());
        qreal endRadius = 0.5 * qMax(endNode()->getExtents().width(), endNode()->getExtents().height());
        if (qSqrt(((endPosition - startPosition).x() * (endPosition - startPosition).x()) + ((endPosition - startPosition).y() * (endPosition - startPosition).y())) > (startRadius + endRadius + 10.0)) {
            startRadius += 5;
            endRadius += 5;
        }
        qreal slope = qAtan2(endPosition.y() - startPosition.y(), endPosition.x() - startPosition.x());
        
        return QPointF(startPosition.x() + startRadius * qCos(slope), startPosition.y() + startRadius * qSin(slope));
    }
    
    return QPointF();
}

const QPointF MyEdge::getEndPosition() {
    if (startNode() && endNode()) {
        QPointF startPosition = startNode()->position();
        QPointF endPosition = endNode()->position();
        qreal startRadius = 0.5 * qMax(startNode()->getExtents().width(), startNode()->getExtents().height());
        qreal endRadius = 0.5 * qMax(endNode()->getExtents().width(), endNode()->getExtents().height());
        if (qSqrt(((endPosition - startPosition).x() * (endPosition - startPosition).x()) + ((endPosition - startPosition).y() * (endPosition - startPosition).y())) > (startRadius + endRadius + 10.0)) {
            startRadius += 5;
            endRadius += 5;
        }
        qreal slope = qAtan2(endPosition.y() - startPosition.y(), endPosition.x() - startPosition.x());
        
        return QPointF(endPosition.x() - endRadius * qCos(slope), endPosition.y() - endRadius * qSin(slope));
    }
    
    return QPointF();
}

void MyEdge::enableNormalMode() {
    MyElementBase::enableNormalMode();
    graphicsItem()->setCursor(Qt::ArrowCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableNormalMode();
}

void MyEdge::enableAddNodeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableAddNodeMode();
}

void MyEdge::enableSelectNodeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableSelectNodeMode();
}

void MyEdge::enableAddEdgeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableAddEdgeMode();
}

void MyEdge::enableSelectEdgeMode() {
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableSelectEdgeMode();
}

void MyEdge::enableRemoveMode() {
    graphicsItem()->setCursor(Qt::PointingHandCursor);
    
    // arrow head
    if (isSetArrowHead())
        arrowHead()->enableRemoveMode();
}

QWidget* MyEdge::getFeatureMenu() {
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // source node
    contentLayout->addWidget(new MyLabel("Source"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(startNode()->name()), contentLayout->rowCount() - 1, 1);
    
    // target node
    contentLayout->addWidget(new MyLabel("Target"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(endNode()->name()), contentLayout->rowCount() - 1, 1);
    
    return featureMenu;
}

const qint32 MyEdge::calculateZValue() {
    return qMax(startNode()->calculateZValue(), endNode()->calculateZValue()) - 2;
}

void MyEdge::read(const QJsonObject &json) {
    
}

void MyEdge::write(QJsonObject &json) {
    // id
    json["id"] = name();
    
    QJsonObject positionObject;
    // start node
    QJsonObject startObject;
    if (startNode()) {
        startObject["node"] = startNode()->name();
        
        QJsonObject positionObject;
        positionObject["x"] = getStartPosition().x();
        positionObject["y"] = getStartPosition().y();
        startObject["position"] = positionObject;
        
        json["start"] = startObject;
    }
    
    // end node
    QJsonObject endObject;
    if (endNode()) {
        endObject["node"] = endNode()->name();
        
        QJsonObject positionObject;
        positionObject["x"] = getEndPosition().x();
        positionObject["y"] = getEndPosition().y();
        endObject["position"] = positionObject;
        
        json["end"] = endObject;
    }
    
    // style
    QJsonObject styleObject;
    style()->write(styleObject);
    json["style"] = styleObject;
}
