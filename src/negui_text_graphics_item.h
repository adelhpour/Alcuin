#ifndef __NEGUI_TEXT_GRAPHICS_ITEM_H
#define __NEGUI_TEXT_GRAPHICS_ITEM_H

#include "negui_shape_graphics_item_base.h"
#include "negui_text_style.h"

#include <QTextDocument>

class MyTextGraphicsItem: public MyShapeGraphicsItemBase, public QGraphicsTextItem {
public:
    
    MyTextGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    void updateExtents(const QRectF& extents) override;
    
    QRectF getExtents() const override;
};

#endif
