#ifndef __NEGUI_ELEMENT_ICON_BUILDER_H
#define __NEGUI_ELEMENT_ICON_BUILDER_H

#include "negui_element_style_base.h"
#include <QGraphicsItem>

class MyElementIconBuilderBase : public QObject {
    Q_OBJECT
    
public:
    MyElementIconBuilderBase(MyElementStyleBase* style);
    
    void build();
    
    virtual QList<QGraphicsItem*> getIconGraphicsItems() = 0;
    
    const QRectF getIconExtents(QList<QGraphicsItem*> items);
    
    const QIcon icon();
    
    MyElementStyleBase* style();
    
protected:
    MyElementStyleBase* _style;
    QIcon _icon;
};

class MyNodeIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyNodeIconBuilder(MyElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyEdgeIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyEdgeIconBuilder(MyElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyArrowHeadIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyArrowHeadIconBuilder(MyElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyTemplateIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyTemplateIconBuilder(MyElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
    QList<QGraphicsItem*> getIntermediaryNodeIconGraphicsItems();
    
    QList<QGraphicsItem*> getSourceEdgeIconGraphicsItems();
    
    QList<QGraphicsItem*> getTargetEdgeIconGraphicsItems();
    
    qreal getIconWidth();
    
    qreal getIntermediaryShapeWidth();
};

QList<QGraphicsItem*> getNodeIconGraphicsItems(MyElementStyleBase* style, const QPointF& position);

QList<QGraphicsItem*> getEdgeIconGraphicsItems(MyElementStyleBase* style, const QPointF& startPoint, const QPointF& endPoint);

QList<QGraphicsItem*> getArrowHeadIconGraphicsItems(MyElementStyleBase* style, const QPointF& position, const qreal& rotation);

qreal getVerticalPadding(MyElementStyleBase* style, QList<MyElementStyleBase*> styles);

#endif