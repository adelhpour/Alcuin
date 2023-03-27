#ifndef __NEGUI_NODE_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_NODE_GRAPHICS_ITEM_BUILDER_H

#include "negui_element_graphics_item_base.h"

MyElementGraphicsItemBase* createClassicNodeSceneGraphicsItem(const QPointF &position);

MyElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position);

MyElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position);

#endif
