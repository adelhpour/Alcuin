#include "negui_shape_style_builder.h"
#include "negui_ellipse_style.h"
#include "negui_rect_style.h"
#include "negui_polygon_style.h"
#include "negui_centroid_style.h"
#include "negui_line_style.h"
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

MyShapeStyleBase* createNodeCentroidStyle(const QString& shape) {
    return new MyNodeCentroidStyle(shape);
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


MyShapeStyleBase* createClassicLineStyle(const QString& shape) {
    return new MyClassicLineStyle(shape);
}

MyShapeStyleBase* createConnectedToCentroidNodeLineStyle(const QString& shape) {
    return new MyConnectedToCentroidNodeLineStyle(shape);
}

MyShapeStyleBase* createTextStyle(const QString& shape) {
    return new MyTextStyle(shape);
}
