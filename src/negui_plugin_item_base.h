#ifndef __NEGUI_PLUGIN_ITEM_BASE_H
#define __NEGUI_PLUGIN_ITEM_BASE_H

#include "negui_base.h"
#include <QIcon>

class MyPluginItemBase : public MyBase {
    
public:
    
    MyPluginItemBase(const QString& name);
    
    // icon
    virtual const QIcon icon() = 0;
    const QSize& iconSize() const;
    
protected:
    
    QSize _iconSize;
};

#endif
