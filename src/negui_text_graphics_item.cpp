#include "negui_text_graphics_item.h"
#include "negui_text_style.h"
#include "negui_resize_handled_graphics_item.h"

// MyTextGraphicsItemBase

MyTextGraphicsItemBase::MyTextGraphicsItemBase(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsTextItem(parent) {

}

void MyTextGraphicsItemBase::updateStyle() {
    if (isSetStyle()) {
        // plain-text
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

void MyTextGraphicsItemBase::setSelectedWithBorderColor(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItemBase::setSelectedWithFillColor(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItemBase::updateExtents(const QRectF& extents) {
    if (isSetStyle()) {
        // x
        ((MyTextStyleBase*)style())->setX(extents.x() - (_originalPosition.x() + _movedOriginalPosition.x()));
        
        // y
        ((MyTextStyleBase*)style())->setY(extents.y() - (_originalPosition.y() + _movedOriginalPosition.y()));
        
        // width
        ((MyTextStyleBase*)style())->setWidth(extents.width());
        
        // height
        ((MyTextStyleBase*)style())->setHeight(extents.height());
    }

    updateStyle();
}

QRectF MyTextGraphicsItemBase::getExtents() {
    return QRectF(((MyTextStyleBase*)style())->x() + _originalPosition.x() + _movedOriginalPosition.x(),
                  ((MyTextStyleBase*)style())->y() + _originalPosition.y() + _movedOriginalPosition.y(),
                  ((MyTextStyleBase*)style())->width(), ((MyTextStyleBase*)style())->height());
}

void MyTextGraphicsItemBase::moveOriginalPosition(const qreal& dx, const qreal& dy) {
    _movedOriginalPosition += QPointF(dx, dy);
}

void MyTextGraphicsItemBase::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
}

// MySimpleTextGraphicsItem

MySimpleTextGraphicsItem::MySimpleTextGraphicsItem(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent) : MyTextGraphicsItemBase(x, y, parent)  {
    _elementName = elementName;
}

void MySimpleTextGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        if (((MyTextStyleBase *) style())->plainText() == "text" &&
            ((MySimpleTextStyle *) style())->whetherSetNameAsDefaultPlainText())
            ((MyTextStyleBase *) style())->setPlainText(_elementName);
    }
    MyTextGraphicsItemBase::updateStyle();
}

QGraphicsItem* MySimpleTextGraphicsItem::getFocusedGraphicsItem() {
    return NULL;
}

// MyWithPlainTextTextGraphicsItem

MyWithPlainTextTextGraphicsItem::MyWithPlainTextTextGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : MyTextGraphicsItemBase(x, y, parent)  {

}

QGraphicsItem* MyWithPlainTextTextGraphicsItem::getFocusedGraphicsItem() {
    QRectF focusedRect = getExtents();
    MyResizeHandledGraphicsItemBase* focusedGraphicsItem = new MyResizeHandledGraphicsItem(focusedRect, zValue());
    MyShapeGraphicsItemBase::connect(focusedGraphicsItem, SIGNAL(rectIsUpdated(const QRectF&)), (MyShapeGraphicsItemBase*)this, SLOT(updateExtents(const QRectF&)));

    return focusedGraphicsItem;
}

