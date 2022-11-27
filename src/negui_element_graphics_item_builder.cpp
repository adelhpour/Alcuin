#include "negui_element_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include "negui_edge_graphics_item.h"
#include "negui_arrow_head_graphics_item.h"

MyElementGraphicsItemBase* createNodeSceneGraphicsItem(const QPointF &position) {
    return new MyNodeSceneGraphicsItem(position);
}

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem() {
    return new MyEdgeSceneGraphicsItem();
}

MyElementGraphicsItemBase* createArrowHeadSceneGraphicsItem() {
    return new MyArrowHeadSceneGraphicsItem();
}

MyElementGraphicsItemBase* createNodeIconGraphicsItem() {
    return new MyNodeIconGraphicsItem();
}

MyElementGraphicsItemBase* createEdgeIconGraphicsItem() {
    return new MyEdgeIconGraphicsItem();
}

MyElementGraphicsItemBase* createArrowHeadIconGraphicsItem() {
    return new MyArrowHeadIconGraphicsItem();
}
