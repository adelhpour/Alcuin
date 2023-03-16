#include "negui_element_graphics_item_base.h"

#include <QGraphicsSceneMouseEvent>

// MyElementGraphicsItemBase

MyElementGraphicsItemBase::MyElementGraphicsItemBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    _isChosen = false;
    _originalPosition = QPointF(0.0, 0.0);
    connect(this, SIGNAL(mouseLeftButtonIsPressed()), this, SIGNAL(askForClearResizeHandledGraphicsItems()));
    connect(this, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SIGNAL(askForClearResizeHandledGraphicsItems()));
}

void MyElementGraphicsItemBase::update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _originalPosition += pos();
    setPos(0.0, 0.0);
    clear();
    addShapeItems(shapeStyles, zValue);
}

void MyElementGraphicsItemBase::addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _shapeStyles = shapeStyles;
    setZValue(zValue);
    for (MyShapeStyleBase* style : qAsConst(shapeStyles))
        addShapeItem(style);
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

QList<QGraphicsItem*> MyElementGraphicsItemBase::createResizeHandledGraphicsItems() {
    QList<QGraphicsItem*> resizeHandledGraphicsItems;
    QGraphicsItem* resizeHandledGraphicsItem = NULL;
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item) {
            resizeHandledGraphicsItem = casted_item->getResizeHandledGraphicsItem();
            if (resizeHandledGraphicsItem)
                resizeHandledGraphicsItems.push_back((resizeHandledGraphicsItem));
        }
    }
    
    return resizeHandledGraphicsItems;
}

void MyElementGraphicsItemBase::addResizeHandledGraphicsItems() {
    _resizeHandledGraphicsItems = createResizeHandledGraphicsItems();
    for (QGraphicsItem* item : _resizeHandledGraphicsItems)
        emit askForAddGraphicsItem(item);
}

void MyElementGraphicsItemBase::clearResizeHandledGraphicsItems() {
    for (QGraphicsItem* item : _resizeHandledGraphicsItems) {
        emit askForRemoveGraphicsItem(item);
        delete item;
    }
    _resizeHandledGraphicsItems.clear();
}

const QRectF MyElementGraphicsItemBase::getExtents() const {
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
        addResizeHandledGraphicsItems();
    else
        clearResizeHandledGraphicsItems();
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

void MyElementGraphicsItemBase::setZValue(qreal z) {
    for (QGraphicsItem* item : childItems())
        item->setZValue(item->zValue() + (z - zValue()));
    QGraphicsItemGroup::setZValue(z);
}

void MyElementGraphicsItemBase::enableNormalMode() {
    clearResizeHandledGraphicsItems();
}

void MyElementGraphicsItemBase::enableAddNodeMode() {
    
}

void MyElementGraphicsItemBase::enableSelectNodeMode() {
    
}

void MyElementGraphicsItemBase::enableAddEdgeMode() {
    
}

void MyElementGraphicsItemBase::enableSelectEdgeMode() {
    
}

void MyElementGraphicsItemBase::enableRemoveMode() {
    
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
        emit askForCreateChangeStageCommand();
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
