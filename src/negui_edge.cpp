#include "negui_edge.h"
#include "negui_node.h"
#include "negui_edge_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_edge_style.h"
#include "negui_edge_graphics_item.h"
#include "negui_arrow_head_graphics_item.h"
#include "negui_customized_common_widgets.h"

#include <QtMath>
#include <QJsonObject>
#include <QGridLayout>

// MyEdgeBase

MyEdgeBase::MyEdgeBase(const QString& name) : MyNetworkElementBase(name) {
    _arrowHeadGraphicsItem = createArrowHeadSceneGraphicsItem();
}

MyEdgeBase::ELEMENT_TYPE MyEdgeBase::type() {
    return EDGE_ELEMENT;
};

void MyEdgeBase::connectGraphicsItem() {
    MyNetworkElementBase::connectGraphicsItem();
    connect(_graphicsItem, SIGNAL(askForUpdateArrowHeadPlacement()), this, SLOT(updateArrowHeadPlacement()));
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForSelectNetworkElement, this, [this] () { emit askForSelectNetworkElement(this); });
    connect(_arrowHeadGraphicsItem, &MyNetworkElementGraphicsItemBase::askForSelectNetworkElement, this, [this] () { emit askForSelectNetworkElement(this); });;
    connect(_arrowHeadGraphicsItem, SIGNAL(askForEnableFeatureMenuDisplay()), this, SIGNAL(askForEnableFeatureMenuDisplay()));
    connect(_arrowHeadGraphicsItem, SIGNAL(mouseRightButtonIsReleased(const QPointF&)), _graphicsItem, SLOT(displayContextMenu(const QPointF&)));
}

void MyEdgeBase::setSourceNode(MyNetworkElementBase* sourceNode) {
    _sourceNode = sourceNode;
    ((MyNodeBase*)_sourceNode)->addEdge(this);
    if (((MyNodeBase*)_sourceNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_sourceNode, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustStartPointToControlBezierLine(const QLineF&)));
}

void MyEdgeBase::setTargetNode(MyNetworkElementBase* targetNode) {
    _targetNode = targetNode;
    ((MyNodeBase*)_targetNode)->addEdge(this);
    if (((MyNodeBase*)_targetNode)->nodeType() == MyNodeBase::CENTROID_NODE)
        connect(_targetNode, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), graphicsItem(), SIGNAL(askForAdjustEndPointToControlBezierLine(const QLineF&)));
}

MyNetworkElementBase* MyEdgeBase::sourceNode() {
    return _sourceNode;
}

MyNetworkElementBase* MyEdgeBase::targetNode() {
    return _targetNode;
}

MyNetworkElementGraphicsItemBase* MyEdgeBase::arrowHeadGraphicsItem() {
    return _arrowHeadGraphicsItem;
}

void MyEdgeBase::updateGraphicsItem() {
    MyNetworkElementBase::updateGraphicsItem();
    updateArrowHeadGraphicsItem();
    updatePoints();
}

void MyEdgeBase::updateArrowHeadGraphicsItem() {
    if (arrowHeadGraphicsItem())
        ((MyArrowHeadGraphicsItemBase*)arrowHeadGraphicsItem())->update(((MyEdgeStyleBase*)style())->arrowHeadStyle()->shapeStyles(), calculateArrowHeadZValue());
}

void MyEdgeBase::setStyle(MyNetworkElementStyleBase* style) {
    MyNetworkElementBase::setStyle(style);
    updateArrowHeadGraphicsItem();
}

void MyEdgeBase::setSelected(const bool& selected) {
    if (selected != isSelected()) {
        MyNetworkElementBase::setSelected(selected);
        if (arrowHeadGraphicsItem())
            arrowHeadGraphicsItem()->setFocused(selected);
        emit askForSetConnectedElementsSelected(selected);
    }
}

void MyEdgeBase::setSelectedWithColor(const bool& selected) {
    graphicsItem()->setSelectedWithBorderColor(selected);
    if (!selected)
        graphicsItem()->setSelectedWithFillColor(selected);
    if (arrowHeadGraphicsItem())
        arrowHeadGraphicsItem()->setSelectedWithFillColor(selected);
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
    ((MyEdgeSceneGraphicsItemBase*)graphicsItem())->setLine(QLineF(sourcePosition.x(), sourcePosition.y(), targetPosition.x(), targetPosition.y()));
    graphicsItem()->setZValue(calculateZValue());
    updateFocusedGraphicsItems();
    emit updateArrowHeadPlacement();
}

void MyEdgeBase::updateArrowHeadPlacement() {
    if (arrowHeadGraphicsItem()) {
        ((MyArrowHeadSceneGraphicsItem *) arrowHeadGraphicsItem())->update(
                getEndOfTheLinePosition(targetNode(), sourceNode()),
                ((MyEdgeSceneGraphicsItemBase *) graphicsItem())->getEndSlope());
        graphicsItem()->setZValue(calculateArrowHeadZValue());
    }
}

const bool MyEdgeBase::canBeMoved() {
    return false;
}

void MyEdgeBase::move(const qreal& dx, const qreal& dy) {
    if (canBeMoved())
        return;
}

const QRectF MyEdgeBase::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyEdgeBase::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // source node
    QString sourceLabel = "Source";
    if (!((MyEdgeStyleBase*)style())->connectableSourceNodeTitle().isEmpty())
        sourceLabel = ((MyEdgeStyleBase*)style())->connectableSourceNodeTitle();
    contentLayout->addWidget(new MyLabel(sourceLabel), contentLayout->rowCount(), 0, Qt::AlignLeft);
    contentLayout->addWidget(new MyReadOnlyLineEdit(sourceNode()->name()), contentLayout->rowCount() - 1, 1, Qt::AlignRight);

    // target node
    QString targetLabel = "Target";
    if (!((MyEdgeStyleBase*)style())->connectableTargetNodeTitle().isEmpty())
        targetLabel = ((MyEdgeStyleBase*)style())->connectableTargetNodeTitle();
    contentLayout->addWidget(new MyLabel(targetLabel), contentLayout->rowCount(), 0, Qt::AlignLeft);
    contentLayout->addWidget(new MyReadOnlyLineEdit(targetNode()->name()), contentLayout->rowCount() - 1, 1, Qt::AlignRight);

    return featureMenu;
}

const qint32 MyEdgeBase::calculateZValue() {
    return qMax(((MyNodeBase*)sourceNode())->calculateConnectedEdgeZValue(), ((MyNodeBase*)targetNode())->calculateConnectedEdgeZValue());
}

const qint32 MyEdgeBase::calculateArrowHeadZValue() {
    return calculateZValue() + 1;
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
    _graphicsItem = createClassicEdgeSceneGraphicsItem();
    connectGraphicsItem();
    setSourceNode(sourceNode);
    setTargetNode(targetNode);
}

MyEdgeBase::EDGE_TYPE MyClassicEdge::edgeType() {
    return CLASSIC_EDGE;
}

const bool MyClassicEdge::isCopyable() {
    if (isSelected() && sourceNode()->isCopyable() && targetNode()->isCopyable())
        return true;

    return false;
}

const bool MyClassicEdge::isCuttable() {
    if (isSelected() && sourceNode()->isCopyable() && targetNode()->isCopyable())
        return true;

    return false;
}

// MyConnectedToCentroidNodeEdgeBase

MyConnectedToCentroidNodeEdgeBase::MyConnectedToCentroidNodeEdgeBase(const QString& name) : MyEdgeBase(name) {

}

void MyConnectedToCentroidNodeEdgeBase::connectGraphicsItem() {
    MyEdgeBase::connectGraphicsItem();
    connect(this, SIGNAL(askForConnectedToCentroidNodeControlPoint()), _graphicsItem, SIGNAL(askForConnectedToCentroidNodeControlPoint()));
}

// MyConnectedToSourceCentroidNodeEdge

MyConnectedToSourceCentroidNodeEdge::MyConnectedToSourceCentroidNodeEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) : MyConnectedToCentroidNodeEdgeBase(name) {
    _graphicsItem = createConnectedToSourceCentroidNodeEdgeSceneGraphicsItem();
    connectGraphicsItem();
    setSourceNode(sourceNode);
    setTargetNode(targetNode);
}

MyEdgeBase::EDGE_TYPE MyConnectedToSourceCentroidNodeEdge::edgeType() {
    return CONNECTED_TO_SOURCE_CENTROID_NODE_EDGE;
}

const bool MyConnectedToSourceCentroidNodeEdge::isCopyable() {
    if (isSelected() && targetNode()->isCopyable())
        return true;

    return false;
}

const bool MyConnectedToSourceCentroidNodeEdge::isCuttable() {
    if (isSelected() && targetNode()->isCopyable())
        return true;

    return false;
}


const QPointF MyConnectedToSourceCentroidNodeEdge::nonCentroidNodePosition() {
    return ((MyNodeBase*)targetNode())->getPosition();
}

MyNetworkElementBase* MyConnectedToSourceCentroidNodeEdge::nonCentroidNodeParent() {
    return ((MyNodeBase*)targetNode())->parentNode();
}

// MyConnectedToTargetCentroidNodeEdge

MyConnectedToTargetCentroidNodeEdge::MyConnectedToTargetCentroidNodeEdge(const QString& name, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode) : MyConnectedToCentroidNodeEdgeBase(name) {
    _graphicsItem = createConnectedToTargetCentroidNodeEdgeSceneGraphicsItem();
    connectGraphicsItem();
    setSourceNode(sourceNode);
    setTargetNode(targetNode);
}

MyEdgeBase::EDGE_TYPE MyConnectedToTargetCentroidNodeEdge::edgeType() {
    return CONNECTED_TO_TARGET_CENTROID_NODE_EDGE;
}

const bool MyConnectedToTargetCentroidNodeEdge::isCopyable() {
    if (isSelected() && sourceNode()->isCopyable())
        return true;

    return false;
}

const bool MyConnectedToTargetCentroidNodeEdge::isCuttable() {
    if (isSelected() && sourceNode()->isCopyable())
        return true;

    return false;
}

const QPointF MyConnectedToTargetCentroidNodeEdge::nonCentroidNodePosition() {
    return ((MyNodeBase*)sourceNode())->getPosition();
}

MyNetworkElementBase* MyConnectedToTargetCentroidNodeEdge::nonCentroidNodeParent() {
    return ((MyNodeBase*)sourceNode())->parentNode();
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
