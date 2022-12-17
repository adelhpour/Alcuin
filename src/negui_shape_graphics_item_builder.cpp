#include "negui_shape_graphics_item_builder.h"
#include "negui_ellipse_graphics_item.h"
#include "negui_rect_graphics_item.h"
#include "negui_polygon_graphics_item.h"
#include "negui_line_graphics_item.h"
#include "negui_text_graphics_item.h"

MyShapeGraphicsItemBase* createEllipseShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyEllipseGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createRectShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyRectGraphicsItem(x, y, parent);
}


MyShapeGraphicsItemBase* createPolygonShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyPolygonGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createLineShape(const QLineF& line, QGraphicsItem *parent) {
    return new MyLineGraphicsItem(line, parent);
}

MyShapeGraphicsItemBase* createTextShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyTextGraphicsItem(x, y, parent);
}
