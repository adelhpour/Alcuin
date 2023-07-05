#include "negui_edge_graphics_item_builder.h"
#include "negui_edge_graphics_item.h"

MyNetworkElementGraphicsItemBase* createClassicEdgeSceneGraphicsItem() {
    return new MyClassicEdgeSceneGraphicsItem();
}

MyNetworkElementGraphicsItemBase* createConnectedToSourceCentroidNodeEdgeSceneGraphicsItem() {
    return new MyConnectedToSourceCentroidNodeEdgeSceneGraphicsItem();
}

MyNetworkElementGraphicsItemBase* createConnectedToTargetCentroidNodeEdgeSceneGraphicsItem() {
    return new MyConnectedToTargetCentroidNodeEdgeSceneGraphicsItem();
}

MyNetworkElementGraphicsItemBase* createEdgeIconGraphicsItem(const QPointF& startPoint, const QPointF& endPoint) {
    return new MyEdgeIconGraphicsItem(startPoint, endPoint);
}
