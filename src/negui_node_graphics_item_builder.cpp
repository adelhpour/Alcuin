#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"

MyNetworkElementGraphicsItemBase* createClassicNodeSceneGraphicsItem(const QPointF &position) {
    return new MyClassicNodeSceneGraphicsItem(position);
}

MyNetworkElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position) {
    return new MyCentroidNodeSceneGraphicsItem(position);
}

MyNetworkElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position) {
    return new MyNodeIconGraphicsItem(position);
}