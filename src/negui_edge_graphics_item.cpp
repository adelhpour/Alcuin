#include "negui_edge_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include "negui_1d_shape_graphics_item_base.h"

// MyEdgeGraphicsItemBase

MyEdgeGraphicsItemBase::MyEdgeGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    enableNormalMode();
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::LINE_SHAPE_STYLE)
        item = createLineShape(_initialLine, this);
    else if (style->type() == MyShapeStyleBase::BEZIER_SHAPE_STYLE)
        item = createBezierShape(_initialLine, this);
    
    if (item)
        item->setZValue(zValue());
    
    return item;
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

QList<QGraphicsItem*> MyEdgeGraphicsItemBase::createResizeHandleBaredGraphicsItems() {
    return QList<QGraphicsItem*>();
}

void MyEdgeGraphicsItemBase::enableNormalMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeGraphicsItemBase::enableAddNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeGraphicsItemBase::enableSelectNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeGraphicsItemBase::enableAddEdgeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeGraphicsItemBase::enableSelectEdgeMode() {
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeGraphicsItemBase::enableRemoveMode() {
    setCursor(Qt::PointingHandCursor);
}

// MyEdgeSceneGraphicsItem

MyEdgeSceneGraphicsItem::MyEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 0.0, 0.0);
}

// MyEdgeIconGraphicsItem

MyEdgeIconGraphicsItem::MyEdgeIconGraphicsItem(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 60.0, 0.0);
}
