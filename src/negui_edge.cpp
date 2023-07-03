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

MyEdgeBase::MyEdgeBase(const QString& name) : MyNetworkElementBase(name) {
    _arrowHead = NULL;
    _isSetArrowHead = false;
}

MyEdgeBase::~MyEdgeBase() {
    if (isSetArrowHead())
        delete _arrowHead;
}

MyEdgeBase::ELEMENT_TYPE MyEdgeBase::type() {
    return EDGE_ELEMENT;
};

void MyEdgeBase::connectGraphicsItem() {
    MyNetworkElementBase::connectGraphicsItem();
    connect(_graphicsItem, SIGNAL(askForUpdateArrowHeadPlacement()), this, SLOT(updateArrowHeadPlacement()));
    connect(_graphicsItem, SIGNAL(askForUpdateConnectedEdgesToSourceNode(const QPointF&)), this, SLOT(adjustConnectedEdgesToSourceNode(const QPointF&)));
    connect(_graphicsItem, SIGNAL(askForUpdateConnectedEdgesToTargetNode(const QPointF&)), this, SLOT(adjustConnectedEdgesToTargetNode(const QPointF&)));
    connect(_graphicsItem, SIGNAL(askForSetConnectedElementsFocused(const bool&)), this, SIGNAL(askForSetConnectedElementsSelected(const bool&)));
}

void MyEdgeBase::setSourceNode(MyNetworkElementBase* sourceNode) {
    _sourceNode = sourceNode;
    ((MyNodeBase*)_sourceNode)->addEdge(this);
    if (((MyNodeBase*)_sourceNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_sourceNode, SIGNAL(controlBezierLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustStartPointToControlBezierLine(const QLineF&)));
}

void MyEdgeBase::setTargetNode(MyNetworkElementBase* targetNode) {
    _targetNode = targetNode;
    ((MyNodeBase*)_targetNode)->addEdge(this);
    if (((MyNodeBase*)_targetNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_targetNode, SIGNAL(controlBezierLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustEndPointToControlBezierLine(const QLineF&)));
}

MyNetworkElementBase* MyEdgeBase::sourceNode() {
    return _sourceNode;
}

MyNetworkElementBase* MyEdgeBase::targetNode() {
    return _targetNode;
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

const bool MyEdgeBase::isCopyable() {
    if (isSelected() && sourceNode()->isCopyable() && targetNode()->isCopyable())
        return true;

    return false;
}

const bool MyEdgeBase::isCuttable() {
    if (isSelected() && sourceNode()->isCopyable() && targetNode()->isCopyable())
        return true;

    return false;
}

void MyEdgeBase::setSelected(const bool& selected) {
    MyNetworkElementBase::setSelected(selected);
    if (isSetArrowHead())
        arrowHead()->setSelected(selected);
}

void MyEdgeBase::setSelectedWithColor(const bool& selected) {
    graphicsItem()->setSelectedWithStroke(selected);
    if (!selected)
        graphicsItem()->setSelectedWithFill(selected);

    if (isSetArrowHead())
        arrowHead()->setSelectedWithColor(selected);
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
    if (active && sourceNode() && sourceNode()->isActive() && targetNode() && targetNode()->isActive()) {
        updatePoints();
        return _isActive = true;
    }

    return _isActive = false;
}

void MyEdgeBase::updatePoints() {
    QPointF sourcePosition = getEndOfTheLinePosition(sourceNode(), targetNode());
    QPointF targetPosition = getEndOfTheLinePosition(targetNode(), sourceNode());
    ((MyEdgeSceneGraphicsItem*)graphicsItem())->setLine(QLineF(sourcePosition.x(), sourcePosition.y(), targetPosition.x(), targetPosition.y()));
    graphicsItem()->setZValue(calculateZValue());
    emit updateArrowHeadPlacement();
}

void MyEdgeBase::adjustConnectedEdgesToSourceNode(const QPointF& updatedSourcePoint) {
    if (((MyNodeBase*)sourceNode())->nodeType() == MyNodeBase::CENTROID_NODE) {
        emit askForAdjustConnectedEdges(updatedSourcePoint);
        emit askForDisconnectNodePositionFromNeighborNodes();
    }
}

void MyEdgeBase::adjustConnectedEdgesToTargetNode(const QPointF& updatedTargetPoint) {
    if (((MyNodeBase*)targetNode())->nodeType() == MyNodeBase::CENTROID_NODE) {
        emit askForAdjustConnectedEdges(updatedTargetPoint);
        emit askForDisconnectNodePositionFromNeighborNodes();
    }
}

void MyEdgeBase::updateArrowHeadPlacement() {
    if (isSetArrowHead())
        ((MyArrowHeadBase*)arrowHead())->updatePlacement(getEndOfTheLinePosition(targetNode(), sourceNode()), ((MyEdgeSceneGraphicsItem*)graphicsItem())->getEndSlope());
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

const QPointF MyEdgeBase::middlePosition() {
    return 0.5 * (((MyNodeBase*)sourceNode())->getExtents().center() + ((MyNodeBase*)targetNode())->getExtents().center());
}

const QRectF MyEdgeBase::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyEdgeBase::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // source node
    contentLayout->addWidget(new MyLabel("Source"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(sourceNode()->name()), contentLayout->rowCount() - 1, 1);

    // target node
    contentLayout->addWidget(new MyLabel("Target"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(targetNode()->name()), contentLayout->rowCount() - 1, 1);

    return featureMenu;
}

const qint32 MyEdgeBase::calculateZValue() {
    return qMax(sourceNode()->calculateZValue(), targetNode()->calculateZValue()) - 2;
}

void MyEdgeBase::read(const QJsonObject &json) {

}

void MyEdgeBase::write(QJsonObject &json) {
    // id
    json["id"] = name();

    QJsonObject positionObject;
    // source node
    QJsonObject sourceObject;
    if (sourceNode()) {
        sourceObject["node"] = sourceNode()->name();

        QPointF sourcePosition = getEndOfTheLinePosition(sourceNode(), targetNode());
        QJsonObject positionObject;
        positionObject["x"] = sourcePosition.x();
        positionObject["y"] = sourcePosition.y();
        sourceObject["position"] = positionObject;

        json["source"] = sourceObject;
    }

    // target node
    QJsonObject targetObject;
    if (targetNode()) {
        targetObject["node"] = targetNode()->name();

        QPointF targetPosition = getEndOfTheLinePosition(targetNode(), sourceNode());
        QJsonObject positionObject;
        positionObject["x"] = targetPosition.x();
        positionObject["y"] = targetPosition.y();
        targetObject["position"] = positionObject;

        json["target"] = targetObject;
    }

    // style
    QJsonObject styleObject;
    style()->write(styleObject);
    json["style"] = styleObject;
}

// MyClassicEdge

MyClassicEdge::MyClassicEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) : MyEdgeBase(name) {
    _graphicsItem = createEdgeSceneGraphicsItem();
    connectGraphicsItem();
    setSourceNode(sourceNode);
    setTargetNode(targetNode);
}

MyEdgeBase::EDGE_TYPE MyClassicEdge::edgeType() {
    return CLASSIC_EDGE;
}

// MyConnectedToCentroidNodeEdge

MyConnectedToCentroidNodeEdge::MyConnectedToCentroidNodeEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) : MyEdgeBase(name) {
    _graphicsItem = createEdgeSceneGraphicsItem();
    connectGraphicsItem();
    setSourceNode(sourceNode);
    setTargetNode(targetNode);
}

MyEdgeBase::EDGE_TYPE MyConnectedToCentroidNodeEdge::edgeType() {
    return CONNECTED_TO_CENTROID_NODE_EDGE;
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
