#ifndef __NEGUI_EDGE_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_EDGE_GRAPHICS_ITEM_BUILDER_H

#include "negui_network_element_graphics_item_base.h"

MyNetworkElementGraphicsItemBase* createEdgeSceneGraphicsItem();

MyNetworkElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint);

#endif
