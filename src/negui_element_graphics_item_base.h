#ifndef __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H
#define __NEGUI_ELEMENT_GRAPHICS_ITEM_BASE_H

#include "negui_base.h"
#include "negui_shape_style_base.h"
#include "negui_shape_graphics_item_base.h"
#include "negui_feature_menu.h"

class MyElementGraphicsItemBase : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
    
public:
    
    MyElementGraphicsItemBase(QGraphicsItem *parent = nullptr);
    
    void addShapeItems(QList<MyShapeStyleBase*> shapeStyles, const qint32& zValue = 0);
    
    void addShapeItem(MyShapeStyleBase* style);
    
    QList<MyShapeStyleBase*> getShapeStyles();
    
    virtual MyShapeGraphicsItemBase* defaultEllipseItem() = 0;
    
    virtual MyShapeGraphicsItemBase* defaultRectItem() = 0;
    
    virtual MyShapeGraphicsItemBase* defaultPolygonItem() = 0;
    
    virtual MyShapeGraphicsItemBase* defaultLineItem() = 0;
    
    virtual MyShapeGraphicsItemBase* defaultBezierItem() = 0;
    
    virtual MyShapeGraphicsItemBase* defaultTextItem() = 0;
    
    void setSelectedWithStroke(const bool& selected);
    
    void setSelectedWithFill(const bool& selected);
    
    const bool& isSelected() {return _isSelected; };
    
    void setCursor(const QCursor &cursor);
    
    void clear();
    
signals:
    
    void mouseLeftButtonIsPressed();
    QWidget* askForElementFeatureMenu();
    void askForSetShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    
    QList<MyShapeStyleBase*> _shapeStyles;
    bool _isChosen;
    bool _isSelected;
};

#endif
