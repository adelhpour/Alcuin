#include "negui_network_element_graphics_item_base.h"
#include "negui_shape_graphics_item_builder.h"

#include <QGraphicsSceneMouseEvent>


// MyNetworkElementGraphicsItemBase

MyNetworkElementGraphicsItemBase::MyNetworkElementGraphicsItemBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    _isChosen = false;
    _originalPosition = QPointF(0.0, 0.0);
    connect(this, SIGNAL(mouseLeftButtonIsPressed()), this, SIGNAL(askForSelectNetworkElement()));
    connect(this, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SIGNAL(askForCreateFeatureMenu()));
    connect(this, &MyNetworkElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () {
        if (getSceneMode() == DISPLAY_FEATURE_MENU_MODE)
            askForCreateFeatureMenu();
    });
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

MyShapeGraphicsItemBase* MyNetworkElementGraphicsItemBase::createShapeGraphicsItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::ELLIPSE_SHAPE_STYLE && canAddEllipseShape()) {
        item = createEllipseShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::RECTANGLE_SHAPE_STYLE && canAddRectangleShape()) {
        item = createRectangleShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE && canAddPolygonShape()) {
        item = createPolygonShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
    }
    else if (style->type() == MyShapeStyleBase::SIMPLE_TEXT_SHAPE_STYLE && canAddTextShape()) {
        item = createSimpleTextShape(_originalPosition.x(), _originalPosition.y(), askForDisplayName(), this);
        item->setZValue(zValue() + 1);
    }
    else if (style->type() == MyShapeStyleBase::WITH_PLAIN_TEXT_TEXT_SHAPE_STYLE && canAddTextShape()) {
        item = createWithPlainTextTextShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue() + 1);
    }
    else if (style->type() == MyShapeStyleBase::CENTROID_SHAPE_STYLE && canAddCentroidShape()) {
        item = createCentroidShape(_originalPosition.x(), _originalPosition.y(), this);
        item->setZValue(zValue());
        connectShapeGraphicsItem(item);
    }

    return item;
}

void MyNetworkElementGraphicsItemBase::connectShapeGraphicsItem(MyShapeGraphicsItemBase* item) {

}

const bool MyNetworkElementGraphicsItemBase::canAddEllipseShape() {
    return false;
}

const bool MyNetworkElementGraphicsItemBase::canAddRectangleShape() {
    return false;
}

const bool MyNetworkElementGraphicsItemBase::canAddPolygonShape() {
    return false;
}

const bool MyNetworkElementGraphicsItemBase::canAddTextShape() {
    return false;
}

const bool MyNetworkElementGraphicsItemBase::canAddCentroidShape() {
    return false;
}

const bool MyNetworkElementGraphicsItemBase::canAddLineShape() {
    return false;
}

void MyNetworkElementGraphicsItemBase::connectContextMenu(QMenu* contextMenu) {
    connect(contextMenu, SIGNAL(askForCreateFeatureMenu()), this, SIGNAL(askForCreateFeatureMenu()));
    connect(contextMenu, SIGNAL(askForCopyNetworkElement()), this, SIGNAL(askForCopyNetworkElement()));
    connect(contextMenu, SIGNAL(askForCutNetworkElement()), this, SIGNAL(askForCutNetworkElement()));
    connect(contextMenu, SIGNAL(askForCopyNetworkElementStyle()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(contextMenu, SIGNAL(askForPasteNetworkElementStyle()), this, SIGNAL(askForPasteNetworkElementStyle()));
    connect(contextMenu, SIGNAL(askForDeleteNetworkElement()), this, SIGNAL(askForDeleteNetworkElement()));
    connect(contextMenu, SIGNAL(askForWhetherElementStyleIsCopied()), this, SIGNAL(askForWhetherElementStyleIsCopied()));
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

void MyNetworkElementGraphicsItemBase::updateFocusedGraphicsItems() {
    bool isSelected = _focusedGraphicsItems.size() ? true : false;
    clearFocusedGraphicsItems();
    if (isSelected)
        addFocusedGraphicsItems();
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

void MyNetworkElementGraphicsItemBase::setSelectedWithBorderColor(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithBorderColor(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyNetworkElementGraphicsItemBase::setSelectedWithFillColor(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithFillColor(selected);
    }
    QGraphicsItem::setSelected(selected);
}

void MyNetworkElementGraphicsItemBase::setFocused(const bool& isFocused) {
    if (!isFocused)
        clearFocusedGraphicsItems();
    else if (!_focusedGraphicsItems.size())
        addFocusedGraphicsItems();
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

void MyNetworkElementGraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _isChosen = true;
        emit mouseLeftButtonIsPressed();
        event->accept();
    }
    QGraphicsItem::mousePressEvent(event);
}

void MyNetworkElementGraphicsItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _isChosen = false;
        event->accept();
        emit askForCreateChangeStageCommand();
    }
    else if (event->button() == Qt::RightButton) {
        displayContextMenu(event->screenPos());
        event->accept();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyNetworkElementGraphicsItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseLeftButtonIsDoubleClicked();
        event->accept();
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void MyNetworkElementGraphicsItemBase::displayContextMenu(const QPoint& position) {
    if (getSceneMode() == NORMAL_MODE) {
        if (!(askForWhetherNetworkElementIsSelected() && askForWhetherAnyOtherElementsAreSelected())) {
            QMenu* contextMenu = createContextMenu();
            contextMenu->exec(position);
        }
    }
}