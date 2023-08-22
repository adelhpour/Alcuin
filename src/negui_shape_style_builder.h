#ifndef __NEGUI_SHAPE_STYLE_BUILDER_H
#define __NEGUI_SHAPE_STYLE_BUILDER_H

#include "negui_shape_style_base.h"

MyShapeStyleBase* createNodeEllipseStyle(const QString& shape);

MyShapeStyleBase* createNodeRectangleStyle(const QString& shape);

MyShapeStyleBase* createNodePolygonStyle(const QString& shape);

MyShapeStyleBase* createNodeDefaultPolygonStyle(const QString& shape);

MyShapeStyleBase* createNodeCentroidStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadEllipseStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadRectangleStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadPolygonStyle(const QString& shape);

MyShapeStyleBase* createArrowHeadDefaultPolygonStyle(const QString& shape);

MyShapeStyleBase* createClassicLineStyle(const QString& shape);

MyShapeStyleBase* createConnectedToStartCentroidShapeLineStyle(const QString& shape);

MyShapeStyleBase* createConnectedToEndCentroidShapeLineStyle(const QString& shape);

MyShapeStyleBase* createSimpleTextStyle(const QString& shape);

MyShapeStyleBase* createWithPlainTextTextStyle(const QString& shape);

#endif
