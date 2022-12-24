#ifndef __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H
#define __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H

#include "negui_shape_graphics_item_base.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class MyElementGraphicsItemBase : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyElementGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue);
    
    void addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue = 0);
    
    void addShapeItem(MyShapeStyleBase* style);
    
    virtual MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) = 0;
    
    QList<MyShapeStyleBase*> getShapeStyles();
    
    QList<QGraphicsItem*> createResizeHandleBaredGraphicsItems();
    
    void addResizeHandleBaredGraphicsItems();
    
    virtual void clearResizeHandleBaredGraphicsItems();
    
    const QRectF getExtents() const;
    
    void setSelectedWithStroke(const bool& selected);
    
    void setSelectedWithFill(const bool& selected);
    
    void setFocused(const bool& isFocused);
    
    void setCursor(const QCursor &cursor);
    
    void clear();
    
    void setZValue(qreal z);
    
    virtual void enableNormalMode() = 0;
    
    virtual void enableAddNodeMode() = 0;
    
    virtual void enableSelectNodeMode() = 0;
    
    virtual void enableAddEdgeMode() = 0;
    
    virtual void enableSelectEdgeMode() = 0;
    
    virtual void enableRemoveMode() = 0;
    
signals:
    
    void mouseLeftButtonIsPressed();
    void mouseLeftButtonIsDoubleClicked();
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);
    void askForCreateChangeStageCommand();
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    
    QPointF _originalPosition;
    QList<MyShapeStyleBase*> _shapeStyles;
    QList<QGraphicsItem*> _resizeHandlebaredGraphicsItems;
    bool _isChosen;
};

#endif
