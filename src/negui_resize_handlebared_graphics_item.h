#ifndef __NEGUI_RESIZE_HANDLEBARED_GRAPHICS_ITEM_H
#define __NEGUI_RESIZE_HANDLEBARED_GRAPHICS_ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyResizeHandlebaredGraphicsItemBase: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyResizeHandlebaredGraphicsItemBase(const QRectF &rect, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createResizeRectItem(const QRectF &rect);
    
    virtual void createResizeHandlbarItems(const QRectF &rect);
    
    void createMiddleLeftResizeHandlebar(const QRectF &rect);
    
    void createTopLeftResizeHandlebar(const QRectF &rect);
    
    void createTopMiddleResizeHandlebar(const QRectF &rect);
    
    void createTopRightResizeHandlebar(const QRectF &rect);
    
    void createMiddleRightResizeHandlebar(const QRectF &rect);
    
    void createBottomRightResizeHandlebar(const QRectF &rect);
    
    void createBottomMiddleResizeHandlebar(const QRectF &rect);
    
    void createBottomLeftResizeHandlebar(const QRectF &rect);

signals:
    void rectIsUpdated(const QRectF&);
    
protected slots:
    
    virtual void updateRectUsingOffsetRect(const QRectF& offsetRect);
    
    virtual void updateHandlebarItemsPositions();
    
protected:
    QGraphicsRectItem* _resizeRect;
    QList<QGraphicsItem*> _resizeHandlebarItems;
};

class MyResizeHandlebaredGraphicsItem: public MyResizeHandlebaredGraphicsItemBase {
    
public:
    
    MyResizeHandlebaredGraphicsItem(const QRectF &rect, qreal zValue, QGraphicsItem *parent = nullptr);
};

class MyRoundedRectangleResizeHandlebaredGraphicsItem: public MyResizeHandlebaredGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyRoundedRectangleResizeHandlebaredGraphicsItem(const QRectF &rect, qreal radiusX, qreal radiusY, qreal zValue, QGraphicsItem *parent = nullptr);
    
    void createResizeHandlbarItems(const QRectF &rect, qreal radiusX, qreal radiusY);
    
    void createCurvatureRadiusXControlHandlebar(const QRectF &rect, qreal radius);
    
    void createCurvatureRadiusYControlHandlebar(const QRectF &rect, qreal radius);
    
signals:
    
    void curvatureRadiiAreUpdated(const qreal&, const qreal&);
    
private slots:
    
    void updateRectUsingRadiusXHandlebarPosition(const qreal& radiusX);
    
    void updateRectUsingRadiusYHandlebarPosition(const qreal& radiusY);
    
    void updateRectUsingOffsetRect(const QRectF& offsetRect) override;
        
    void updateHandlebarItemsPositions() override;
    
protected:
    qreal _radiusX;
    QGraphicsItem* _curvatureRadiusXHandlebarItem;
    qreal _radiusY;
    QGraphicsItem* _curvatureRadiusYHandlebarItem;
};

class MyHandlebarGraphicsItemBase: public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    
public:
    
    MyHandlebarGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QPointF& center);
    
    virtual void updatePostion(const QRectF &rect);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    QRectF _rect;
    qreal _handlebarRadius;
    QPointF _pressedPosition;
};

class MyResizeHandlebarGraphicsItemBase: public MyHandlebarGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyResizeHandlebarGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    const bool isUpwardMovePossbile(const QPointF& eventScenePosition);

    const bool isDownwardMovePossbile(const QPointF& eventScenePosition);

    const bool isRightwardMovePossbile(const QPointF& eventScenePosition);

    const bool isLeftwardMovePossbile(const QPointF& eventScenePosition);
    
signals:
    void offsetRectUpdated(const QRectF&);
};

class MyMiddleLeftResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyMiddleLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopLeftResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyTopLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopMiddleResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyTopMiddleResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyTopRightResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyTopRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyMiddleRightResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyMiddleRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomRightResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyBottomRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomMiddleResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyBottomMiddleResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyBottomLeftResizeHandlebarGraphicsItem: public MyResizeHandlebarGraphicsItemBase {
public:
    
    MyBottomLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyCurvatureRadiusControlHandlebarGraphicsItemBase: public MyHandlebarGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyCurvatureRadiusControlHandlebarGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    virtual void updatePostion(const QRectF &rect, qreal radius);
    
signals:
    void handlebarPositionUpdated(const qreal&);
    
protected:
    qreal _radius;
};

class MyCurvatureRadiusXControlHandlebarGraphicsItem: public MyCurvatureRadiusControlHandlebarGraphicsItemBase {
public:
    
    MyCurvatureRadiusXControlHandlebarGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect, qreal radius) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

class MyCurvatureRadiusYControlHandlebarGraphicsItem: public MyCurvatureRadiusControlHandlebarGraphicsItemBase {
public:
    
    MyCurvatureRadiusYControlHandlebarGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent = nullptr);
    
    void updatePostion(const QRectF &rect, qreal radius) override;
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif
