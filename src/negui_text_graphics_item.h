#ifndef __NEGUI_TEXT_GRAPHICS_ITEM_H
#define __NEGUI_TEXT_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"
#include <QTextDocument>
#include <QPainter>

class MyTextGraphicsItemBase: public My2DShapeGraphicsItemBase, public QGraphicsTextItem {
public:
    
    MyTextGraphicsItemBase(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithBorderColor(const bool& selected) override;
    
    void setSelectedWithFillColor(const bool& selected) override;
    
    QRectF getExtents() override;
    
    void setZValue(qreal z) override;

    void moveOriginalPosition(const qreal& dx, const qreal& dy) override;
    
public slots:

    void updateExtents(const QRectF& extents) override;
};

class MySimpleTextGraphicsItem: public MyTextGraphicsItemBase {
public:

    MySimpleTextGraphicsItem(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent);

    void updateStyle() override;

    QGraphicsItem* getFocusedGraphicsItem() override;

protected:

    QString _elementName;
};

class MyWithPlainTextTextGraphicsItem: public MyTextGraphicsItemBase {
public:

    MyWithPlainTextTextGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);

    QGraphicsItem* getFocusedGraphicsItem() override;
};


#endif
