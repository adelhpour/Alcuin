#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"

MyNetworkElementGraphicsItemBase* createComplexNodeSceneGraphicsItem(const QPointF &position) {
    return new MyComplexNodeSceneGraphicsItem(position);
}

MyNetworkElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position) {
    return new MyCentroidNodeSceneGraphicsItem(position);
}

MyNetworkElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position) {
    return new MyNodeIconGraphicsItem(position);
}