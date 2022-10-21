#include "negui_node_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include "negui_2d_shape_graphics_item_base.h"
#include <QtMath>

// MyNodeGraphicsItemBase

MyNodeGraphicsItemBase::MyNodeGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    
}

MyShapeGraphicsItemBase* MyNodeGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::ELLIPSE_SHAPE_STYLE)
        item = createEllipseShape(_initialPosition.x(), _initialPosition.y(), this);
    else if (style->type() == MyShapeStyleBase::RECT_SHAPE_STYLE)
        item = createRectShape(_initialPosition.x(), _initialPosition.y(), this);
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE)
        item = createPolygonShape(_initialPosition.x(), _initialPosition.y(), this);
    else if (style->type() == MyShapeStyleBase::TEXT_SHAPE_STYLE) {
        item = createTextShape(_initialPosition.x(), _initialPosition.y(), this);
        item->setZValue(zValue() + 1);
    }
    
    if (item)
        item->setZValue(zValue());
    
    return item;
}

// MyNodeSceneGraphicsItem

MyNodeSceneGraphicsItem::MyNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _initialPosition = position;
    
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
        My2DShapeGraphicsItemBase* casted_item = dynamic_cast<My2DShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->updateExtents(QRectF(extents.x() - x(), extents.y() - y(), extents.width(), extents.height()));
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
        emit positionChanged(value.toPointF() + _initialPosition);
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

// MyNodeIconGraphicsItem

MyNodeIconGraphicsItem::MyNodeIconGraphicsItem(QGraphicsItem *parent) : MyNodeGraphicsItemBase(parent) {
    _initialPosition = QPointF(0.0, 0.0);
}
