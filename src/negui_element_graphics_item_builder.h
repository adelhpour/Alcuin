#ifndef __NEGUI_ELEMENT_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_ELEMENT_GRAPHICS_ITEM_BUILDER_H

#include "negui_element_graphics_item_base.h"

MyElementGraphicsItemBase* createNodeSceneGraphicsItem(const QPointF &position);

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem();

MyElementGraphicsItemBase* createArrowHeadSceneGraphicsItem();

MyElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position);

MyElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint);

MyElementGraphicsItemBase* createArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation);

#endif
