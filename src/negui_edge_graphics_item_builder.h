#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_BUILDER_H

#include "negui_element_graphics_item_base.h"

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem();

MyElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint);

#endif
