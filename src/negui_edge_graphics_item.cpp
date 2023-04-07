#include "negui_edge_graphics_item.h"
#include "negui_shape_graphics_item_builder.h"
#include "negui_1d_shape_graphics_item_base.h"

// MyEdgeGraphicsItemBase

MyEdgeGraphicsItemBase::MyEdgeGraphicsItemBase(QGraphicsItem *parent) : MyElementGraphicsItemBase(parent) {
    enableNormalMode();
}

MyShapeGraphicsItemBase* MyEdgeGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::LINE_SHAPE_STYLE) {
        item = createLineShape(_initialLine, this);
        connect(item, SIGNAL(lineControlPointIsUpdated(const QPointF&)), SIGNAL(askForUpdateArrowHeadPlacement()));
        connect(item, SIGNAL(lineControlPointIsUpdated(const QPointF&)), this, SIGNAL(askForUpdateConnectedEdges(const QPointF)));
        item->setZValue(zValue());
    }
        
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

// MyEdgeSceneGraphicsItem

MyEdgeSceneGraphicsItem::MyEdgeSceneGraphicsItem(QGraphicsItem *parent) : MyEdgeGraphicsItemBase(parent) {
    _initialLine = QLineF(0.0, 0.0, 0.0, 0.0);
    setZValue(0);
}

void MyEdgeSceneGraphicsItem::enableNormalMode() {
    MyElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableAddNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableSelectNodeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableAddEdgeMode() {
    setCursor(Qt::ArrowCursor);
}

void MyEdgeSceneGraphicsItem::enableSelectEdgeMode() {
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeSceneGraphicsItem::enableRemoveMode() {
    setCursor(Qt::PointingHandCursor);
}

void MyEdgeSceneGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
        _mousePressedPosition = event->scenePos();
}

void MyEdgeSceneGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    MyElementGraphicsItemBase::mouseReleaseEvent(event);
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
