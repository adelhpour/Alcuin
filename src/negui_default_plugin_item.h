#ifndef __NEGUI_DEFAULT_PLUGIN_ITEM_H
#define __NEGUI_DEFAULT_PLUGIN_ITEM_H

#include "negui_plugin_item_base.h"

class MyDefaultPluginItem : public MyPluginItemBase {

public:

    MyDefaultPluginItem(const QString& name);
    const QString type() const override;

    // icon
    const QIcon icon() override;
    const QString& iconDirectory() const;

    void read(const QJsonObject &json) override;
    void write(QJsonObject &json) override;

protected:

    QString _iconDirectory;
};

#endif
