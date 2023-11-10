#ifndef __NEGUI_DEFAULT_NETWORK_ELEMENT_STYLE_MANAGER_H
#define __NEGUI_DEFAULT_NETWORK_ELEMENT_STYLE_MANAGER_H

#include <QObject>

#include "negui_plugin_item_base.h"

class MyDefaultNetworkElementStyleManager : public QObject {
    Q_OBJECT

public:

    MyDefaultNetworkElementStyleManager();

    MyPluginItemBase* createDefaultNodeStyle(QList<MyPluginItemBase*> plugins);

    MyPluginItemBase* createDefaultEdgeStyle(QList<MyPluginItemBase*> plugins);
};

#endif