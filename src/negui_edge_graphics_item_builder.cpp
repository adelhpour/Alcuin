#include "negui_edge_graphics_item_builder.h"
#include "negui_edge_graphics_item.h"

MyElementGraphicsItemBase* createEdgeSceneGraphicsItem() {
    return new MyEdgeSceneGraphicsItem();
}

MyElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint) {
    return new MyEdgeIconGraphicsItem(startPoint, endPoint);
}
