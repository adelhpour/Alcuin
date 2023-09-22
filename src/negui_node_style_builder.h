#ifndef __NEGUI_NODE_STYLE_BUILDER_H
#define __NEGUI_NODE_STYLE_BUILDER_H

#include "negui_network_element_style_base.h"

MyNetworkElementStyleBase* createNodeStyle(const QJsonObject &json);

const bool isCentroidNodeStyle(const QJsonObject &json);

const bool isSimpleClassicNodeStyle(const QJsonObject &json);

const bool whetherHaveTwoShapesWithOneOfThemATextShapeOrOneShape(const QJsonArray& shapesArray);

#endif
