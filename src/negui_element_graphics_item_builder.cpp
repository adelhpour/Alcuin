#include "negui_element_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include "negui_edge_graphics_item.h"
#include "negui_arrow_head_graphics_item.h"

MyElementGraphicsItemBase* createNodeSceneGraphicsItem(const QPointF &position) {
    MyElementGraphicsItemBase* item = new MyNodeSceneGraphicsItem(position);
    item->setZValue(2);
    return item;
}

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem() {
    MyElementGraphicsItemBase* item = new MyEdgeSceneGraphicsItem();
    item->setZValue(0);
    return item;
}

MyElementGraphicsItemBase* createArrowHeadSceneGraphicsItem() {
    MyElementGraphicsItemBase* item = new MyArrowHeadSceneGraphicsItem();
    item->setZValue(1);
    return item;
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
