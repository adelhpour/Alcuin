#ifndef __NEGUI_NODE_GRAPHICS_ITEM_H
#define __NEGUI_NODE_GRAPHICS_ITEM_H

#include "negui_element_graphics_item_base.h"
#include <QKeyEvent>

class MyNodeGraphicsItemBase : public MyElementGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) override;
    
protected:
    QPointF _initialPosition;
};

class MyNodeSceneGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeSceneGraphicsItem(const QPointF &position, QGraphicsItem *parent = nullptr);
    
    void moveBy(qreal dx, qreal dy);
    
    void updateExtents(const QRectF& extents);
    
    QRectF getExtents() const;
    
signals:
    void positionChanged(const QPointF& position);
    
    void extentsModified();
    
    void askForDeparent();
    
    void askForReparent();
    
protected:
    
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
    void keyPressEvent(QKeyEvent *event) override;
    
    void keyReleaseEvent(QKeyEvent *event) override;
    
    bool _reparent;
};

class MyNodeIconGraphicsItem : public MyNodeGraphicsItemBase {
    Q_OBJECT
    
public:
    
    MyNodeIconGraphicsItem(QGraphicsItem *parent = nullptr);
};

#endif
