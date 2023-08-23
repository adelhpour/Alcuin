#ifndef __NEGUI_LINE_GRAPHICS_ITEM_H
#define __NEGUI_LINE_GRAPHICS_ITEM_H

#include "negui_1d_shape_graphics_item_base.h"
#include <QGraphicsItem>

class MyLineGraphicsItemBase: public My1DShapeGraphicsItemBase, public QGraphicsPathItem {
    Q_OBJECT
    
public:

    MyLineGraphicsItemBase(const QLineF& line, QGraphicsItem *parent);
    
    void updateStyle() override;
    
    void setSelectedWithBorderColor(const bool& selected) override;
    
    void setSelectedWithFillColor(const bool& selected) override;
    
    const QPointF getStartPoint();
    
    const QPointF getControlPoint1();
    
    const QPointF getControlPoint2();
    
    const QPointF getEndPoint();
    
    const qreal getEndSlope() override;
    
    QRectF getExtents() override;
    
    void setZValue(qreal z) override;
    
    void setLine(const QLineF& line) override;

    void resetLine();
    
signals:
    
    void lineControlPoint1IsUpdated(const QPointF&);

    void lineControlPoint2IsUpdated(const QPointF&);
    
public slots:
    
    void setControlPoint1(const QPointF& controlPoint1);

    void setControlPoint2(const QPointF& controlPoint2);

    void adjustLineControlPoint1ToControlBezierLine(const QLineF& controlBezierLine);

    void adjustLineControlPoint2ToControlBezierLine(const QLineF& controlBezierLine);
};

class MyClassicLineGraphicsItem: public MyLineGraphicsItemBase {
    Q_OBJECT

public:

    MyClassicLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);

    QGraphicsItem* getFocusedGraphicsItem() override;
};

class MyConnectedToCentroidShapeLineGraphicsItemBase: public MyLineGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToCentroidShapeLineGraphicsItemBase(const QLineF& line, QGraphicsItem *parent);

public slots:

    virtual const QPointF connectedToCentroidNodeControlPoint() = 0;

};

class MyConnectedToStartCentroidShapeLineGraphicsItem: public MyConnectedToCentroidShapeLineGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToStartCentroidShapeLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);

    QGraphicsItem* getFocusedGraphicsItem() override;

public slots:

    const QPointF connectedToCentroidNodeControlPoint() override;
};

class MyConnectedToEndCentroidShapeLineGraphicsItem: public MyConnectedToCentroidShapeLineGraphicsItemBase {
    Q_OBJECT

public:

    MyConnectedToEndCentroidShapeLineGraphicsItem(const QLineF& line, QGraphicsItem *parent);

    QGraphicsItem* getFocusedGraphicsItem() override;

public slots:

    const QPointF connectedToCentroidNodeControlPoint() override;
};

#endif
