#include "negui_shape_graphics_item_builder.h"
#include "negui_ellipse_graphics_item.h"
#include "negui_rectangle_graphics_item.h"
#include "negui_polygon_graphics_item.h"
#include "negui_line_graphics_item.h"
#include "negui_text_graphics_item.h"
#include "negui_centroid_graphics_item.h"

MyShapeGraphicsItemBase* createEllipseShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyEllipseGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createRectangleShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyRectangleGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createPolygonShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyPolygonGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createClassicLineShape(const QLineF& line, QGraphicsItem *parent) {
    return new MyClassicLineGraphicsItem(line, parent);
}

MyShapeGraphicsItemBase* createConnectedToStartCentroidShapeLineShape(const QLineF& line, QGraphicsItem *parent) {
    return new MyConnectedToStartCentroidShapeLineGraphicsItem(line, parent);
}

MyShapeGraphicsItemBase* createConnectedToEndCentroidShapeLineShape(const QLineF& line, QGraphicsItem *parent) {
    return new MyConnectedToEndCentroidShapeLineGraphicsItem(line, parent);
}

MyShapeGraphicsItemBase* createSimpleTextShape(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent) {
    return new MySimpleTextGraphicsItem(x, y, elementName, parent);
}

MyShapeGraphicsItemBase* createWithPlainTextTextShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyWithPlainTextTextGraphicsItem(x, y, parent);
}

MyShapeGraphicsItemBase* createCentroidShape(qreal x, qreal y, QGraphicsItem *parent) {
    return new MyCentroidGraphicsItem(x, y, parent);
}
