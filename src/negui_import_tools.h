#ifndef __NEGUI_IMPORT_TOOLS_H
#define __NEGUI_IMPORT_TOOLS_H

#include "negui_plugin_item_base.h"

class MyImportTool : public MyPluginItemBase {
public:
    
    MyImportTool(const QString& name);
    
    const QString type() const override;
    
    // get the open file name from the user by poping up a file dialog
    const QString getOpenFileName() const;
    
    // get the file extension of the import tool
    const QString& fileExtension() const;
    
    // get the icon associated with this plugin
    const QIcon icon() override;
    
    // get the icon dirctory of the import tool
    const QString& iconDirectory() const;
    
    // read the import info from the json object
    void read(const QJsonObject &json) override;
    
    // write the import info to the json object
    void write(QJsonObject &json) override;
    
protected:
    QString _fileExtension;
    QString _iconDirectory;
};

#endif
