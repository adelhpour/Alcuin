#ifndef __NEGUI_BEZIER_ADJUST_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_BEZIER_ADJUST_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyBezierAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyBezierAdjustHandledGraphicsItems(const QPointF& startPoint, const QPointF& controPoint1, const QPointF& controlPoint2, const QPointF& endPoint, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createBezierStartAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint);
    
    void createBezierEndAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint);
    
signals:
    
    void controlPointsAreUpdated(const QPointF&, const QPointF&);
    
public slots:
    
    void updateStartAdjustHandledGraphicsItem(const QPointF& startControlPoint);
    
    void updateEndAdjustHandledGraphicsItem(const QPointF& endControlPoint);
    
protected:
    QGraphicsItem* _startAdjustHandledGraphicsItem;
    QGraphicsItem* _endAdjustHandledGraphicsItem;
    
};

class MyBezierAdjustHandledGraphicsItem: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyBezierAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint, QGraphicsItem *parent = nullptr);
    
    void createLine(const QPointF& p1, const QPointF& p2);
    
    void createHandle(const QPointF& center);
    
    void updatePostion(const QPointF& controlPoint);
    
    const QPointF position();
    
signals:
    
    void adjustHandledGraphicsItemIsUpdated(const QPointF&);
    
protected:
    QGraphicsItem* _line;
    QGraphicsItem* _controlHandle;

};

class MyBezierAdjustLineGraphicsItem: public QGraphicsLineItem {
    
public:
    
    MyBezierAdjustLineGraphicsItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
};

class MyBezierControlPointHandleGraphicsItem: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    
public:
    
    MyBezierControlPointHandleGraphicsItem(const QPointF& center, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QPointF& center);
    
    const QPointF position();
    
signals:
    
    void positionChanged(const QPointF&);
    
protected:
    
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    
    qreal _handleRadius;
};

#endif
