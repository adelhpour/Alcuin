#ifndef __NEGUI_PLUGIN_ITEM_BUILDER_H
#define __NEGUI_PLUGIN_ITEM_BUILDER_H

#include "negui_plugin_item_base.h"

MyPluginItemBase* createPluginItem(const QJsonObject &json);

MyPluginItemBase* createPluginItemBase(const QJsonObject &json);

#endif
