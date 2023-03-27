#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item.h"

MyElementGraphicsItemBase* createArrowHeadSceneGraphicsItem() {
    return new MyArrowHeadSceneGraphicsItem();
}

MyElementGraphicsItemBase* createArrowHeadIconGraphicsItem(const QPointF& position, const qreal& rotation) {
    return new MyArrowHeadIconGraphicsItem(position, rotation);
}
