#ifndef __NEGUI_BEZIER_CONTROL_POINT_HANDLE_GRAPHICS_ITEM_H
#define __NEGUI_BEZIER_CONTROL_POINT_HANDLE_GRAPHICS_ITEM_H

#include <QGraphicsItem>

class MyBezierControlPointHandleGraphicsItem: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:

    MyBezierControlPointHandleGraphicsItem(const QPointF& center, QGraphicsItem *parent = nullptr);

    void setPen();

    void setBrush();

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

class MyBezierAdjustLineGraphicsItem: public QGraphicsLineItem {

public:

    MyBezierAdjustLineGraphicsItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
};

#endif
