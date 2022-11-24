#ifndef __NEGUI_ELEMENT_BUILDER_H
#define __NEGUI_ELEMENT_BUILDER_H

#include "negui_element_base.h"

MyElementBase* createNode(const QString& name, const qreal& x, const qreal& y);

MyElementBase* createNode(const QJsonObject &json);

MyElementBase* createEdge(const QString& name, MyElementBase* startNode, MyElementBase* endNode);

MyElementBase* createEdge(const QJsonObject &json, MyElementBase* startNode, MyElementBase* endNode);

MyElementBase* createArrowHead(const QString& name, MyElementBase* edge);

#endif
