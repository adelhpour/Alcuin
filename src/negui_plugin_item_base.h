#ifndef __NEGUI_PLUGIN_ITEM_BASE_H
#define __NEGUI_PLUGIN_ITEM_BASE_H

#include "negui_base.h"

#include <QIcon>

class MyPluginItemBase : public MyBase {
    
public:
    
    MyPluginItemBase(const QString& name);
    virtual const QString type() const;
    const bool isFrequentlyUsed();
    const QString& category() const;
    const QString& subCategory() const;
    QList<MyBase*> callFunctions() const;
    void clearCallFunctions();
    virtual const QIcon icon();
    const QString& iconDirectory() const;
    const QSize& iconSize() const;
    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) override;
    
protected:
    void updateCallFunctions(const QJsonObject &json);
    void addCallFunction(const QJsonObject &json);
    bool _isFrequentlyUsed;
    QString _type;
    QString _category;
    QString _subCategory;
    QList<MyBase*> _callFunctions;
    QSize _iconSize;
    QString _iconDirectory;
};

QList<MyPluginItemBase*> getPluginsOfType(QList<MyPluginItemBase*> plugins, const QString& type);
QList<MyPluginItemBase*> getPluginsOfCategory(QList<MyPluginItemBase*> plugins, const QString& category);
QList<MyPluginItemBase*> getPluginsOfSubCategory(QList<MyPluginItemBase*> plugins, const QString& subCategory);
MyPluginItemBase* findPluginByName(QList<MyPluginItemBase*> plugins, const QString& pluginName);
const QList<QString> getPluginsCategories(QList<MyPluginItemBase*> plugins);
const QList<QString> getPluginsSubCategories(QList<MyPluginItemBase*> plugins);
MyPluginItemBase* getDefaultSavePlugin(QList<MyPluginItemBase*> plugins);

#endif
