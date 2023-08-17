#include "negui_edge_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include "negui_1d_shape_graphics_item_base.h"
#include "negui_context_menu.h"

// MyEdgeGraphicsItemBase

MyEdgeGraphicsItemBase::MyEdgeGraphicsItemBase(QGraphicsItem *parent) : MyNetworkElementGraphicsItemBase(parent) {
    enableNormalMode();
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::LINE_SHAPE_STYLE) {
        item = createLineShapeGraphicsItem();
        connectShapeGraphicsItem(item);
        item->setZValue(zValue());
    }

    return item;
}

void MyEdgeGraphicsItemBase::connectShapeGraphicsItem(MyShapeGraphicsItemBase* item) {
    connect(item, SIGNAL(lineControlPoint2IsUpdated(const QPointF&)), SIGNAL(askForUpdateArrowHeadPlacement()));
    connect(this, SIGNAL(askForAdjustStartPointToControlBezierLine(const QLineF&)), item, SLOT(adjustLineControlPoint1ToControlBezierLine(const QLineF&)));
    connect(this, SIGNAL(askForAdjustEndPointToControlBezierLine(const QLineF&)), item, SLOT(adjustLineControlPoint2ToControlBezierLine(const QLineF&)));
}

QMenu* MyEdgeGraphicsItemBase::createContextMenu() {
    QMenu* contextMenu = new MyEdgeGraphicsItemContextMenu();
    connectContextMenu(contextMenu);
    ((MyContextMenuBase*)contextMenu)->initializeActionsStatus();
    return contextMenu;
}

void MyEdgeGraphicsItemBase::setLine(const QLineF &line) {
    for (QGraphicsItem* item : childItems()) {
        My1DShapeGraphicsItemBase* casted_item = dynamic_cast<My1DShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setLine(line);
    }
}

const qreal MyEdgeGraphicsItemBase::getEndSlope() const {
    qreal endSlope = 0.0;
    for (QGraphicsItem* item : childItems()) {
        My1DShapeGraphicsItemBase* casted_item = dynamic_cast<My1DShapeGraphicsItemBase*>(item);
        if (casted_item)
            endSlope += casted_item->getEndSlope();
    }
    
    if (childItems().size())
        return (endSlope / childItems().size());
    
    return endSlope;
}

// MyEdgeSceneGraphicsItemBase

MyEdgeSceneGraphicsItemBase::MyEdgeSceneGraphicsItemBase(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 0.0, 0.0);
    setZValue(0);
}

void MyEdgeSceneGraphicsItemBase::enableNormalMode() {
    MyNetworkElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItemBase::enableAddNodeMode() {
    MyNetworkElementGraphicsItemBase::enableAddNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItemBase::enableSelectNodeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItemBase::enableAddEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableAddEdgeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItemBase::enableSelectEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectEdgeMode();
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeSceneGraphicsItemBase::enableDisplayFeatureMenuMode() {
    MyNetworkElementGraphicsItemBase::enableDisplayFeatureMenuMode();
    setCursor(Qt::ArrowCursor);
}

// MyClassicEdgeSceneGraphicsItem

MyClassicEdgeSceneGraphicsItem::MyClassicEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyEdgeSceneGraphicsItemBase(parent) {

}

MyShapeGraphicsItemBase* MyClassicEdgeSceneGraphicsItem::createLineShapeGraphicsItem() {
    return createClassicLineShape(_initialLine, this);
}

// MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase

MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase::MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase(QGraphicsItem *parent) : MyEdgeSceneGraphicsItemBase(parent) {

}

void MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase::connectShapeGraphicsItem(MyShapeGraphicsItemBase* item) {
    MyEdgeGraphicsItemBase::connectShapeGraphicsItem(item);
    connect(this, SIGNAL(askForConnectedToCentroidNodeControlPoint()), item, SLOT(connectedToCentroidNodeControlPoint()));
}
// MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem

MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem::MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase(parent) {

}

MyShapeGraphicsItemBase* MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem::createLineShapeGraphicsItem() {
    return createConnectedToStartCentroidShapeLineShape(_initialLine, this);
}

// MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem

MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem::MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyConnectedToCentroidNodeEdgeSceneGraphicsItemBase(parent) {

}

MyShapeGraphicsItemBase* MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem::createLineShapeGraphicsItem() {
    return createConnectedToEndCentroidShapeLineShape(_initialLine, this);
}

// MyEdgeIconGraphicsItem

MyEdgeIconGraphicsItem::MyEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(startPoint, endPoint);
}

MyShapeGraphicsItemBase* MyEdgeIconGraphicsItem::createLineShapeGraphicsItem() {
    return createClassicLineShape(_initialLine, this);
}

void MyEdgeIconGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    for (QGraphicsItem* item : childItems())
        item->paint(painter, option, widget);
}
