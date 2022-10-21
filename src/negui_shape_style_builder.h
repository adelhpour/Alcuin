#ifndef __NEGUI_SHAPE_STYLE_BUILDER_H
#define __NEGUI_SHAPE_STYLE_BUILDER_H

#include "negui_shape_style_base.h"

MyShapeStyleBase* createNodeEllipseStyle(const QString& shape);

MyShapeStyleBase* createNodeRectStyle(const QString& shape);

MyShapeStyleBase* createNodePolygonStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadEllipseStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadRectStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadPolygonStyle(const QString& shape);

MyShapeStyleBase* createLineStyle(const QString& shape);

MyShapeStyleBase* createBezierStyle(const QString& shape);

MyShapeStyleBase* createTextStyle(const QString& shape);

#endif
