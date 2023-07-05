#ifndef __NEGUI_BEZIER_ADJUST_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_BEZIER_ADJUST_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyBezierAdjustHandledGraphicsItemsBase: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MyBezierAdjustHandledGraphicsItemsBase(qreal zValue, QGraphicsItem *parent = nullptr);

protected:
    bool _isPressed;
};

class MySinglePointBezierAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MySinglePointBezierAdjustHandledGraphicsItems(const QPointF& point, const QPointF& controlPoint, qreal zValue, QGraphicsItem *parent = nullptr);

    void createBezierAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint);

    void adjust(const QPointF& point);

signals:

    void controlPointIsUpdated(const QPointF&);

public slots:

    void updateAdjustHandledGraphicsItem(const QPointF& controlPoint);

protected:
    QGraphicsItem* _adjustHandledGraphicsItem;
    bool _isPressed;
};

class MyDoublePointBezierAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:

    MyDoublePointBezierAdjustHandledGraphicsItems(const QPointF& startPoint, const QPointF& controlPoint1, const QPointF& controlPoint2, const QPointF& endPoint, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createBezierStartAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint);
    
    void createBezierEndAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint);

    void adjust(const QPointF& controlPoint1, const QPointF& controlPoint2);
    
signals:
    
    void startControlPointIsUpdated(const QPointF&);

    void endControlPointIsUpdated(const QPointF&);
    
public slots:
    
    void updateStartAdjustHandledGraphicsItem(const QPointF& startControlPoint);
    
    void updateEndAdjustHandledGraphicsItem(const QPointF& endControlPoint);
    
protected:
    QGraphicsItem* _startAdjustHandledGraphicsItem;
    QGraphicsItem* _endAdjustHandledGraphicsItem;
    bool _isPressed;
};

class MyBezierAdjustHandledGraphicsItem: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyBezierAdjustHandledGraphicsItem(const QPointF& point, const QPointF& controlPoint, QGraphicsItem *parent = nullptr);
    
    void createLine(const QPointF& p1, const QPointF& p2);
    
    void createHandle(const QPointF& center);
    
    void updatePosition(const QPointF& controlPoint);
    
    const QPointF position();
    
signals:
    
    void adjustHandledGraphicsItemIsUpdated(const QPointF&);

    void isPressed();

    void isReleased();
    
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
    
    void updatePosition(const QPointF& center);
    
    const QPointF position();
    
signals:
    
    void positionChanged(const QPointF&);

    void isPressed();

    void isReleased();
    
protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    qreal _handleRadius;
};

#endif
