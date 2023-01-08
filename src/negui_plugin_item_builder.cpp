#include "negui_plugin_item_builder.h"
#include "negui_element_style_builder.h"
#include "negui_import_tools.h"
#include "negui_export_tools.h"
#include "negui_autolayout_engines.h"

MyPluginItemBase* createPluginItem(const QString& name, const QString& type) {
    if (type == "importtool")
        return createImportTool(name);
    else if (type == "dataexporttool")
        return createDataExportTool(name);
    else if (type == "printexporttool")
        return createPrintExportTool(name);
    else if (type == "nodestyle")
        return createNodeStyle(name);
    else if (type == "edgestyle")
        return createEdgeStyle(name);
    else if (type == "templatestyle")
        return createTemplateStyle(name);
    else if (type == "autolayoutengine")
        return createAutoLayoutEngine(name);
    
    return NULL;
}

MyPluginItemBase* createImportTool(const QString& name) {
    return new MyImportTool(name);
}

MyPluginItemBase* createDataExportTool(const QString& name) {
    return new MyDataExportTool(name);
}

MyPluginItemBase* createPrintExportTool(const QString& name) {
    return new MyPrintExportTool(name);
}

MyPluginItemBase* createAutoLayoutEngine(const QString& name) {
    return new MyAutoLayoutEngine(name);
}
