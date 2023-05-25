#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item.h"

MyNetworkElementGraphicsItemBase* createArrowHeadSceneGraphicsItem() {
    return new MyArrowHeadSceneGraphicsItem();
}

MyNetworkElementGraphicsItemBase* createArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation) {
    return new MyArrowHeadIconGraphicsItem(position, rotation);
}
