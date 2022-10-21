#include "negui_text_graphics_item.h"
#include "negui_text_style.h"

// MyTextGraphicsItem

MyTextGraphicsItem::MyTextGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsTextItem(parent) {
}

void MyTextGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // position
        QPointF position = _originalPosition;
        position += QPointF(((MyTextStyle*)style())->x(), ((MyTextStyle*)style())->y());
    
        if (((MyTextStyle*)style())->width() > 0.001) {
            setTextWidth(((MyTextStyle*)style())->width());
            position += QPointF(-0.5 * ((MyTextStyle*)style())->width(), 0.0);
        }
        
        if (((MyTextStyle*)style())->height() > 0.001)
            position += QPointF(0.0, -0.5 * ((MyTextStyle*)style())->height());
        
        setPos(position);
        
        // plain-text
        setPlainText(((MyTextStyle*)style())->plainText());
        
        // text-color
        setDefaultTextColor(((MyTextStyle*)style())->defaultTextColor());
        
        // font
        setFont(((MyTextStyle*)style())->font());
        
        // alignment
        QTextOption option = document()->defaultTextOption();
        option.setAlignment(((MyTextStyle*)style())->verticalAlignment() | ((MyTextStyle*)style())->horizontalAlignment());
        document()->setDefaultTextOption(option);
    }
}

void MyTextGraphicsItem::setSelectedWithStroke(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItem::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyTextGraphicsItem::updateExtents(const QRectF& extents) {
    _originalPosition = QPointF(extents.x() + 0.5 * extents.width(), extents.y() + 0.5 * extents.height());
    
    if (isSetStyle()) {
        // x
        ((MyTextStyle*)style())->setX(0.0);
        
        // y
        ((MyTextStyle*)style())->setY(0.0);
        
        // width
        ((MyTextStyle*)style())->setWidth(extents.width());
        
        // height
        ((MyTextStyle*)style())->setHeight(extents.height());
    }
    
    updateStyle();
}

QRectF MyTextGraphicsItem::getExtents() const {
    return QRectF(x(), y(), boundingRect().width(), boundingRect().height());
}

void MyTextGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
