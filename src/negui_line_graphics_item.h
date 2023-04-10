#ifndef __NEGUI_LINE_GRAPHICS_ITEM_H
#define __NEGUI_LINE_GRAPHICS_ITEM_H

#include "negui_1d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyLineGraphicsItem: public My1DShapeGraphicsItemBase, public QGraphicsPathItem {
    Q_OBJECT
    
public:
    
    MyLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithStroke(const bool& selected) override;
    
    void setSelectedWithFill(const bool& selected) override;
    
    const QPointF getStartPoint();
    
    const QPointF getControlPoint1();
    
    const QPointF getControlPoint2();
    
    const QPointF getEndPoint();
    
    const qreal getEndSlope() override;
    
    QRectF getExtents() override;
    
    QGraphicsItem* getFocusedGraphicsItem() override;
    
    void setZValue(qreal z) override;
    
    void setLine(const QLineF& line) override;
    
signals:
    
    void lineControlPoint1IsUpdated(const QPointF&);

    void lineControlPoint2IsUpdated(const QPointF&);
    
public slots:
    
    void setControlPoint1(const QPointF& controlPoint1);

    void setControlPoint2(const QPointF& controlPoint2);

    void adjustLineControlPoint1ToControlBezierLine(const QLineF& controlBezierLine);

    void adjustLineControlPoint2ToControlBezierLine(const QLineF& controlBezierLine);
};

#endif
