#include "negui_element_graphics_item_base.h"

// MyElementGraphicsItemBase

MyElementGraphicsItemBase::MyElementGraphicsItemBase(QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    _isChosen = false;
    _isSelected = false;
}

void MyElementGraphicsItemBase::addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue) {
    _shapeStyles = shapeStyles;
    setZValue(zValue);
    for (MyShapeStyleBase* style : qAsConst(shapeStyles))
        addShapeItem(style);
}

void MyElementGraphicsItemBase::addShapeItem(MyShapeStyleBase* style) {
    MyShapeGraphicsItemBase* item = NULL;
    if (style->type() == MyShapeStyleBase::ELLIPSE_SHAPE_STYLE)
        item = defaultEllipseItem();
    else if (style->type() == MyShapeStyleBase::RECT_SHAPE_STYLE)
        item = defaultRectItem();
    else if (style->type() == MyShapeStyleBase::POLYGON_SHAPE_STYLE)
        item = defaultPolygonItem();
    else if (style->type() == MyShapeStyleBase::LINE_SHAPE_STYLE)
        item = defaultLineItem();
    else if (style->type() == MyShapeStyleBase::BEZIER_SHAPE_STYLE)
        item = defaultBezierItem();
    else if (style->type() == MyShapeStyleBase::TEXT_SHAPE_STYLE)
        item = defaultTextItem();
    
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
    _isSelected = selected;
}

void MyElementGraphicsItemBase::setSelectedWithFill(const bool& selected) {
    for (QGraphicsItem* item : childItems()) {
        MyShapeGraphicsItemBase* casted_item = dynamic_cast<MyShapeGraphicsItemBase*>(item);
        if (casted_item)
            casted_item->setSelectedWithFill(selected);
    }
    QGraphicsItem::setSelected(selected);
    _isSelected = selected;
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
        QWidget* elementFeatureMenu = emit askForElementFeatureMenu();
        if (elementFeatureMenu) {
            MyFeatureMenu* featureMenu =  new MyFeatureMenu(elementFeatureMenu);
            featureMenu->setShapeStyles(getShapeStyles());
            
            if (featureMenu->exec() == QDialog::Accepted) {
                emit askForSetShapeStyles(featureMenu->getShapeStyles());
                event->accept();
                return;
            }
        }
    }
    event->ignore();
}
