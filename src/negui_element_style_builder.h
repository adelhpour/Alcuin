#ifndef __NEGUI_ELEMENT_STYLE_BUILDER_H
#define __NEGUI_ELEMENT_STYLE_BUILDER_H

#include "negui_element_style_base.h"

MyElementStyleBase* createNodeStyle(const QString& name);

MyElementStyleBase* createNodeStyle(const QJsonObject &json);

MyElementStyleBase* createEdgeStyle(const QString& name);

MyElementStyleBase* createEdgeStyle(const QJsonObject &json);

MyElementStyleBase* createArrowHeadStyle(const QString& name);

MyElementStyleBase* createTemplateStyle(const QString& name);

#endif
