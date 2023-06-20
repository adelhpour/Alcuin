#ifndef __NEGUI_EDGE_BUILDER_H
#define __NEGUI_EDGE_BUILDER_H

#include "negui_network_element_base.h"

MyNetworkElementBase* createEdge(const QString& name, MyNetworkElementStyleBase* edgeStyle, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

MyNetworkElementBase* createEdge(const QJsonObject &json, MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode);

#endif
