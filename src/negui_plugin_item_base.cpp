#include "negui_plugin_item_base.h"
#include <QJsonArray>

// MyPluginItemBase

MyPluginItemBase::MyPluginItemBase(const QString& name) : MyBase(name) {
    _isFrequentlyUsed = false;
    _iconSize = QSize(0, 0);
}

const bool MyPluginItemBase::isFrequentlyUsed() {
    return _isFrequentlyUsed;
}

const QString& MyPluginItemBase::category() const {
    return _category;
}

const QString& MyPluginItemBase::subCategory() const {
    return _subCategory;
}

const QStringList& MyPluginItemBase::nameOfCallFunctions() const {
    return _nameOfCallFunctions;
}

const QString& MyPluginItemBase::defaultNameOfCallFunction() const {
    if (_nameOfCallFunctions.size())
        return _nameOfCallFunctions.at(0);

    return "";
}

const QSize& MyPluginItemBase::iconSize() const {
    return _iconSize;
}

void MyPluginItemBase::read(const QJsonObject &json) {
    if (name() != "Default")
        _category.clear();
    _isFrequentlyUsed = false;
    if (json.contains("is-frequently-used") && json["is-frequently-used"].isBool())
        _isFrequentlyUsed = json["is-frequently-used"].toBool();
    if (json.contains("category") && json["category"].isString())
        _category = json["category"].toString();
    if (json.contains("sub-category") && json["sub-category"].isString())
        _subCategory = json["sub-category"].toString();
    _nameOfCallFunctions.clear();
    if (json.contains("name-of-call-functions") && json["name-of-call-functions"].isArray()) {
        QJsonArray nameOfCallFunctionsArray = json["name-of-call-functions"].toArray();
        for (int nameOfCallFunctionIndex = 0; nameOfCallFunctionIndex < nameOfCallFunctionsArray.size(); ++nameOfCallFunctionIndex) {
            if (nameOfCallFunctionsArray[nameOfCallFunctionIndex].isString())
                _nameOfCallFunctions.append(nameOfCallFunctionsArray[nameOfCallFunctionIndex].toString());
        }
    }
}

void MyPluginItemBase::write(QJsonObject &json) {
    json["is-frequently-used"]  = isFrequentlyUsed();
    json["category"] = category();
    json["sub-category"] = subCategory();
    QJsonArray nameOfCallFunctionsArray;
    for (QString nameOfCallFunction : nameOfCallFunctions())
        nameOfCallFunctionsArray.append(nameOfCallFunction);
    json["name-of-call-functions"] = nameOfCallFunctionsArray;
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

MyPluginItemBase* findPluginByName(QList<MyPluginItemBase*> plugins, const QString& pluginName) {
    for (MyPluginItemBase* plugin : plugins) {
        if (plugin->name() == pluginName)
            return plugin;
    }

    return NULL;
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
