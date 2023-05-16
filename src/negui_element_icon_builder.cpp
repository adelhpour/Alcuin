#include "negui_element_icon_builder.h"
#include "negui_node_graphics_item_builder.h"
#include "negui_edge_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_edge_style.h"
#include "negui_template_style.h"

#include <QtMath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


// MyElementIconBuilderBase

MyElementIconBuilderBase::MyElementIconBuilderBase(MyElementStyleBase* style) {
    _style = style;
}

void MyElementIconBuilderBase::build() {
    QList<QGraphicsItem*> items = getIconGraphicsItems();
    QRectF extents = getIconExtents(items);
    QPixmap pixMap(extents.width(), extents.height());
    pixMap.fill(Qt::transparent);
    QPainter painter(&pixMap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    painter.translate(-1 * extents.x(),-1 * extents.y());
    QStyleOptionGraphicsItem opt;
    for (QGraphicsItem* item : qAsConst(items))
        item->paint(&painter, &opt);
    
    _icon = QIcon(pixMap);
}

const QRectF MyElementIconBuilderBase::getIconExtents(QList<QGraphicsItem*> items) {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    
    for (QGraphicsItem* item : qAsConst(items)) {
        if (item->boundingRect().x() < extents.x())
            extents.setX(item->boundingRect().x());
        if (item->boundingRect().y() < extents.y())
            extents.setY(item->boundingRect().y());
        if (extents.x() + extents.width() < item->boundingRect().x() + item->boundingRect().width())
            extents.setWidth(extents.width() + (item->boundingRect().x() + item->boundingRect().width() - extents.x() - extents.width()));
        if (extents.y() + extents.height() < item->boundingRect().y() + item->boundingRect().height())
            extents.setHeight(extents.height() + (item->boundingRect().y() + item->boundingRect().height() - extents.y() - extents.height()));
    }
    
    return extents;
}

const QIcon MyElementIconBuilderBase::icon() {
    return _icon;
}

MyElementStyleBase* MyElementIconBuilderBase::style() {
    return _style;
}

// MyNodeIconBuilder

MyNodeIconBuilder::MyNodeIconBuilder(MyElementStyleBase* style) : MyElementIconBuilderBase(style) {
    
}

QList<QGraphicsItem*> MyNodeIconBuilder::getIconGraphicsItems() {
    return getNodeIconGraphicsItems(style(), QPointF(0.0, 0.0));
}

// MyEdgeIconBuilder

MyEdgeIconBuilder::MyEdgeIconBuilder(MyElementStyleBase* style) : MyElementIconBuilderBase(style) {
    
}

QList<QGraphicsItem*> MyEdgeIconBuilder::getIconGraphicsItems() {
    qreal iconWidth = 60.0;
    qreal verticalPadding = 0.0;
    qreal paddingRotation = qRadiansToDegrees(qAtan2(verticalPadding, (0.5 * iconWidth)));
    QList<QGraphicsItem*> items = getEdgeIconGraphicsItems(style(), QPointF(0.0, -verticalPadding), QPointF(iconWidth, verticalPadding));
    
    items += getArrowHeadIconGraphicsItems(((MyEdgeStyleBase*)style())->arrowHeadStyle(), QPointF(iconWidth, verticalPadding), paddingRotation);
    
    return items;
}

// MyArrowHeadIconBuilder

MyArrowHeadIconBuilder::MyArrowHeadIconBuilder(MyElementStyleBase* style) : MyElementIconBuilderBase(style) {
    
}

QList<QGraphicsItem*> MyArrowHeadIconBuilder::getIconGraphicsItems() {
    return getArrowHeadIconGraphicsItems(style(), QPointF(0.0, 0.0), 0.0);
}

// MyTemplateIconBuilder

MyTemplateIconBuilder::MyTemplateIconBuilder(MyElementStyleBase* style) : MyElementIconBuilderBase(style) {
    
}

QList<QGraphicsItem*> MyTemplateIconBuilder::getIconGraphicsItems() {
    return getIntermediaryNodeIconGraphicsItems() + getSourceEdgeIconGraphicsItems() + getTargetEdgeIconGraphicsItems();
}

QList<QGraphicsItem*> MyTemplateIconBuilder::getIntermediaryNodeIconGraphicsItems() {
    QList<QGraphicsItem*> items;
    if (((MyTemplateStyle*)style())->intermediaryNodeStyle())
        items+= getNodeIconGraphicsItems(((MyTemplateStyle*)style())->intermediaryNodeStyle(), QPointF(0.5 * getIconWidth(), 0.0));
    
    return items;
}

QList<QGraphicsItem*> MyTemplateIconBuilder::getSourceEdgeIconGraphicsItems() {
    QList<QGraphicsItem*> items;
    qreal verticalPadding = 0.0;
    qreal paddingRotation = 0.0;
    for (MyElementStyleBase* sourceEdgeStyle : ((MyTemplateStyle*)style())->sourceEdgeStyles()) {
        verticalPadding = getVerticalPadding(sourceEdgeStyle, ((MyTemplateStyle*)style())->sourceEdgeStyles());
        paddingRotation = qRadiansToDegrees(qAtan2(verticalPadding, (0.5 * getIconWidth())));
        items += getEdgeIconGraphicsItems(sourceEdgeStyle, QPointF(0.0, verticalPadding), QPointF(0.5 * (getIconWidth() - getIntermediaryShapeWidth()), 0.0));
        items += getArrowHeadIconGraphicsItems(((MyEdgeStyleBase*)sourceEdgeStyle)->arrowHeadStyle(), QPointF(0.5 * (getIconWidth() - getIntermediaryShapeWidth()), 0.0), -paddingRotation);
    }
    
    return items;
}

QList<QGraphicsItem*> MyTemplateIconBuilder::getTargetEdgeIconGraphicsItems() {
    QList<QGraphicsItem*> items;
    qreal verticalPadding = 0.0;
    qreal paddingRotation = 0.0;
    for (MyElementStyleBase* targetEdgeStyle : ((MyTemplateStyle*)style())->targetEdgeStyles()) {
        verticalPadding = getVerticalPadding(targetEdgeStyle, ((MyTemplateStyle*)style())->targetEdgeStyles());
        paddingRotation = qRadiansToDegrees(qAtan2(verticalPadding, (0.5 * getIconWidth())));
        items += getEdgeIconGraphicsItems(targetEdgeStyle, QPointF(0.5 * (getIconWidth() + getIntermediaryShapeWidth()), 0.0), QPointF(getIconWidth(), verticalPadding));
        items += getArrowHeadIconGraphicsItems(((MyEdgeStyleBase*)targetEdgeStyle)->arrowHeadStyle(), QPointF(getIconWidth(), verticalPadding), paddingRotation);
    }
    
    return items;
}

qreal MyTemplateIconBuilder::getIconWidth() {
    qreal iconWidth = 100.0;
    if (((MyTemplateStyle*)style())->intermediaryNodeStyle())
        iconWidth = qMin(qMax(12 * getIntermediaryShapeWidth(), iconWidth), 2 * iconWidth);
    
    return iconWidth;
}

qreal MyTemplateIconBuilder::getIntermediaryShapeWidth() {
    qreal intermediaryShapeWidth = 0.0;
    if (((MyTemplateStyle*)style())->intermediaryNodeStyle())
        intermediaryShapeWidth = ((MyTemplateStyle*)style())->intermediaryNodeStyle()->getShapesExtents().width();
    
    return intermediaryShapeWidth;
}

QList<QGraphicsItem*> getNodeIconGraphicsItems(MyElementStyleBase* style, const QPointF& position) {
    QList<QGraphicsItem*> items;
    QGraphicsItem* nodeIconItem = createNodeIconGraphicsItem(position);
    ((MyElementGraphicsItemBase*)nodeIconItem)->addShapeItems(style->shapeStyles());
    items.push_back(nodeIconItem);
    
    return items;
}

QList<QGraphicsItem*> getEdgeIconGraphicsItems(MyElementStyleBase* style, const QPointF& startPoint, const QPointF& endPoint) {
    QList<QGraphicsItem*> items;
    QGraphicsItem* edgeIconItem = createEdgeIconGraphicsItem(startPoint, endPoint);
    ((MyElementGraphicsItemBase*)edgeIconItem)->addShapeItems(style->shapeStyles());
    items.push_back(edgeIconItem);
    
    return items;
}

QList<QGraphicsItem*> getArrowHeadIconGraphicsItems(MyElementStyleBase* style, const QPointF& position, const qreal& rotation) {
    QList<QGraphicsItem*> items;
    QGraphicsItem* arrowHeadIconItem = createArrowHeadIconGraphicsItem(position, rotation);
    ((MyElementGraphicsItemBase*)arrowHeadIconItem)->addShapeItems(style->shapeStyles());
    items.push_back(arrowHeadIconItem);
    
    return items;
}

qreal getVerticalPadding(MyElementStyleBase* style, QList<MyElementStyleBase*> styles) {
    if (styles.indexOf(style) != -1)
        return 9.0 * (2 * styles.indexOf(style) + 1 - styles.size());
    
    return 0.0;
}

