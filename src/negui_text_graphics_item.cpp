#include "negui_text_graphics_item.h"
#include "negui_text_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyTextGraphicsItem

MyTextGraphicsItem::MyTextGraphicsItem(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsTextItem(parent) {
    _elementName = elementName;
}

void MyTextGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // plain-text
        if (!_elementName.isEmpty() && ((MyTextStyleBase*)style())->whetherSetNameAsDefaultPlainText()) {
            ((MyTextStyleBase*)style())->setPlainText(_elementName);
            setPlainText(_elementName);
        }
        else if (!((MyWithPlainTextTextStyle*)style())->plainText().isEmpty())
            setPlainText(((MyWithPlainTextTextStyle*)style())->plainText());
        
        // width
        if (((MyTextStyleBase*)style())->width() > 0.001)
            setTextWidth(((MyTextStyleBase*)style())->width());
        
        // text-color
        setDefaultTextColor(((MyTextStyleBase*)style())->defaultTextColor());
        
        // font
        setFont(((MyTextStyleBase*)style())->font());
        
        // alignment
        QTextOption option = document()->defaultTextOption();
        option.setAlignment(((MyTextStyleBase*)style())->verticalAlignment() | ((MyTextStyleBase*)style())->horizontalAlignment());
        document()->setDefaultTextOption(option);
        
        setPos(_originalPosition +  QPointF(((MyTextStyleBase*)style())->x(), ((MyTextStyleBase*)style())->y() + ((MyTextStyleBase*)style())->verticalPadding()));
    }
}

void MyTextGraphicsItem::setSelectedWithStroke(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItem::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItem::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // x
        ((MyTextStyleBase*)style())->setX(extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // y
        ((MyTextStyleBase*)style())->setY(extents.y() - (movedDistance().y() + _originalPosition.y()));
        
        // width
        ((MyTextStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyTextStyleBase*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyTextGraphicsItem::getExtents() {
    return QRectF(((MyTextStyleBase*)style())->x() + (movedDistance().x() + _originalPosition.x()), ((MyTextStyleBase*)style())->y() + (movedDistance().y() + _originalPosition.y()), ((MyTextStyleBase*)style())->width(), ((MyTextStyleBase*)style())->height());
}

void MyTextGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyTextStyleBase*)style())->setX(((MyTextStyleBase*)style())->x() - originalPositionMovedDistance.x());
    ((MyTextStyleBase*)style())->setY(((MyTextStyleBase*)style())->y() - originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyTextGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyResizeHandledGraphicsItem(focusedRect, zValue());
    MyShapeGraphicsItemBase::connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), (MyShapeGraphicsItemBase*)this, SLOT(updateExtents(const QRectF&)));

    return focusedGraphicsItem;
}

void MyTextGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}

