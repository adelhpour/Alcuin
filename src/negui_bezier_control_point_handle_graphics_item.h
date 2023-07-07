#ifndef __NEGUI_BEZIER_CONTROL_POINT_HANDLE_GRAPHICS_ITEM_H
#define __NEGUI_BEZIER_CONTROL_POINT_HANDLE_GRAPHICS_ITEM_H

#include <QGraphicsItem>

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
