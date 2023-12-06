#include "negui_plugin_item_base.h"
#include "negui_plugin_item_call_function.h"
#include <QJsonArray>

// MyPluginItemBase

MyPluginItemBase::MyPluginItemBase(const QString& name) : MyBase(name) {
    _isFrequentlyUsed = false;
    _iconSize = QSize(0, 0);
}

const bool MyPluginItemBase::isFrequentlyUsed() {
    return _isFrequentlyUsed;
}

const QString MyPluginItemBase::type() const {
    return _type;
}

const QString& MyPluginItemBase::category() const {
    return _category;
}

const QString& MyPluginItemBase::subCategory() const {
    return _subCategory;
}

QList<MyBase*> MyPluginItemBase::callFunctions() const {
    return _callFunctions;
}

void MyPluginItemBase::clearCallFunctions() {
    while(callFunctions().size())
        delete callFunctions().takeLast();
}

const QIcon MyPluginItemBase::icon() {
    QPixmap pixMap(iconDirectory());
    return QIcon(pixMap);
}

const QString& MyPluginItemBase::iconDirectory() const {
    return _iconDirectory;
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
    if (json.contains("type") && json["type"].isString())
        _type = json["type"].toString();
    if (json.contains("category") && json["category"].isString())
        _category = json["category"].toString();
    if (json.contains("sub-category") && json["sub-category"].isString())
        _subCategory = json["sub-category"].toString();
    updateCallFunctions(json);
}

void MyPluginItemBase::write(QJsonObject &json) {
    json["is-frequently-used"]  = isFrequentlyUsed();
    json["type"] = type();
    json["category"] = category();
    json["sub-category"] = subCategory();
    QJsonArray callFunctionsArray;
    for (MyBase* callFunction : callFunctions()) {
        QJsonObject callFunctionObject;
        callFunction->write(callFunctionObject);
        callFunctionsArray.append(callFunctionObject);
    }
    json["call-functions"] = callFunctionsArray;
}

void MyPluginItemBase::updateCallFunctions(const QJsonObject &json) {
    clearCallFunctions();
    if (json.contains("call-functions") && json["call-functions"].isArray()) {
        QJsonArray callFunctionsArray = json["call-functions"].toArray();
        for (int callFunctionIndex = 0; callFunctionIndex < callFunctionsArray.size(); ++callFunctionIndex) {
            if (callFunctionsArray[callFunctionIndex].isObject())
                addCallFunction(callFunctionsArray[callFunctionIndex].toObject());
        }
    }
}

void MyPluginItemBase::addCallFunction(const QJsonObject &json) {
    if (json.contains("name") && json["name"].isString()) {
        MyBase* callFunction = new MyPluginItemCallFunction(json["name"].toString());
        callFunction->read(json);
        _callFunctions.append(callFunction);
    }
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
