#include "negui_plugin_item_base.h"

// MyPluginItemBase

const QString& MyPluginItemBase::category() const {
    return _category;
}

const QString& MyPluginItemBase::subCategory() const {
    return _subCategory;
}

MyPluginItemBase::MyPluginItemBase(const QString& name) : MyBase(name) {
    _iconSize = QSize(0, 0);
}

const QSize& MyPluginItemBase::iconSize() const {
    return _iconSize;
}

void MyPluginItemBase::read(const QJsonObject &json) {
    _category.clear();
    if (json.contains("category") && json["category"].isString())
        _category = json["category"].toString();
    if (json.contains("sub-category") && json["sub-category"].isString())
        _subCategory = json["sub-category"].toString();
}

void MyPluginItemBase::write(QJsonObject &json) {
    json["category"] = category();
    json["sub-category"] = subCategory();
}

QList<MyPluginItemBase*> getPluginsOfType(QList<MyPluginItemBase*> plugins, const QString& type) {
    QList<MyPluginItemBase*> pluginsOfType;
    for (MyPluginItemBase* plugin : plugins) {
        if (plugin->type() == type)
            pluginsOfType.push_back(plugin);
    }
    
    return pluginsOfType;
}

QList<MyPluginItemBase*> getPluginsOfCategory(QList<MyPluginItemBase*> plugins, const QString& category) {
    QList<MyPluginItemBase*> pluginsOfCategory;
    for (MyPluginItemBase* plugin : plugins) {
        if (plugin->category() == category)
            pluginsOfCategory.push_back(plugin);
    }
    
    return pluginsOfCategory;
}

QList<MyPluginItemBase*> getPluginsOfSubCategory(QList<MyPluginItemBase*> plugins, const QString& subCategory) {
    QList<MyPluginItemBase*> pluginsOfSubCategory;
    for (MyPluginItemBase* plugin : plugins) {
        if (plugin->subCategory() == subCategory)
            pluginsOfSubCategory.push_back(plugin);
    }
    
    return pluginsOfSubCategory;
}

const QList<QString> getPluginsCategories(QList<MyPluginItemBase*> plugins) {
    QList<QString> categories;
    for (MyPluginItemBase* plugin : plugins) {
        if (!plugin->category().isEmpty() && categories.indexOf(plugin->category()) == -1)
            categories.push_back(plugin->category());
    }
    
    return categories;
}

const QList<QString> getPluginsSubCategories(QList<MyPluginItemBase*> plugins) {
    QList<QString> subCategories;
    for (MyPluginItemBase* plugin : plugins) {
        if (!plugin->subCategory().isEmpty() && subCategories.indexOf(plugin->subCategory()) == -1)
            subCategories.push_back(plugin->subCategory());
    }
    
    return subCategories;
}
