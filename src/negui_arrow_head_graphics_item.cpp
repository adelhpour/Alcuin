#include "negui_arrow_head_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include <QtMath>

// MyArrowHeadGraphicsItemBase

MyArrowHeadGraphicsItemBase::MyArrowHeadGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    enableNormalMode();
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::ELLIPSE_SHAPE_STYLE)
        item = createEllipseShape(_originalPosition.x() - _padding, _originalPosition.y(), this);
    else if (style->type() == MyShapeStyleBase::RECT_SHAPE_STYLE)
        item = createRectShape(_originalPosition.x() - _padding * qCos(qDegreesToRadians(45.0)), _originalPosition.y(), this);
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE)
        item = createPolygonShape(_originalPosition.x() + _padding, _originalPosition.y(), this);
    
    if (item)
        item->setZValue(zValue());
    
    return item;
}

QList<QGraphicsItem*> MyArrowHeadGraphicsItemBase::createResizeHandleBaredGraphicsItems() {
    return QList<QGraphicsItem*>();
}

void MyArrowHeadGraphicsItemBase::enableNormalMode() {
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadGraphicsItemBase::enableAddNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadGraphicsItemBase::enableSelectNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadGraphicsItemBase::enableAddEdgeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadGraphicsItemBase::enableSelectEdgeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadGraphicsItemBase::enableRemoveMode() {
    setCursor(Qt::ArrowCursor);
}

// MyArrowHeadSceneGraphicsItem

MyArrowHeadSceneGraphicsItem::MyArrowHeadSceneGraphicsItem(QGraphicsItem *parent) : MyArrowHeadGraphicsItemBase(parent) {
    _originalPosition = QPointF(0.0, 0.0);
    _padding = 2.5;
}

void MyArrowHeadSceneGraphicsItem::update(const QPointF& position, const qreal& rotation) {
    for (QGraphicsItem* item : childItems()) {
        item->setPos(position.x(), position.y());
        item->setRotation(qRadiansToDegrees(rotation));
    }
}

// MyArrowHeadIconGraphicsItem

MyArrowHeadIconGraphicsItem::MyArrowHeadIconGraphicsItem(QGraphicsItem *parent) : MyArrowHeadGraphicsItemBase(parent) {
    _originalPosition = QPointF(60.0, 0.0);
    _padding = 0.0;
}
