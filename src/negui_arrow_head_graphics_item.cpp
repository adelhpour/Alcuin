#include "negui_arrow_head_graphics_item.h"

// MyArrowHeadGraphicsItemBase

MyArrowHeadGraphicsItemBase::MyArrowHeadGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultEllipseItem() {
    MyShapeGraphicsItemBase* item = new MyEllipseGraphicsItem(_initialPosition.x() - _padding, _initialPosition.y(), this);
    ((MyEllipseGraphicsItem*)item)->setZValue(zValue());
    return item;
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultRectItem() {
    MyShapeGraphicsItemBase* item = new MyRectGraphicsItem(_initialPosition.x() - _padding * qCos(qDegreesToRadians(45.0)), _initialPosition.y(), this);
    ((MyRectGraphicsItem*)item)->setZValue(zValue());
    return item;
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultPolygonItem() {
    MyShapeGraphicsItemBase* item = new MyPolygonGraphicsItem(_initialPosition.x() + _padding, _initialPosition.y(), this);
    ((MyPolygonGraphicsItem*)item)->setZValue(zValue());
    return item;
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultLineItem() {
    return NULL;
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultBezierItem() {
    return NULL;
}

MyShapeGraphicsItemBase* MyArrowHeadGraphicsItemBase::defaultTextItem() {
    return NULL;
}

// MyArrowHeadSceneGraphicsItem

MyArrowHeadSceneGraphicsItem::MyArrowHeadSceneGraphicsItem(QGraphicsItem *parent) : MyArrowHeadGraphicsItemBase(parent) {
    _initialPosition = QPointF(0.0, 0.0);
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
    _initialPosition = QPointF(60.0, 0.0);
    _padding = 0.0;
}
