#include "negui_arrow_head_style.h"

// MyArrowHeadStyle

MyArrowHeadStyle::MyArrowHeadStyle(const QString& name) : MyElementStyleBase(name) {
    
}

void MyArrowHeadStyle::addDefaultShapeStyle() {
    
}

MyShapeStyleBase* MyArrowHeadStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return new MyArrowHeadEllipseStyle(shape);
    else if (shape == "rect")
        return new MyArrowHeadRectStyle(shape);
    else if (shape == "polygon")
        return new MyArrowHeadPolygonStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*> MyArrowHeadStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;
    
    // arrow head style
    MyElementGraphicsItemBase* arrowHeadIconItem = new MyArrowHeadIconGraphicsItem();
    arrowHeadIconItem->addShapeItems(shapeStyles());
    items.push_back(arrowHeadIconItem);
    
    return items;
}
