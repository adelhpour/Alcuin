#ifndef __NEGUI_RESIZE_HANDLED_GRAPHICS_ITEM_H
#define __NEGUI_RESIZE_HANDLED_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyResizeHandledGraphicsItemBase: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyResizeHandledGraphicsItemBase(const QRectF &rect, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createResizeRectItem(const QRectF &rect);
    
    virtual void createResizeHandledItems(const QRectF &rect);
    
    void createMiddleLeftResizeHandle(const QRectF &rect);
    
    void createTopLeftResizeHandle(const QRectF &rect);
    
    void createTopMiddleResizeHandle(const QRectF &rect);
    
    void createTopRightResizeHandle(const QRectF &rect);
    
    void createMiddleRightResizeHandle(const QRectF &rect);
    
    void createBottomRightResizeHandle(const QRectF &rect);
    
    void createBottomMiddleResizeHandle(const QRectF &rect);
    
    void createBottomLeftResizeHandle(const QRectF &rect);

signals:
    void rectIsUpdated(const QRectF&);
    
protected slots:
    
    virtual void updateRectUsingOffsetRect(const QRectF& offsetRect);
    
    virtual void updateHandleItemsPositions();
    
protected:
    QGraphicsRectItem* _resizeRect;
    QList<QGraphicsItem*> _resizeHandleItems;
};

class MyResizeHandledGraphicsItem: public MyResizeHandledGraphicsItemBase {
    
public:
    
    MyResizeHandledGraphicsItem(const QRectF &rect, qreal zValue, QGraphicsItem *parent = nullptr);
};

class MyRoundedRectangleResizeHandledGraphicsItem: public MyResizeHandledGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyRoundedRectangleResizeHandledGraphicsItem(const QRectF &rect, qreal radiusX, qreal radiusY, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createResizeHandledItems(const QRectF &rect, qreal radiusX, qreal radiusY);
    
    void createBorderRadiusXControlHandle(const QRectF &rect, qreal radius);
    
    void createBorderRadiusYControlHandle(const QRectF &rect, qreal radius);
    
signals:
    
    void borderRadiiAreUpdated(const qreal&, const qreal&);
    
private slots:
    
    void updateRectUsingRadiusXHandlePosition(const qreal& radiusX);
    
    void updateRectUsingRadiusYHandlePosition(const qreal& radiusY);
    
    void updateRectUsingOffsetRect(const QRectF& offsetRect) override;
        
    void updateHandleItemsPositions() override;
    
protected:
    qreal _radiusX;
    QGraphicsItem* _borderRadiusXHandleItem;
    qreal _radiusY;
    QGraphicsItem* _borderRadiusYHandleItem;
};

class MyHandleGraphicsItemBase: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    
public:
    
    MyHandleGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QPointF& center);
    
    virtual void updatePosition(const QRectF &rect);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    QRectF _rect;
    qreal _handleRadius;
    QPointF _pressedPosition;
};

class MyResizeHandleGraphicsItemBase: public MyHandleGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyResizeHandleGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    const bool isUpwardMovePossbile(const QPointF& eventScenePosition);

    const bool isDownwardMovePossbile(const QPointF& eventScenePosition);

    const bool isRightwardMovePossbile(const QPointF& eventScenePosition);

    const bool isLeftwardMovePossbile(const QPointF& eventScenePosition);
    
signals:
    void offsetRectUpdated(const QRectF&);
};

class MyMiddleLeftResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyMiddleLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopLeftResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyTopLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopMiddleResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyTopMiddleResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopRightResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyTopRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyMiddleRightResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyMiddleRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomRightResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyBottomRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomMiddleResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyBottomMiddleResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomLeftResizeHandleGraphicsItem: public MyResizeHandleGraphicsItemBase {
public:
    
    MyBottomLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBorderRadiusControlHandleGraphicsItemBase: public MyHandleGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyBorderRadiusControlHandleGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    virtual void updatePosition(const QRectF &rect, qreal radius);
    
signals:
    void handlePositionUpdated(const qreal&);
    
protected:
    qreal _radius;
};

class MyBorderRadiusXControlHandleGraphicsItem: public MyBorderRadiusControlHandleGraphicsItemBase {
public:
    
    MyBorderRadiusXControlHandleGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect, qreal radius) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBorderRadiusYControlHandleGraphicsItem: public MyBorderRadiusControlHandleGraphicsItemBase {
public:
    
    MyBorderRadiusYControlHandleGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr);
    
    void updatePosition(const QRectF &rect, qreal radius) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif
