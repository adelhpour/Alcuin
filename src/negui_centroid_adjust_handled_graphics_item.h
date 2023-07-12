#ifndef __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_CENTROID_ADJUST_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>

class MyCentroidAdjustHandledGraphicsItems: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MyCentroidAdjustHandledGraphicsItems(const QRectF &rect, const QLineF &line, qreal zValue, QGraphicsItem *parent = nullptr);

    void createCentroidFocusedGraphicsItem(const QRectF &rect);

    void createBezierAdjustLineFocusedGraphicsItem(const QLineF &line);

signals:

    void bezierAdjustLineIsUpdated(const QLineF&);

protected:
    QGraphicsItem* _centroidFocusedGraphicsItem;
    QGraphicsItem* _centroidBezierAdjustHandledGraphicsItem;
};

class MyCentroidBezierAdjustHandledGraphicsItem: public QObject, public QGraphicsItemGroup {
    Q_OBJECT

public:

    MyCentroidBezierAdjustHandledGraphicsItem(const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem *parent = nullptr);

    void createLine(const QPointF& p1, const QPointF& p2);

    const QLineF line();

    void createStartHandle(const QPointF& center);

    void createEndHandle(const QPointF& center);

    const QPointF startPosition();

    void updateStartPosition(const QPointF& position);

    const QPointF endPosition();

    void updateEndPosition(const QPointF& position);

signals:

    void startPositionIsUpdated(const QPointF&);

    void endPositionIsUpdated(const QPointF&);

protected:
    QGraphicsItem* _line;
    QGraphicsItem* _startControlHandle;
    QGraphicsItem* _endControlHandle;

};

#endif
