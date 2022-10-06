#ifndef __NEGUI_NODE_STYLE_H
#define __NEGUI_NODE_STYLE_H

#include "negui_element_style_base.h"
#include "negui_ellipse_style.h"
#include "negui_rect_style.h"
#include "negui_polygon_style.h"
#include "negui_text_style.h"
#include "negui_node_graphics_item.h"

class MyNodeStyle : public MyElementStyleBase {
public:
    
    MyNodeStyle(const QString& name);
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
};

#endif
