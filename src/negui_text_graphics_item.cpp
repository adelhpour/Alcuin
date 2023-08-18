#include "negui_text_graphics_item.h"
#include "negui_text_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyTextGraphicsItemBase

MyTextGraphicsItemBase::MyTextGraphicsItemBase(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsTextItem(parent) {
    _elementName = elementName;
}

void MyTextGraphicsItemBase::updateStyle() {
    if (isSetStyle()) {
        if (((MyTextStyle*)style())->width() > 0.001)
            setTextWidth(((MyTextStyle*)style())->width());
        
        // plain-text
        if (((MyTextStyle*)style())->whetherSetNameAsDefaultPlainText()) {
            ((MyTextStyle*)style())->setPlainText(_elementName);
            setPlainText(_elementName);
        }
        else if (!((MyTextStyle*)style())->plainText().isEmpty())
            setPlainText(((MyTextStyle*)style())->plainText());
        
        // text-color
        setDefaultTextColor(((MyTextStyle*)style())->defaultTextColor());
        
        // font
        setFont(((MyTextStyle*)style())->font());
        
        // alignment
        QTextOption option = document()->defaultTextOption();
        option.setAlignment(((MyTextStyle*)style())->verticalAlignment() | ((MyTextStyle*)style())->horizontalAlignment());
        document()->setDefaultTextOption(option);
        
        setPos(_originalPosition +  QPointF(((MyTextStyle*)style())->x(), ((MyTextStyle*)style())->y() + ((MyTextStyle*)style())->verticalPadding()));
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
        ((MyTextStyle*)style())->setX(extents.x() - (movedDistance().x() + _originalPosition.x()));
        
        // y
        ((MyTextStyle*)style())->setY(extents.y() - (movedDistance().y() + _originalPosition.y()));
        
        // width
        ((MyTextStyle*)style())->setWidth(extents.width());
        
        // height
        ((MyTextStyle*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyTextGraphicsItemBase::getExtents() {
    return QRectF(((MyTextStyle*)style())->x() + (movedDistance().x() + _originalPosition.x()), ((MyTextStyle*)style())->y() + (movedDistance().y() + _originalPosition.y()), ((MyTextStyle*)style())->width(), ((MyTextStyle*)style())->height());
}

void MyTextGraphicsItemBase::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {
    ((MyTextStyle*)style())->setX(((MyTextStyle*)style())->x() - originalPositionMovedDistance.x());
    ((MyTextStyle*)style())->setY(((MyTextStyle*)style())->y() - originalPositionMovedDistance.y());
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
