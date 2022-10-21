#ifndef __NEGUI_EDGE_STYLE_H
#define __NEGUI_EDGE_STYLE_H

#include "negui_element_style_base.h"

class MyEdgeStyle : public MyElementStyleBase {
public:
    
    MyEdgeStyle(const QString& name);
    
    MyElementStyleBase* arrowHeadStyle();
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
    
    void read(const QJsonObject &json) override;
    
    void write(QJsonObject &json) override;

protected:
    MyElementStyleBase* _arrowHeadStyle;
};


#endif
