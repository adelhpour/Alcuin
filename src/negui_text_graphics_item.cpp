#include "negui_text_graphics_item.h"
#include "negui_text_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyTextGraphicsItemBase

MyTextGraphicsItemBase::MyTextGraphicsItemBase(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsTextItem(parent) {
    _elementName = elementName;
}

void MyTextGraphicsItemBase::updateStyle() {
    if (isSetStyle()) {
        if (((MyTextStyleBase*)style())->width() > 0.001)
            setTextWidth(((MyTextStyleBase*)style())->width());
        
        // plain-text
        if (((MyTextStyleBase*)style())->whetherSetNameAsDefaultPlainText()) {
            ((MyTextStyleBase*)style())->setPlainText(_elementName);
            setPlainText(_elementName);
        }
        else if (!((MyTextStyleBase*)style())->plainText().isEmpty())
            setPlainText(((MyTextStyleBase*)style())->plainText());
        
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

void MyTextGraphicsItemBase::setSelectedWithStroke(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItemBase::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItemBase::updateExtents(const QRectF& extents) {
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

QRectF MyTextGraphicsItemBase::getExtents() {
    return QRectF(((MyTextStyleBase*)style())->x() + (movedDistance().x() + _originalPosition.x()), ((MyTextStyleBase*)style())->y() + (movedDistance().y() + _originalPosition.y()), ((MyTextStyleBase*)style())->width(), ((MyTextStyleBase*)style())->height());
}

void MyTextGraphicsItemBase::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyTextStyleBase*)style())->setX(((MyTextStyleBase*)style())->x() - originalPositionMovedDistance.x());
    ((MyTextStyleBase*)style())->setY(((MyTextStyleBase*)style())->y() - originalPositionMovedDistance.y());
    _originalPosition += originalPositionMovedDistance;
}

QGraphicsItem* MyTextGraphicsItemBase::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyResizeHandledGraphicsItem(focusedRect, zValue());
    MyShapeGraphicsItemBase::connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), (MyShapeGraphicsItemBase*)this, SLOT(updateExtents(const QRectF&)));

    return focusedGraphicsItem;
}

void MyTextGraphicsItemBase::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}
