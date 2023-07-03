#ifndef __NEGUI_EDGE_STYLE_BUILDER_H
#define __NEGUI_EDGE_STYLE_BUILDER_H

#include "negui_network_element_style_base.h"

MyNetworkElementStyleBase* createEdgeStyle(const QJsonObject &json);

const bool isConnectedToCentroidNodeEdgeStyle(const QJsonObject &json);

#endif
