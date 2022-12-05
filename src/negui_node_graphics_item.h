#ifndef __NEGUI_NODE_GRAPHICS_ITEM_H
#define __NEGUI_NODE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"
#include <QKeyEvent>

class MyNodeGraphicsItemBase : public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
    
    QList<QGraphicsItem*> createResizeHandleBaredGraphicsItems() override;
    
    void enableNormalMode() override;

    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;

    void enableAddEdgeMode() override;

    void enableSelectEdgeMode() override;

    void enableRemoveMode() override;
    
protected:
    QPointF _originalPosition;
};

class MyNodeSceneGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);
    
    void addResizeHandleBaredGraphicsItems() override;
    
    void clearResizeHandleBaredGraphicsItems() override;
    
    void moveBy(qreal dx, qreal dy);
    
    void deparent();
    
    void moveChildItems(const QPointF& movedDistance);
    
    void movePosition(const QPointF& movedDistance);
    
    QRectF getExtents() const;
    
public slots:
    void updateExtents(const QRectF& extents);
    
signals:
    
    void extentsModified();
    
    void askForDeparent();
    
    void askForReparent();
    
protected:
    
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    void keyPressEvent(QKeyEvent *event) override;
    
    void keyReleaseEvent(QKeyEvent *event) override;
    
    void focusOutEvent(QFocusEvent *event) override;
    
    bool _reparent;
    QPointF _mousePressedPosition;
};

class MyNodeIconGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeIconGraphicsItem(QGraphicsItem *parent = nullptr);
};

#endif
