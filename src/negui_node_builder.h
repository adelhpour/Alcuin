#ifndef __NEGUI_NODE_BUILDER_H
#define __NEGUI_NODE_BUILDER_H

#include "negui_network_element_base.h"

MyNetworkElementBase* createNode(const QString& name, MyNetworkElementStyleBase* nodeStyle, const qreal& x, const qreal& y);

MyNetworkElementBase* createNode(const QJsonObject &json);

#endif
