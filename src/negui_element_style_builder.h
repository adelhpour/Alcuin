#ifndef __NEGUI_ELEMENT_STYLE_BUILDER_H
#define __NEGUI_ELEMENT_STYLE_BUILDER_H

#include "negui_element_style_base.h"

MyElementStyleBase* createNodeStyle(const QString& name);

MyElementStyleBase* createEdgeStyle(const QString& name);

MyElementStyleBase* createArrowHeadStyle(const QString& name);

#endif
