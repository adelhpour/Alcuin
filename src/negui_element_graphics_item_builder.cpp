#include "negui_element_graphics_item_builder.h"
#include "negui_node_graphics_item.h"
#include "negui_edge_graphics_item.h"
#include "negui_arrow_head_graphics_item.h"

MyElementGraphicsItemBase* createClassicNodeSceneGraphicsItem(const QPointF &position) {
    return new MyClassicNodeSceneGraphicsItem(position);
}

MyElementGraphicsItemBase* createCentroidNodeSceneGraphicsItem(const QPointF &position) {
    return new MyCentroidNodeSceneGraphicsItem(position);
}

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem() {
    return new MyEdgeSceneGraphicsItem();
}

MyElementGraphicsItemBase* createArrowHeadSceneGraphicsItem() {
    return new MyArrowHeadSceneGraphicsItem();
}

MyElementGraphicsItemBase* createNodeIconGraphicsItem(const QPointF& position) {
    return new MyNodeIconGraphicsItem(position);
}

MyElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint) {
    return new MyEdgeIconGraphicsItem(startPoint, endPoint);
}

MyElementGraphicsItemBase* createArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation) {
    return new MyArrowHeadIconGraphicsItem(position, rotation);
}
