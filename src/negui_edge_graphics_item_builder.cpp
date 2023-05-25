#include "negui_edge_graphics_item_builder.h"
#include "negui_edge_graphics_item.h"

MyNetworkElementGraphicsItemBase* createEdgeSceneGraphicsItem() {
    return new MyEdgeSceneGraphicsItem();
}

MyNetworkElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint) {
    return new MyEdgeIconGraphicsItem(startPoint, endPoint);
}
