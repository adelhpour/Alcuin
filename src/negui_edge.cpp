#include "negui_edge.h"
#include "negui_node.h"
#include "negui_arrow_head.h"
#include "negui_arrow_head_builder.h"
#include "negui_edge_graphics_item_builder.h"
#include "negui_edge_style.h"
#include "negui_edge_graphics_item.h"
#include "negui_customized_common_widgets.h"

#include <QtMath>
#include <QJsonObject>
#include <QGridLayout>

// MyEdgeBase

MyEdgeBase::MyEdgeBase(const QString& name, MyNetworkElementBase* startNode, MyNetworkElementBase* endNode) : MyNetworkElementBase(name) {
    _arrowHead = NULL;
    _isSetArrowHead = false;
    _isConnectedToNodes = false;
    _graphicsItem = createEdgeSceneGraphicsItem();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(createFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
    connect(_graphicsItem, SIGNAL(askForUpdateArrowHeadPlacement()), this, SLOT(updateArrowHeadPlacement()));
    connect(_graphicsItem, SIGNAL(askForUpdateConnectedEdgesToStartNode(const QPointF&)), this, SLOT(adjustConnectedEdgesToStartNode(const QPointF&)));
    connect(_graphicsItem, SIGNAL(askForUpdateConnectedEdgesToEndNode(const QPointF&)), this, SLOT(adjustConnectedEdgesToEndNode(const QPointF&)));
    setStartNode(startNode);
    setEndNode(endNode);
}

MyEdgeBase::~MyEdgeBase() {
    delete _graphicsItem;

    if (isSetArrowHead())
        delete _arrowHead;
}

MyEdgeBase::ELEMENT_TYPE MyEdgeBase::type() {
    return EDGE_ELEMENT;
};

void MyEdgeBase::setStartNode(MyNetworkElementBase* startNode) {
    _startNode = startNode;
    ((MyNodeBase*)_startNode)->addEdge(this);
    if (((MyNodeBase*)_startNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_startNode, SIGNAL(controlBezierLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustStartPointToControlBezierLine(const QLineF&)));
}

void MyEdgeBase::setEndNode(MyNetworkElementBase* endNode) {
    _endNode = endNode;
    ((MyNodeBase*)_endNode)->addEdge(this);
    if (((MyNodeBase*)_endNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_endNode, SIGNAL(controlBezierLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustEndPointToControlBezierLine(const QLineF&)));
}

MyNetworkElementBase* MyEdgeBase::startNode() {
    return _startNode;
}

MyNetworkElementBase* MyEdgeBase::endNode() {
    return _endNode;
}

void MyEdgeBase::updateGraphicsItem() {
    MyNetworkElementBase::updateGraphicsItem();
    if (isSetArrowHead())
        arrowHead()->updateGraphicsItem();
    updatePoints();
}

void MyEdgeBase::setStyle(MyNetworkElementStyleBase* style) {
    MyNetworkElementBase::setStyle(style);
    setArrowHead();
}

void MyEdgeBase::setSelected(const bool& selected) {
    MyNetworkElementBase::setSelected(selected);
    graphicsItem()->setSelectedWithStroke(selected);
    if (!selected)
        graphicsItem()->setSelectedWithFill(selected);

    if (isSetArrowHead())
        arrowHead()->setSelected(selected);
}

MyNetworkElementBase* MyEdgeBase::arrowHead() {
    return _arrowHead;
}

void MyEdgeBase::setArrowHead() {
    if (isSetArrowHead()) {
        delete _arrowHead;
        _isSetArrowHead = false;
    }

    if (((MyEdgeStyleBase*)style())->arrowHeadStyle() && ((MyEdgeStyleBase*)style())->arrowHeadStyle()->shapeStyles().size()) {
        _arrowHead = createArrowHead(name() + "_ArrowHead", ((MyEdgeStyleBase*)style())->arrowHeadStyle(), this);
        connect(_arrowHead, SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SIGNAL(askForDisplayFeatureMenu(QWidget*)));
        _isSetArrowHead = true;
    }
}

bool MyEdgeBase::setActive(const bool& active) {
    if (active && startNode() && startNode()->isActive() && endNode() && endNode()->isActive()) {
        updatePoints();
        return _isActive = true;
    }

    return _isActive = false;
}

bool MyEdgeBase::connectToNodes(const bool& connect) {
    return _isConnectedToNodes = connect;
}

void MyEdgeBase::updatePoints() {
    QPointF startPosition = getEndOfTheLinePosition(startNode(), endNode());
    QPointF endPosition = getEndOfTheLinePosition(endNode(), startNode());
    ((MyEdgeSceneGraphicsItem*)graphicsItem())->setLine(QLineF(startPosition.x(), startPosition.y(), endPosition.x(), endPosition.y()));
    graphicsItem()->setZValue(calculateZValue());
    emit updateArrowHeadPlacement();
}

void MyEdgeBase::adjustConnectedEdgesToStartNode(const QPointF& updatedStartPoint) {
    if (((MyNodeBase*)startNode())->nodeType() == MyNodeBase::CENTROID_NODE) {
        emit askForAdjustConnectedEdges(updatedStartPoint);
        emit askForDisconnectNodePositionFromNeighborNodes();
    }
}

void MyEdgeBase::adjustConnectedEdgesToEndNode(const QPointF& updatedEndPoint) {
    if (((MyNodeBase*)endNode())->nodeType() == MyNodeBase::CENTROID_NODE) {
        emit askForAdjustConnectedEdges(updatedEndPoint);
        emit askForDisconnectNodePositionFromNeighborNodes();
    }
}

void MyEdgeBase::updateArrowHeadPlacement() {
    if (isSetArrowHead())
        ((MyArrowHeadBase*)arrowHead())->updatePlacement(getEndOfTheLinePosition(endNode(), startNode()), ((MyEdgeSceneGraphicsItem*)graphicsItem())->getEndSlope());
}

void MyEdgeBase::enableNormalMode() {
    MyNetworkElementBase::enableNormalMode();
    if (isSetArrowHead())
        arrowHead()->enableNormalMode();
}

void MyEdgeBase::enableAddNodeMode() {
    MyNetworkElementBase::enableAddNodeMode();
    if (isSetArrowHead())
        arrowHead()->enableAddNodeMode();
}

void MyEdgeBase::enableSelectNodeMode() {
    MyNetworkElementBase::enableSelectNodeMode();
    if (isSetArrowHead())
        arrowHead()->enableSelectNodeMode();
}

void MyEdgeBase::enableAddEdgeMode() {
    MyNetworkElementBase::enableAddEdgeMode();
    if (isSetArrowHead())
        arrowHead()->enableAddEdgeMode();
}

void MyEdgeBase::enableSelectEdgeMode() {
    MyNetworkElementBase::enableSelectEdgeMode();
    if (isSetArrowHead())
        arrowHead()->enableSelectEdgeMode();
}

void MyEdgeBase::enableRemoveMode() {
    MyNetworkElementBase::enableRemoveMode();
    if (isSetArrowHead())
        arrowHead()->enableRemoveMode();
}

const QPointF MyEdgeBase::middlePosition() {
    return 0.5 * (((MyNodeBase*)startNode())->getExtents().center() + ((MyNodeBase*)endNode())->getExtents().center());
}

const QRectF MyEdgeBase::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyEdgeBase::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // source node
    contentLayout->addWidget(new MyLabel("Source"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(startNode()->name()), contentLayout->rowCount() - 1, 1);

    // target node
    contentLayout->addWidget(new MyLabel("Target"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(endNode()->name()), contentLayout->rowCount() - 1, 1);

    return featureMenu;
}

const qint32 MyEdgeBase::calculateZValue() {
    return qMax(startNode()->calculateZValue(), endNode()->calculateZValue()) - 2;
}

void MyEdgeBase::read(const QJsonObject &json) {

}

void MyEdgeBase::write(QJsonObject &json) {
    // id
    json["id"] = name();

    QJsonObject positionObject;
    // start node
    QJsonObject startObject;
    if (startNode()) {
        startObject["node"] = startNode()->name();

        QPointF startPosition = getEndOfTheLinePosition(startNode(), endNode());
        QJsonObject positionObject;
        positionObject["x"] = startPosition.x();
        positionObject["y"] = startPosition.y();
        startObject["position"] = positionObject;

        json["start"] = startObject;
    }

    // end node
    QJsonObject endObject;
    if (endNode()) {
        endObject["node"] = endNode()->name();

        QPointF endPosition = getEndOfTheLinePosition(endNode(), startNode());
        QJsonObject positionObject;
        positionObject["x"] = endPosition.x();
        positionObject["y"] = endPosition.y();
        endObject["position"] = positionObject;

        json["end"] = endObject;
    }

    // style
    QJsonObject styleObject;
    style()->write(styleObject);
    json["style"] = styleObject;
}

const QPointF getEndOfTheLinePosition(MyNetworkElementBase* mainNode, MyNetworkElementBase* connectedNode) {
    QRectF mainNodeExtents = ((MyNodeBase*)mainNode)->getExtents();
    QRectF connectedNodeExtents = ((MyNodeBase*)connectedNode)->getExtents();
    QPointF mainNodePosition = mainNodeExtents.center();
    QPointF connectedNodePosition = connectedNodeExtents.center();
    qreal mainNodeRadius = getRadius(mainNodeExtents, connectedNodePosition);
    qreal connectedNodeRadius = getRadius(connectedNodeExtents, mainNodePosition);
    if (qSqrt(((connectedNodePosition - mainNodePosition).x() * (connectedNodePosition - mainNodePosition).x()) + ((connectedNodePosition - mainNodePosition).y() * (connectedNodePosition - mainNodePosition).y())) > (mainNodeRadius + connectedNodeRadius + 10.0))
        mainNodeRadius += ((MyNodeBase*)mainNode)->endEdgePadding();
    qreal slope = qAtan2(connectedNodePosition.y() - mainNodePosition.y(), connectedNodePosition.x() - mainNodePosition.x());
    return QPointF(mainNodePosition.x() + mainNodeRadius * qCos(slope), mainNodePosition.y() + mainNodeRadius * qSin(slope));
}

qreal getRadius(const QRectF& rect, const QPointF& point) {
    qreal q1CuttingAngle = qRadiansToDegrees(qAtan2(rect.height(), rect.width()));
    qreal q2CuttingAngle = qRadiansToDegrees(qAtan2(rect.height(), -rect.width()));
    qreal q3CuttingAngle = qRadiansToDegrees(qAtan2(-rect.height(), -rect.width()));
    qreal q4CuttingAngle = qRadiansToDegrees(qAtan2(-rect.height(), rect.width()));
    qreal pointAngle = qRadiansToDegrees(qAtan2((point - rect.center()).y(), (point - rect.center()).x()));
    if (pointAngle > q4CuttingAngle && pointAngle <= q1CuttingAngle)
        return 0.5 * rect.width() / qFabs(qCos(qDegreesToRadians(pointAngle)));
    else if (pointAngle > q1CuttingAngle && pointAngle <= q2CuttingAngle)
        return 0.5 * rect.height() / qFabs(qSin(qDegreesToRadians(pointAngle)));
    else if ((pointAngle > q2CuttingAngle && pointAngle <= 180.0) || (pointAngle <= q3CuttingAngle && pointAngle >= -180.0))
        return 0.5 * rect.width() / qFabs(qCos(qDegreesToRadians(pointAngle)));
    else if (pointAngle > q3CuttingAngle && pointAngle <= q4CuttingAngle) {
        return 0.5 * rect.height() / qFabs(qSin(qDegreesToRadians(pointAngle)));
    }

    return qMax(rect.width(), rect.height());
}

// MyClassicEdge

MyClassicEdge::MyClassicEdge(const QString& name, MyNetworkElementBase* startNode, MyNetworkElementBase* endNode) : MyEdgeBase(name, startNode, endNode) {

}

MyClassicEdge::EDGE_TYPE MyClassicEdge::edgeType() {
    return CLASSIC_EDGE;
}
