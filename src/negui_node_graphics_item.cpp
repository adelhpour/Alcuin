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
        item = createEllipseShape(_originalPosition.x() - x(), _originalPosition.y() - y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::RECT_SHAPE_STYLE) {
        item = createRectShape(_originalPosition.x() - x(), _originalPosition.y() - y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE) {
        item = createPolygonShape(_originalPosition.x() - x(), _originalPosition.y() - y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::TEXT_SHAPE_STYLE) {
        item = createTextShape(_originalPosition.x() - x(), _originalPosition.y() - y(), this);
        item->setZValue(zValue() + 1);
    }
    
    if (item)
        item->setMovedDistance(QPointF(x(), y()));
    
    return item;
}

QList<QGraphicsItem*> MyNodeGraphicsItemBase::createResizeHandleBaredGraphicsItems() {
    QList<QGraphicsItem*> resizeHandlebaredGraphicsItems;
    QGraphicsItem* resizeHandlebaredGraphicsItem = NULL;
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item) {
            resizeHandlebaredGraphicsItem = casted_item->getResizeHandlebaredGraphicsItem();
            if (resizeHandlebaredGraphicsItem)
                resizeHandlebaredGraphicsItems.push_back((resizeHandlebaredGraphicsItem));
        }
    }
    
    return resizeHandlebaredGraphicsItems;
}

void MyNodeGraphicsItemBase::enableNormalMode() {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    clearResizeHandleBaredGraphicsItems();
}

void MyNodeGraphicsItemBase::enableAddNodeMode() {
    setCursor(Qt::ArrowCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeGraphicsItemBase::enableSelectNodeMode() {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeGraphicsItemBase::enableAddEdgeMode() {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeGraphicsItemBase::enableSelectEdgeMode() {
    setCursor(Qt::ArrowCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MyNodeGraphicsItemBase::enableRemoveMode() {
    setCursor(Qt::PointingHandCursor);
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

// MyNodeSceneGraphicsItem

MyNodeSceneGraphicsItem::MyNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = position;
    
    // make it movable
    setFlag(QGraphicsItem::ItemIsMovable, true);
    
    // make it send position changes
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    
    // make it focusable
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    
    _reparent = false;
}

void MyNodeSceneGraphicsItem::moveBy(qreal dx, qreal dy) {
    if (qFabs(dx) > 0.0001 || qFabs(dy) > 0.0001)
        QGraphicsItem::moveBy(dx, dy);
    else
        emit extentsModified();
}

void MyNodeSceneGraphicsItem::updateExtents(const QRectF& extents) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->updateExtents(QRectF(extents.x(), extents.y(), extents.width(), extents.height()));
    }
    extentsModified();
}

QRectF MyNodeSceneGraphicsItem::getExtents() const {
    QRectF extents = boundingRect();
    if (childItems().size()) {
        QRectF childExtents;
        for (QGraphicsItem* childItem : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(childItem);
            childExtents = casted_item->getExtents();
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
    
    return extents;
}

QVariant MyNodeSceneGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        if (_reparent)
            emit askForDeparent();
        for (QGraphicsItem* item : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
            if (casted_item)
                casted_item->setMovedDistance(value.toPointF());
        }
        _originalPosition += QPointF(value.toPointF().x() - x(), value.toPointF().y() - y());
        emit positionChanged(_originalPosition);
    }
    
    return QGraphicsItem::itemChange(change, value);
}

void MyNodeSceneGraphicsItem::keyPressEvent(QKeyEvent *event) {
    QGraphicsItem::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            if (_isChosen)
                _reparent = true;
            event->accept();
        }
    }
}

void MyNodeSceneGraphicsItem::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsItem::keyReleaseEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            emit askForReparent();
            _reparent = false;
            event->accept();
        }
    }
}

void MyNodeSceneGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        _mousePressedPosition = event->scenePos();
}

void MyNodeSceneGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (qAbs(_mousePressedPosition.x() - event->scenePos().x()) < 0.01 && qAbs(_mousePressedPosition.y() - event->scenePos().y()) < 0.01) {
            setFocused(true);
            setFlag(QGraphicsItem::ItemIsMovable, false);
        }
    }
}

void MyNodeSceneGraphicsItem::focusOutEvent(QFocusEvent *event) {
    setFocused(false);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::focusOutEvent(event);
}

// MyNodeIconGraphicsItem

MyNodeIconGraphicsItem::MyNodeIconGraphicsItem(QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _originalPosition = QPointF(0.0, 0.0);
}
