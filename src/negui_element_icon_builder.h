#ifndef __NEGUI_ELEMENT_ICON_BUILDER_H
#define __NEGUI_ELEMENT_ICON_BUILDER_H

#include "negui_network_element_style_base.h"
#include <QGraphicsItem>

class MyElementIconBuilderBase : public QObject {
    Q_OBJECT
    
public:
    MyElementIconBuilderBase(MyNetworkElementStyleBase* style);
    
    void build();
    
    virtual QList<QGraphicsItem*> getIconGraphicsItems() = 0;
    
    const QRectF getIconExtents(QList<QGraphicsItem*> items);
    
    const QIcon icon();
    
    MyNetworkElementStyleBase* style();
    
protected:
    MyNetworkElementStyleBase* _style;
    QIcon _icon;
};

class MyNodeIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyNodeIconBuilder(MyNetworkElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyEdgeIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyEdgeIconBuilder(MyNetworkElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyArrowHeadIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyArrowHeadIconBuilder(MyNetworkElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
};

class MyTemplateIconBuilder : public MyElementIconBuilderBase {
    
public:
    MyTemplateIconBuilder(MyNetworkElementStyleBase* style);
    
    QList<QGraphicsItem*> getIconGraphicsItems() override;
    
    QList<QGraphicsItem*> getIntermediaryNodeIconGraphicsItems();
    
    QList<QGraphicsItem*> getSourceEdgeIconGraphicsItems();
    
    QList<QGraphicsItem*> getTargetEdgeIconGraphicsItems();
    
    qreal getIconWidth();
    
    qreal getIntermediaryShapeWidth();
};

QList<QGraphicsItem*> getNodeIconGraphicsItems(MyNetworkElementStyleBase* style, const QPointF& position);

QList<QGraphicsItem*> getEdgeIconGraphicsItems(MyNetworkElementStyleBase* style, const QPointF& startPoint, const QPointF& endPoint);

QList<QGraphicsItem*> getArrowHeadIconGraphicsItems(MyNetworkElementStyleBase* style, const QPointF& position, const qreal& rotation);

qreal getVerticalPadding(MyNetworkElementStyleBase* style, QList<MyNetworkElementStyleBase*> styles);

#endif
