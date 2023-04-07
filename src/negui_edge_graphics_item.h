#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"

class MyEdgeGraphicsItemBase: public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
    
    void setLine(const QLineF &line);
    
    const qreal getEndSlope() const;
    
signals:
    
    void askForUpdateArrowHeadPlacement();

    void askForUpdateConnectedEdges(const QPointF&);

protected:
    QLineF _initialLine;
};

class MyEdgeSceneGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeSceneGraphicsItem(QGraphicsItem *parent = nullptr);
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void enableRemoveMode() override;
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    void focusOutEvent(QFocusEvent *event) override;
    
    QPointF _mousePressedPosition;
};

class MyEdgeIconGraphicsItem: public MyEdgeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
