#ifndef __NEGUI_CENTROID_GRAPHICS_ITEM_H
#define __NEGUI_CENTROID_GRAPHICS_ITEM_H

#include "negui_2d_shape_graphics_item_base.h"

class MyCentroidGraphicsItem: public My2DShapeGraphicsItemBase, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    
    MyCentroidGraphicsItem(qreal x, qreal y, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithBorderColor(const bool& selected) override;
    
    void setSelectedWithFillColor(const bool& selected) override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getFocusedGraphicsItem() override;

    const QRectF getFocusedRect();
    
    void setZValue(qreal z) override;

    void moveOriginalPosition(const qreal& dx, const qreal& dy) override;

signals:

    const QLineF askForGetBezierAdjustLine();

    void bezierAdjustLineIsUpdated(const QLineF&);
    
public slots:

    void updateExtents(const QRectF& extents) override;
};

#endif
