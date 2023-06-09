#include "negui_network_element_graphics_item_base.h"
#include "negui_context_menu.h"

#include <QGraphicsSceneMouseEvent>


// MyNetworkElementGraphicsItemBase

MyNetworkElementGraphicsItemBase::MyNetworkElementGraphicsItemBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    _isChosen = false;
    _originalPosition = QPointF(0.0, 0.0);
    connect(this, SIGNAL(mouseLeftButtonIsPressed()), this, SIGNAL(askForClearFocusedGraphicsItems()));
    connect(this, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SIGNAL(askForClearFocusedGraphicsItems()));
}

void MyNetworkElementGraphicsItemBase::update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _originalPosition += pos();
    setPos(0.0, 0.0);
    clear();
    addShapeItems(shapeStyles, zValue);
}

void MyNetworkElementGraphicsItemBase::addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _shapeStyles = shapeStyles;
    setZValue(zValue);
    for (MyShapeStyleBase* style : qAsConst(shapeStyles))
        addShapeItem(style);
}

void MyNetworkElementGraphicsItemBase::addShapeItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = createShapeGraphicsItem(style);
    if (item) {
        item->setStyle(style);
        item->updateStyle();
        QGraphicsItem* casted_item = dynamic_cast<QGraphicsItem*>(item);
        if (casted_item)
            addToGroup(casted_item);
    }
}

QList<MyShapeStyleBase*> MyNetworkElementGraphicsItemBase::getShapeStyles() {
    return _shapeStyles;
}

QList<QGraphicsItem*> MyNetworkElementGraphicsItemBase::createFocusedGraphicsItems() {
    QList<QGraphicsItem*> focusedGraphicsItems;
    QGraphicsItem* focusedGraphicsItem = NULL;
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item) {
            focusedGraphicsItem = casted_item->getFocusedGraphicsItem();
            if (focusedGraphicsItem)
                focusedGraphicsItems.push_back((focusedGraphicsItem));
        }
    }
    
    return focusedGraphicsItems;
}

void MyNetworkElementGraphicsItemBase::addFocusedGraphicsItems() {
    _focusedGraphicsItems = createFocusedGraphicsItems();
    for (QGraphicsItem* item : _focusedGraphicsItems)
        emit askForAddGraphicsItem(item);
}

void MyNetworkElementGraphicsItemBase::clearFocusedGraphicsItems() {
    for (QGraphicsItem* item : _focusedGraphicsItems) {
        emit askForRemoveGraphicsItem(item);
        delete item;
    }
    _focusedGraphicsItems.clear();
}

const QRectF MyNetworkElementGraphicsItemBase::getExtents() const {
    if (childItems().size()) {
        QRectF childExtents = dynamic_cast<MyShapeGraphicsItemBase*>(childItems().at(0))->getExtents();
        qreal extentsX = childExtents.x();
        qreal extentsY = childExtents.y();
        qreal extentsWidth = childExtents.width();
        qreal extentsHeight = childExtents.height();
        for (QGraphicsItem* childItem : childItems()) {
            MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(childItem);
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
        return QRectF(extentsX, extentsY, extentsWidth, extentsHeight);
    }

    return QRectF(_originalPosition.x() + x(), _originalPosition.y() + y(), 0.0, 0.0);
}

void MyNetworkElementGraphicsItemBase::setSelectedWithStroke(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithStroke(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyNetworkElementGraphicsItemBase::setSelectedWithFill(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithFill(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyNetworkElementGraphicsItemBase::setFocused(const bool& isFocused) {
    if (isFocused && getSceneMode() == NORMAL_MODE)
        addFocusedGraphicsItems();
    else
        clearFocusedGraphicsItems();
}

void MyNetworkElementGraphicsItemBase::setCursor(const QCursor &cursor) {
    for (QGraphicsItem* item : childItems())
        item->setCursor(cursor);
    QGraphicsItemGroup::setCursor(cursor);
}
 
void MyNetworkElementGraphicsItemBase::clear() {
    for (QGraphicsItem* item : childItems()) {
        removeFromGroup(item);
        delete item;
    }
}

void MyNetworkElementGraphicsItemBase::setZValue(qreal z) {
    for (QGraphicsItem* item : childItems())
        item->setZValue(item->zValue() + (z - zValue()));
    QGraphicsItemGroup::setZValue(z);
}

void MyNetworkElementGraphicsItemBase::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    clearFocusedGraphicsItems();
}

void MyNetworkElementGraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        _isChosen = true;
        emit mouseLeftButtonIsPressed();
        event->accept();
    }
}

void MyNetworkElementGraphicsItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        _isChosen = false;
        event->accept();
        emit askForCreateChangeStageCommand();
    }
}

void MyNetworkElementGraphicsItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        event->accept();
        emit mouseLeftButtonIsDoubleClicked();
        return;
    }
    event->ignore();
}

void MyNetworkElementGraphicsItemBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QGraphicsItem::contextMenuEvent(event);
    MyGraphicsItemContextMenu contextMenu;
    contextMenu.exec(QPoint(event->scenePos().x(), event->scenePos().y()));
    event->accept();
}
