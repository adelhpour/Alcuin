#include "negui_arrow_head_graphics_item.h"
#include "negui_context_menu.h"

#include <QtMath>
#include <QPainter>

// MyArrowHeadGraphicsItemBase

MyArrowHeadGraphicsItemBase::MyArrowHeadGraphicsItemBase(QGraphicsItem *parent) : MyNetworkElementGraphicsItemBase(parent) {
    enableNormalMode();
}

const bool MyArrowHeadGraphicsItemBase::canAddEllipseShape() {
    return true;
}

const bool MyArrowHeadGraphicsItemBase::canAddRectangleShape() {
    return true;
}

const bool MyArrowHeadGraphicsItemBase::canAddPolygonShape() {
    return true;
}

QMenu* MyArrowHeadGraphicsItemBase::createContextMenu() {
    QMenu* contextMenu = new MyArrowHeadGraphicsItemContextMenu();
    connectContextMenu(contextMenu);
    ((MyContextMenuBase*)contextMenu)->initializeActionsStatus();
    return contextMenu;
}

// MyArrowHeadSceneGraphicsItem

MyArrowHeadSceneGraphicsItem::MyArrowHeadSceneGraphicsItem(QGraphicsItem *parent) : MyArrowHeadGraphicsItemBase(parent) {
    setZValue(1);
}

void MyArrowHeadSceneGraphicsItem::update(const QPointF& position, const qreal& rotation) {
    for (QGraphicsItem* item : childItems()) {
        item->setPos(position.x(), position.y());
        item->setRotation(qRadiansToDegrees(rotation));
    }
}

void MyArrowHeadSceneGraphicsItem::enableNormalMode() {
    MyNetworkElementGraphicsItemBase::enableNormalMode();
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadSceneGraphicsItem::enableAddNodeMode() {
    MyNetworkElementGraphicsItemBase::enableAddNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadSceneGraphicsItem::enableSelectNodeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectNodeMode();
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadSceneGraphicsItem::enableAddEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableAddEdgeMode();
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadSceneGraphicsItem::enableSelectEdgeMode() {
    MyNetworkElementGraphicsItemBase::enableSelectEdgeMode();
    setCursor(Qt::ArrowCursor);
}

void MyArrowHeadSceneGraphicsItem::enableDisplayFeatureMenuMode() {
    MyNetworkElementGraphicsItemBase::enableDisplayFeatureMenuMode();
    setCursor(Qt::ArrowCursor);
}

// MyArrowHeadIconGraphicsItem

MyArrowHeadIconGraphicsItem::MyArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation, QGraphicsItem *parent) : MyArrowHeadGraphicsItemBase(parent) {
    _originalPosition = position;
    _rotation = rotation;
}

void MyArrowHeadIconGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    for (QGraphicsItem* item : childItems()) {
        painter->translate(_originalPosition.x(), _originalPosition.y());
        painter->rotate(_rotation);
        painter->translate(-_originalPosition.x(), -_originalPosition.y());
        item->paint(painter, option, widget);
        painter->translate(_originalPosition.x(), _originalPosition.y());
        painter->rotate(-_rotation);
        painter->translate(-_originalPosition.x(), -_originalPosition.y());
    }
}
