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
        item = createLineShape(_initialLine, this);
        connect(item, SIGNAL(lineControlPoint2IsUpdated(const QPointF&)), SIGNAL(askForUpdateArrowHeadPlacement()));
        connect(item, SIGNAL(lineControlPoint1IsUpdated(const QPointF&)), this, SIGNAL(askForUpdateConnectedEdgesToStartNode(const QPointF&)));
        connect(item, SIGNAL(lineControlPoint2IsUpdated(const QPointF&)), this, SIGNAL(askForUpdateConnectedEdgesToEndNode(const QPointF&)));
        connect(this, SIGNAL(askForAdjustStartPointToControlBezierLine(const QLineF&)), item, SLOT(adjustLineControlPoint1ToControlBezierLine(const QLineF&)));
        connect(this, SIGNAL(askForAdjustEndPointToControlBezierLine(const QLineF&)), item, SLOT(adjustLineControlPoint2ToControlBezierLine(const QLineF&)));
        item->setZValue(zValue());
    }
        
    return item;
}

QMenu* MyEdgeGraphicsItemBase::createContextMenu() {
    QMenu* contextMenu = new MyEdgeGraphicsItemContextMenu();
    connect(contextMenu, SIGNAL(askForCreateFeatureMenu()), this, SIGNAL(askForCreateFeatureMenu()));
    connect(contextMenu, SIGNAL(askForCopyNetworkElementStyle()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(contextMenu, SIGNAL(askForWhetherCopiedElementStyleIsSet()), this, SIGNAL(askForWhetherCopiedElementStyleIsSet()));
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

// MyEdgeSceneGraphicsItem

MyEdgeSceneGraphicsItem::MyEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 0.0, 0.0);
    setZValue(0);
}

void MyEdgeSceneGraphicsItem::enableNormalMode() {
    MyNetworkElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableAddNodeMode() {
    MyNetworkElementGraphicsItemBase::enableAddNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableSelectNodeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableAddEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableAddEdgeMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableSelectEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectEdgeMode();
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeSceneGraphicsItem::enableRemoveMode() {
    MyNetworkElementGraphicsItemBase::enableRemoveMode();
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeSceneGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MyNetworkElementGraphicsItemBase::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        _mousePressedPosition = event->scenePos();
}

void MyEdgeSceneGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    MyNetworkElementGraphicsItemBase::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (qAbs(_mousePressedPosition.x() - event->scenePos().x()) < 0.01 && qAbs(_mousePressedPosition.y() - event->scenePos().y()) < 0.01) {
            setFocused(true);
        }
    }
}

void MyEdgeSceneGraphicsItem::focusOutEvent(QFocusEvent *event) {
    setFocused(false);
    QGraphicsItem::focusOutEvent(event);
}

// MyEdgeIconGraphicsItem

MyEdgeIconGraphicsItem::MyEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(startPoint, endPoint);
}

void MyEdgeIconGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    for (QGraphicsItem* item : childItems())
        item->paint(painter, option, widget);
}
