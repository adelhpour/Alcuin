#ifndef __NEGUI_NODE_BUILDER_H
#define __NEGUI_NODE_BUILDER_H

#include "negui_element_base.h"

MyElementBase* createNode(const QString& name, MyElementStyleBase* nodeStyle, const qreal& x, const qreal& y);

MyElementBase* createNode(const QJsonObject &json);

#endif
