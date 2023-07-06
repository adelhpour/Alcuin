#ifndef __NEGUI_EDGE_STYLE_BUILDER_H
#define __NEGUI_EDGE_STYLE_BUILDER_H

#include "negui_network_element_style_base.h"

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json);

const bool isConnectedToSourceCentroidNode(const QJsonObject &json);

const bool isConnectedToTargetCentroidNode(const QJsonObject &json);

#endif
