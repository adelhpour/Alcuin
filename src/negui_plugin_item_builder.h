#ifndef __NEGUI_PLUGIN_ITEM_BUILDER_H
#define __NEGUI_PLUGIN_ITEM_BUILDER_H

#include "negui_plugin_item_base.h"

MyPluginItemBase* createPluginItem(const QString& name, const QString& type);

MyPluginItemBase* createImportTool(const QString& name);

MyPluginItemBase* createDataExportTool(const QString& name);

MyPluginItemBase* createPrintExportTool(const QString& name);

MyPluginItemBase* createAutoLayoutEngine(const QString& name);

#endif
