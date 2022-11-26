#ifndef __NEGUI_TEXT_GRAPHICS_ITEM_H
#define __NEGUI_TEXT_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"
#include <QTextDocument>

class MyTextGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsTextItem {
public:
    
    MyTextGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getResizeHandlebaredGraphicsItem() override;
    
    void setZValue(qreal z) override;
    
public slots:
    void updateExtents(const QRectF& extents) override;
};

#endif