#include "negui_node_graphics_item.h"
#include "negui_context_menu.h"

#include <QtMath>

// MyNodeGraphicsItemBase

MyNodeGraphicsItemBase::MyNodeGraphicsItemBase(QGraphicsItem *parent) : MyNetworkElementGraphicsItemBase(parent) {
    enableNormalMode();
}

QMenu* MyNodeGraphicsItemBase::createContextMenu() {
    QMenu* contextMenu = createContextMenuObject();
    connectContextMenu(contextMenu);
    ((MyContextMenuBase*)contextMenu)->initializeActionsStatus();
    return contextMenu;
}

QMenu* MyNodeGraphicsItemBase::createContextMenuObject() {
    return new MyNodeGraphicsItemContextMenuBase();
}

// MyNodeSceneGraphicsItemBase

MyNodeSceneGraphicsItemBase::MyNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = position;

    setFlag(QGraphicsItem::ItemIsFocusable, true);

    connect(this, SIGNAL(positionChangedByMouseMoveEvent(const QPointF&)), this, SLOT(updateFocusedGraphicsItems()));
    
    setZValue(2);
}

void MyNodeSceneGraphicsItemBase::update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    if (_shapeStyles.size()) {
        _originalPosition = getExtents().center();
        setPos(0, 0);
    }
    MyNetworkElementGraphicsItemBase::update(shapeStyles, zValue);
}

void MyNodeSceneGraphicsItemBase::updateOriginalPosition() {
    QPointF originalPosition = getExtents().center();
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->updateOriginalPosition(originalPosition);
    }
}

void MyNodeSceneGraphicsItemBase::moveChildItems(const qreal& dx, const qreal& dy) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->moveOriginalPosition(dx,dy);
    }
}

void MyNodeSceneGraphicsItemBase::enableNormalMode() {
    MyNetworkElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::PointingHandCursor);
}

void MyNodeSceneGraphicsItemBase::enableAddNodeMode() {
    MyNetworkElementGraphicsItemBase::enableAddNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyNodeSceneGraphicsItemBase::enableSelectNodeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectNodeMode();
    setCursor(Qt::PointingHandCursor);
}

void MyNodeSceneGraphicsItemBase::enableAddEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableAddEdgeMode();
    setCursor(Qt::PointingHandCursor);
}

void MyNodeSceneGraphicsItemBase::enableSelectEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectEdgeMode();
    setCursor(Qt::ArrowCursor);
}

void MyNodeSceneGraphicsItemBase::move(qreal dx, qreal dy) {
    moveBy(dx, dy);
    moveChildItems(dx, dy);
}

void MyNodeSceneGraphicsItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    MyNetworkElementGraphicsItemBase::mouseMoveEvent(event);
    emit positionChangedByMouseMoveEvent(event->scenePos() - event->lastScenePos());
}

void MyNodeSceneGraphicsItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit askForUpdateParentNode();
    MyNetworkElementGraphicsItemBase::mouseReleaseEvent(event);
}

// MyClassicNodeSceneGraphicsItemBase

MyClassicNodeSceneGraphicsItemBase::MyClassicNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent) : MyNodeSceneGraphicsItemBase(position, parent) {

}

const bool MyClassicNodeSceneGraphicsItemBase::canAddEllipseShape() {
    return true;
}

const bool MyClassicNodeSceneGraphicsItemBase::canAddRectangleShape() {
    return true;
}

const bool MyClassicNodeSceneGraphicsItemBase::canAddPolygonShape() {
    return true;
}

const bool MyClassicNodeSceneGraphicsItemBase::canAddTextShape() {
    return true;
}

void MyClassicNodeSceneGraphicsItemBase::setFocused(const bool& isFocused) {
    if (!isFocused && _focusedGraphicsItems.size()) {
        emit askForUpdateConnectedEdgesPoints();
        emit askForCreateChangeStageCommand();
    }
    MyNetworkElementGraphicsItemBase::setFocused(isFocused);
}

void MyClassicNodeSceneGraphicsItemBase::updateExtents(const QRectF& extents) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->updateExtents(QRectF(extents.x(), extents.y(), extents.width(), extents.height()));
    }
}

// MySimpleClassicNodeSceneGraphicsItem

MySimpleClassicNodeSceneGraphicsItem::MySimpleClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyClassicNodeSceneGraphicsItemBase(position, parent) {

}

QList<QGraphicsItem*> MySimpleClassicNodeSceneGraphicsItem::createFocusedGraphicsItems() {
    if (whetherShapesAreOneSimpleTextShapeAndAnotherShape()) {
        QList<QGraphicsItem*> focusedGraphicsItems;
        QGraphicsItem* focusedGraphicsItem = NULL;
        QGraphicsItem* shapeFocusedGraphicsItem = NULL;
        MyShapeGraphicsItemBase* textGraphicsItem = NULL;
        for (QGraphicsItem* item : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
            if (casted_item) {
                focusedGraphicsItem = casted_item->getFocusedGraphicsItem();
                if (focusedGraphicsItem) {
                    focusedGraphicsItems.push_back(focusedGraphicsItem);
                    shapeFocusedGraphicsItem = focusedGraphicsItem;
                }
                else if (casted_item->style()->type() == MyShapeStyleBase::SIMPLE_TEXT_SHAPE_STYLE)
                    textGraphicsItem = casted_item;
            }
        }

        connect(dynamic_cast<QObject*>(shapeFocusedGraphicsItem), SIGNAL(rectIsUpdated(const QRectF&)), textGraphicsItem, SLOT(updateExtents(const QRectF&)));
        return focusedGraphicsItems;
    }

    return MyNetworkElementGraphicsItemBase::createFocusedGraphicsItems();
}

const bool MySimpleClassicNodeSceneGraphicsItem::whetherShapesAreOneSimpleTextShapeAndAnotherShape() {
    bool oneSimpleTextShapeExists = false;
    bool oneOtherShapeExists = false;
    if (childItems().size() == 2) {
        for (QGraphicsItem* item : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
            if (casted_item->style()->type() == MyShapeStyleBase::SIMPLE_TEXT_SHAPE_STYLE)
                oneSimpleTextShapeExists = true;
            else
                oneOtherShapeExists = true;
        }
    }

    return oneSimpleTextShapeExists && oneOtherShapeExists;
}

const QRectF MySimpleClassicNodeSceneGraphicsItem::getExtents() const {
    if (childItems().size()) {
        QRectF childExtents = dynamic_cast<MyShapeGraphicsItemBase*>(childItems().at(0))->getExtents();
        qreal extentsX = childExtents.x();
        qreal extentsY = childExtents.y();
        qreal extentsWidth = childExtents.width();
        qreal extentsHeight = childExtents.height();
        for (QGraphicsItem* childItem : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(childItem);
            if (casted_item->style()->type() != MyShapeStyleBase::SIMPLE_TEXT_SHAPE_STYLE) {
                childExtents = casted_item->getExtents();
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
                    extentsHeight += childExtents.y() + childExtents.height() - extentsY - extentsHeight;
            }
        }
        return QRectF(extentsX, extentsY, extentsWidth, extentsHeight);
    }

    return QRectF(_originalPosition.x() + x(), _originalPosition.y() + y(), 0.0, 0.0);
}

// MyComplexClassicNodeSceneGraphicsItem

MyComplexClassicNodeSceneGraphicsItem::MyComplexClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyClassicNodeSceneGraphicsItemBase(position, parent) {

}

// MyCentroidNodeSceneGraphicsItem

MyCentroidNodeSceneGraphicsItem::MyCentroidNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyNodeSceneGraphicsItemBase(position, parent) {
    setAcceptHoverEvents(true);
}

void MyCentroidNodeSceneGraphicsItem::connectShapeGraphicsItem(MyShapeGraphicsItemBase *item) {
    connect(item, SIGNAL(askForGetBezierAdjustLine()), this, SIGNAL(askForGetBezierAdjustLine()));
    connect(item, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), this, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)));
}

const bool MyCentroidNodeSceneGraphicsItem::canAddCentroidShape() {
    return true;
}

QMenu* MyCentroidNodeSceneGraphicsItem::createContextMenuObject() {
    return new MyCentroidNodeGraphicsItemContextMenu();
}

void MyCentroidNodeSceneGraphicsItem::connectContextMenu(QMenu* contextMenu) {
    MyNetworkElementGraphicsItemBase::connectContextMenu(contextMenu);
    connect(contextMenu, SIGNAL(askForConnectNodePositionToNeighborNodes(const bool&)), this, SIGNAL(askForConnectNodePositionToNeighborNodes(const bool&)));
    connect(contextMenu, SIGNAL(askForWhetherNodePositionIsConnectedToNeighborNodes()), this, SIGNAL(askForWhetherNodePositionIsConnectedToNeighborNodes()));
}

// MyNodeIconGraphicsItem

MyNodeIconGraphicsItem::MyNodeIconGraphicsItem(const QPointF& position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = position;
}

const bool MyNodeIconGraphicsItem::canAddEllipseShape() {
    return true;
}

const bool MyNodeIconGraphicsItem::canAddRectangleShape() {
    return true;
}

const bool MyNodeIconGraphicsItem::canAddPolygonShape() {
    return true;
}

const bool MyNodeIconGraphicsItem::canAddTextShape() {
    return false;
}

void MyNodeIconGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    for (QGraphicsItem* item : childItems())
        item->paint(painter, option, widget);
}
