#include "negui_node_graphics_item_builder.h"
#include "negui_node_graphics_item.h"

MyElementGraphicsItemBase* createClassicNodeSceneGraphicsItem(const QPointF &position) {
    return new MyClassicNodeSceneGraphicsItem(position);
}

MyElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position) {
    return new MyCentroidNodeSceneGraphicsItem(position);
}

MyElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position) {
    return new MyNodeIconGraphicsItem(position);
}