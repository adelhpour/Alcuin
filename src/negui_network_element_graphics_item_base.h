#ifndef __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H
#define __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H

#include "negui_scene_mode_element_base.h"
#include "negui_shape_graphics_item_base.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

class MyNetworkElementGraphicsItemBase : public QObject, public MySceneModeElementBase, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyNetworkElementGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void update(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue);
    
    void addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue = 0);
    
    void addShapeItem(MyShapeStyleBase* style);
    
    virtual MyShapeGraphicsItemBase* createShapeGraphicsItem(MyShapeStyleBase* style) = 0;

    virtual QMenu* createContextMenu() = 0;
    
    QList<MyShapeStyleBase*> getShapeStyles();
    
    QList<QGraphicsItem*> createFocusedGraphicsItems();
    
    void addFocusedGraphicsItems();
    
    virtual void clearFocusedGraphicsItems();
    
    const QRectF getExtents() const;
    
    void setSelectedWithStroke(const bool& selected);
    
    void setSelectedWithFill(const bool& selected);
    
    void setFocused(const bool& isFocused);
    
    void setCursor(const QCursor &cursor);
    
    void clear();
    
    void setZValue(qreal z);
    
    void enableNormalMode() override;
    
signals:
    
    void mouseLeftButtonIsPressed();
    void mouseLeftButtonIsDoubleClicked();
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);
    void askForCreateChangeStageCommand();
    void askForClearFocusedGraphicsItems();
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    
    QPointF _originalPosition;
    QList<MyShapeStyleBase*> _shapeStyles;
    QList<QGraphicsItem*> _focusedGraphicsItems;
    bool _isChosen;
};

#endif
