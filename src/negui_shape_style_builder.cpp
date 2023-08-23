#include "negui_shape_style_builder.h"
#include "negui_ellipse_style.h"
#include "negui_rectangle_style.h"
#include "negui_polygon_style.h"
#include "negui_centroid_style.h"
#include "negui_line_style.h"
#include "negui_text_style.h"

MyShapeStyleBase* createNodeEllipseStyle(const QString& shape) {
    return new MyNodeEllipseStyle(shape);
}

MyShapeStyleBase* createNodeRectangleStyle(const QString& shape) {
    return new MyNodeRectangleStyle(shape);
}

MyShapeStyleBase* createNodePolygonStyle(const QString& shape) {
    return new MyNodePolygonStyle(shape);
}

MyShapeStyleBase* createNodeDefaultPolygonStyle(const QString& shape) {
    MyShapeStyleBase* shapeStyle = new MyNodePolygonStyle(shape);
    ((MyNodePolygonStyle*)shapeStyle)->addDefaultPoints();
    return shapeStyle;
}

MyShapeStyleBase* createNodeCentroidStyle(const QString& shape) {
    return new MyNodeCentroidStyle(shape);
}

MyShapeStyleBase* createArrowHeadEllipseStyle(const QString& shape) {
    return new MyArrowHeadEllipseStyle(shape);
}

MyShapeStyleBase* createArrowHeadRectangleStyle(const QString& shape) {
    return new MyArrowHeadRectangleStyle(shape);
}

MyShapeStyleBase* createArrowHeadPolygonStyle(const QString& shape) {
    return new MyArrowHeadPolygonStyle(shape);
}

MyShapeStyleBase* createArrowHeadDefaultPolygonStyle(const QString& shape) {
    MyShapeStyleBase* shapeStyle = new MyArrowHeadPolygonStyle(shape);
    ((MyArrowHeadPolygonStyle*)shapeStyle)->addDefaultPoints();
    return shapeStyle;
}


MyShapeStyleBase* createClassicLineStyle(const QString& shape) {
    return new MyClassicLineStyle(shape);
}

MyShapeStyleBase* createConnectedToStartCentroidShapeLineStyle(const QString& shape) {
    return new MyConnectedToStartCentroidShapeLineStyle(shape);
}

MyShapeStyleBase* createConnectedToEndCentroidShapeLineStyle(const QString& shape) {
    return new MyConnectedToEndCentroidShapeLineStyle(shape);
}

MyShapeStyleBase* createSimpleTextStyle(const QString& shape) {
    return new MySimpleTextStyle(shape);
}

MyShapeStyleBase* createWithPlainTextTextStyle(const QString& shape) {
    return new MyWithPlainTextTextStyle(shape);
}
