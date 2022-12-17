#include "negui_shape_style_builder.h"
#include "negui_ellipse_style.h"
#include "negui_rect_style.h"
#include "negui_polygon_style.h"
#include "negui_line_style.h"
#include "negui_bezier_style.h"
#include "negui_text_style.h"

MyShapeStyleBase* createNodeEllipseStyle(const QString& shape) {
    return new MyNodeEllipseStyle(shape);
}

MyShapeStyleBase* createNodeRectStyle(const QString& shape) {
    return new MyNodeRectStyle(shape);
}

MyShapeStyleBase* createNodePolygonStyle(const QString& shape) {
    return new MyNodePolygonStyle(shape);
}

MyShapeStyleBase* createNodeDefaultPolygonStyle(const QString& shape) {
    MyShapeStyleBase* shapeStyle = new MyNodePolygonStyle(shape);
    ((MyNodePolygonStyle*)shapeStyle)->addDefaultPoints();
    return shapeStyle;
}

MyShapeStyleBase* createArrowHeadEllipseStyle(const QString& shape) {
    return new MyArrowHeadEllipseStyle(shape);
}

MyShapeStyleBase* createArrowHeadRectStyle(const QString& shape) {
    return new MyArrowHeadRectStyle(shape);
}

MyShapeStyleBase* createArrowHeadPolygonStyle(const QString& shape) {
    return new MyArrowHeadPolygonStyle(shape);
}

MyShapeStyleBase* createArrowHeadDefaultPolygonStyle(const QString& shape) {
    MyShapeStyleBase* shapeStyle = new MyArrowHeadPolygonStyle(shape);
    ((MyArrowHeadPolygonStyle*)shapeStyle)->addDefaultPoints();
    return shapeStyle;
}

MyShapeStyleBase* createLineStyle(const QString& shape) {
    return new MyLineStyle(shape);
}

MyShapeStyleBase* createBezierStyle(const QString& shape) {
    return new MyBezierStyle(shape);
}

MyShapeStyleBase* createTextStyle(const QString& shape) {
    return new MyTextStyle(shape);
}
