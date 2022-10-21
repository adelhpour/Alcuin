#include "negui_plugin_item_base.h"

// MyPluginItemBase

MyPluginItemBase::MyPluginItemBase(const QString& name) : MyBase(name) {
    _iconSize = QSize(0, 0);
}

const QSize& MyPluginItemBase::iconSize() const {
    return _iconSize;
}
