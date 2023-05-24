#include "negui_node_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include <QtMath>

// MyNodeGraphicsItemBase

MyNodeGraphicsItemBase::MyNodeGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    enableNormalMode();
}

MyShapeGraphicsItemBase* MyNodeGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::ELLIPSE_SHAPE_STYLE) {
        item = createEllipseShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::RECT_SHAPE_STYLE) {
        item = createRectShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE) {
        item = createPolygonShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::TEXT_SHAPE_STYLE) {
        item = createTextShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue() + 1);
    }
    else if (style->type() == MyShapeStyleBase::CENTROID_SHAPE_STYLE) {
        item = createCentroidShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }

    return item;
}

// MyNodeSceneGraphicsItemBase

MyNodeSceneGraphicsItemBase::MyNodeSceneGraphicsItemBase(const QPointF &position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = position;

    // make it movable
    setFlag(QGraphicsItem::ItemIsMovable, true);
    
    // make it send position changes
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    
    // make it focusable
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    
    _reparent = false;
    
    setZValue(2);
}

void MyNodeSceneGraphicsItemBase::deparent() {
    if (_reparent)
        emit askForDeparent();
}

void MyNodeSceneGraphicsItemBase::moveChildItems(const QPointF& movedDistance) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setMovedDistance(movedDistance);
    }
}

void MyNodeSceneGraphicsItemBase::enableNormalMode() {
    MyElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void MyNodeSceneGraphicsItemBase::enableAddNodeMode() {
    MyElementGraphicsItemBase::enableAddNodeMode();
    setCursor(Qt::ArrowCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeSceneGraphicsItemBase::enableSelectNodeMode() {
    MyElementGraphicsItemBase::enableSelectNodeMode();
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeSceneGraphicsItemBase::enableAddEdgeMode() {
    MyElementGraphicsItemBase::enableAddEdgeMode();
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeSceneGraphicsItemBase::enableSelectEdgeMode() {
    MyElementGraphicsItemBase::enableSelectEdgeMode();
    setCursor(Qt::ArrowCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeSceneGraphicsItemBase::enableRemoveMode() {
    MyElementGraphicsItemBase::enableRemoveMode();
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

QVariant MyNodeSceneGraphicsItemBase::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        deparent();
        moveChildItems(value.toPointF());
        emit askForResetPosition();
    }

    return QGraphicsItem::itemChange(change, value);
}

void MyNodeSceneGraphicsItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mouseMoveEvent(event);
    emit positionChangedByMouseMoveEvent();
}

void MyNodeSceneGraphicsItemBase::keyPressEvent(QKeyEvent *event) {
    QGraphicsItem::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            if (_isChosen)
                _reparent = true;
            event->accept();
        }
    }
}

void MyNodeSceneGraphicsItemBase::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsItem::keyReleaseEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            emit askForReparent();
            _reparent = false;
            event->accept();
        }
    }
}

// MyClassicNodeSceneGraphicsItem

MyClassicNodeSceneGraphicsItem::MyClassicNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyNodeSceneGraphicsItemBase(position, parent) {

}

void MyClassicNodeSceneGraphicsItem::clearFocusedGraphicsItems() {
    MyElementGraphicsItemBase::clearFocusedGraphicsItems();
    emit askForResetPosition();
    adjustOriginalPosition();
    emit askForCreateChangeStageCommand();
}

void MyClassicNodeSceneGraphicsItem::moveBy(qreal dx, qreal dy) {
    if (qFabs(dx) > 0.0001 || qFabs(dy) > 0.0001)
        QGraphicsItem::moveBy(dx, dy);
    else
        emit askForResetPosition();
}

void MyClassicNodeSceneGraphicsItem::adjustOriginalPosition() {
    QPointF extentsCenter = getExtents().center();
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->adjustOriginalPosition(extentsCenter - (_originalPosition + pos()));
    }
    _originalPosition = extentsCenter - pos();
}

void MyClassicNodeSceneGraphicsItem::updateExtents(const QRectF& extents) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->updateExtents(QRectF(extents.x(), extents.y(), extents.width(), extents.height()));
    }
}

void MyClassicNodeSceneGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        _mousePressedPosition = event->scenePos();
}

void MyClassicNodeSceneGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (qAbs(_mousePressedPosition.x() - event->scenePos().x()) < 0.01 && qAbs(_mousePressedPosition.y() - event->scenePos().y()) < 0.01) {
            setFocused(true);
            setFlag(QGraphicsItem::ItemIsMovable, false);
        }
    }
}

void MyClassicNodeSceneGraphicsItem::focusOutEvent(QFocusEvent *event) {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFocused(false);
    QGraphicsItem::focusOutEvent(event);
}

// MyCentroidNodeSceneGraphicsItem

MyCentroidNodeSceneGraphicsItem::MyCentroidNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyNodeSceneGraphicsItemBase(position, parent) {
    setAcceptHoverEvents(true);
}

void MyCentroidNodeSceneGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setFocused(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void MyCentroidNodeSceneGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setFocused(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

void MyCentroidNodeSceneGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    setFocused(false);
    MyNodeSceneGraphicsItemBase::mouseMoveEvent(event);
    setFocused(true);
}

void MyCentroidNodeSceneGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        setFocused(false);
    MyElementGraphicsItemBase::mouseReleaseEvent(event);
}

// MyNodeIconGraphicsItem

MyNodeIconGraphicsItem::MyNodeIconGraphicsItem(const QPointF& position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = position;
}

void MyNodeIconGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    for (QGraphicsItem* item : childItems())
        item->paint(painter, option, widget);
}
