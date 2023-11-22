#include "negui_default_plugin_item.h"

// MyDefaultPluginItem

MyDefaultPluginItem::MyDefaultPluginItem(const QString& name) : MyPluginItemBase(name) {
    _iconSize = QSize(75, 25);
}

const QString MyDefaultPluginItem::type() const {
    return "default";
}

const QIcon MyDefaultPluginItem::icon() {
    QPixmap pixMap(iconDirectory());
    return QIcon(pixMap);
}

const QString& MyDefaultPluginItem::iconDirectory() const {
    return _iconDirectory;
}

void MyDefaultPluginItem::read(const QJsonObject &json) {
    MyPluginItemBase::read(json);

    // icon directory
    if (json.contains("icon-file") && json["icon-file"].isString())
        _iconDirectory = json["icon-file"].toString();
}

void MyDefaultPluginItem::write(QJsonObject &json) {
    MyPluginItemBase::write(json);
    // name
    json["name"] = name();

    // icon directory
    json["icon-file"] = iconDirectory();
}