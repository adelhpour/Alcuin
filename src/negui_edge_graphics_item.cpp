#include "negui_edge_graphics_item.h"

// MyEdgeGraphicsItemBase

MyEdgeGraphicsItemBase::MyEdgeGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultEllipseItem() {
    return NULL;
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultRectItem() {
    return NULL;
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultPolygonItem() {
    return NULL;
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultLineItem() {
    MyShapeGraphicsItemBase* item = new MyLineGraphicsItem(_initialLine, this);
    ((MyLineGraphicsItem*)item)->setZValue(zValue());
    return item;
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultBezierItem() {
    MyShapeGraphicsItemBase* item = new MyBezierGraphicsItem(_initialLine, this);
    ((MyBezierGraphicsItem*)item)->setZValue(zValue());
    return item;
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::defaultTextItem() {
    return NULL;
}

void MyEdgeGraphicsItemBase::setLine(const QLineF &line) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setLine(line);
    }
}

const qreal MyEdgeGraphicsItemBase::getEndSlope() const {
    qreal endSlope = 0.0;
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
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
}

// MyEdgeIconGraphicsItem

MyEdgeIconGraphicsItem::MyEdgeIconGraphicsItem(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 60.0, 0.0);
}
