#include "negui_element_graphics_item_base.h"

#include <QGraphicsSceneMouseEvent>

// MyElementGraphicsItemBase

MyElementGraphicsItemBase::MyElementGraphicsItemBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    _isChosen = false;
}

void MyElementGraphicsItemBase::addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _shapeStyles = shapeStyles;
    setZValue(zValue);
    for (MyShapeStyleBase* style : qAsConst(shapeStyles))
        addShapeItem(style);
    clearResizeHandleBaredGraphicsItems();
}

void MyElementGraphicsItemBase::addShapeItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = createShapeGraphicsItem(style);
    if (item) {
        item->setStyle(style);
        item->updateStyle();
        QGraphicsItem* casted_item = dynamic_cast<QGraphicsItem*>(item);
        if (casted_item)
            addToGroup(casted_item);
    }
}

QList<MyShapeStyleBase*> MyElementGraphicsItemBase::getShapeStyles() {
    return _shapeStyles;
}

void MyElementGraphicsItemBase::setSelectedWithStroke(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithStroke(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyElementGraphicsItemBase::setSelectedWithFill(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithFill(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyElementGraphicsItemBase::setFocused(const bool& isFocused) {
    if (isFocused)
        addResizeHandleBaredGraphicsItems();
    else
        clearResizeHandleBaredGraphicsItems();
}

void MyElementGraphicsItemBase::addResizeHandleBaredGraphicsItems() {
    clearResizeHandleBaredGraphicsItems();
    _resizeHandlebaredGraphicsItems = createResizeHandleBaredGraphicsItems();
    for (QGraphicsItem* item : _resizeHandlebaredGraphicsItems)
        emit askForAddGraphicsItem(item);
}

void MyElementGraphicsItemBase::clearResizeHandleBaredGraphicsItems() {
    for (QGraphicsItem* item : _resizeHandlebaredGraphicsItems) {
        emit askForRemoveGraphicsItem(item);
        delete item;
    }
    _resizeHandlebaredGraphicsItems.clear();
}


void MyElementGraphicsItemBase::setCursor(const QCursor &cursor) {
    for (QGraphicsItem* item : childItems())
        item->setCursor(cursor);
    QGraphicsItemGroup::setCursor(cursor);
}
 
void MyElementGraphicsItemBase::clear() {
    for (QGraphicsItem* item : childItems()) {
        removeFromGroup(item);
        delete item;
    }
}

void MyElementGraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        _isChosen = true;
        emit mouseLeftButtonIsPressed();
        event->accept();
    }
}

void MyElementGraphicsItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        _isChosen = false;
        event->accept();
    }
}

void MyElementGraphicsItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        event->accept();
        emit mouseLeftButtonIsDoubleClicked();
        return;
    }
    event->ignore();
}
