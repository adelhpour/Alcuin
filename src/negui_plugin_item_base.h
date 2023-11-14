#ifndef __NEGUI_PLUGIN_ITEM_BASE_H
#define __NEGUI_PLUGIN_ITEM_BASE_H

#include "negui_base.h"
#include <QIcon>

class MyPluginItemBase : public MyBase {
    
public:
    
    MyPluginItemBase(const QString& name);
    virtual const QString type() const = 0;
    const bool isFrequentlyUsed();
    const QString& category() const;
    const QString& subCategory() const;
    virtual const QIcon icon() = 0;
    const QSize& iconSize() const;
    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) override;
    
protected:
    bool _isFrequentlyUsed;
    QString _category;
    QString _subCategory;
    QSize _iconSize;
};

QList<MyPluginItemBase*> getPluginsOfType(QList<MyPluginItemBase*> plugins, const QString& type);
QList<MyPluginItemBase*> getPluginsOfCategory(QList<MyPluginItemBase*> plugins, const QString& category);
QList<MyPluginItemBase*> getPluginsOfSubCategory(QList<MyPluginItemBase*> plugins, const QString& subCategory);
MyPluginItemBase* findPluginByName(QList<MyPluginItemBase*> plugins, const QString& pluginName);
const QList<QString> getPluginsCategories(QList<MyPluginItemBase*> plugins);
const QList<QString> getPluginsSubCategories(QList<MyPluginItemBase*> plugins);

#endif
