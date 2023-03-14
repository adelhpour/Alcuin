#ifndef __NEGUI_NODE_STYLE_BUILDER_H
#define __NEGUI_NODE_STYLE_BUILDER_H

#include "negui_element_style_base.h"

MyElementStyleBase* createNodeStyle(const QJsonObject &json);

const bool isCentroidNodeStyle(const QJsonObject &json);

#endif
