#ifndef __NEGUI_NODE_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_NODE_GRAPHICS_ITEM_BUILDER_H

#include "negui_network_element_graphics_item_base.h"

MyNetworkElementGraphicsItemBase* createClassicNodeSceneGraphicsItem(const QPointF &position);

MyNetworkElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position);

MyNetworkElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position);

#endif
