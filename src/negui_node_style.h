#ifndef __NEGUI_NODE_STYLE_H
#define __NEGUI_NODE_STYLE_H

#include "negui_element_style_base.h"

class MyNodeStyle : public MyElementStyleBase {
public:
    
    MyNodeStyle(const QString& name);
    
    const QString type() const override;
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
};

#endif
