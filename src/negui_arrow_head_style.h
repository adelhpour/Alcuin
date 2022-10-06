#ifndef __NEGUI_ARROW_HEAD_STYLE_H
#define __NEGUI_ARROW_HEAD_STYLE_H

#include "negui_element_style_base.h"
#include "negui_ellipse_style.h"
#include "negui_rect_style.h"
#include "negui_polygon_style.h"
#include "negui_arrow_head_graphics_item.h"

class MyArrowHeadStyle : public MyElementStyleBase {
public:
    
    MyArrowHeadStyle(const QString& name);
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
};

#endif
