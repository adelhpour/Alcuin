#ifndef __NEGUI_EDGE_BUILDER_H
#define __NEGUI_EDGE_BUILDER_H

#include "negui_element_base.h"

MyElementBase* createEdge(const QString& name, MyElementStyleBase* edgeStyle, MyElementBase* startNode, MyElementBase* endNode);

MyElementBase* createEdge(const QJsonObject &json, MyElementBase* startNode, MyElementBase* endNode);

#endif
