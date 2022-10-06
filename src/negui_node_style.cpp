#include "negui_node_style.h"

// MyNodeStyle

MyNodeStyle::MyNodeStyle(const QString& name) : MyElementStyleBase(name) {
    if (name == "Default")
        addDefaultShapeStyle();
    
    _iconSize = QSize(40, 40);
}

void MyNodeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(new MyNodeEllipseStyle("ellipse"));
}

MyShapeStyleBase* MyNodeStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return new MyNodeEllipseStyle(shape);
    else if (shape == "rect")
        return new MyNodeRectStyle(shape);
    else if (shape == "polygon")
        return new MyNodePolygonStyle(shape);
    else if (shape == "text")
        return new MyTextStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*>  MyNodeStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;
    
    // node style
    MyElementGraphicsItemBase* nodeIconItem = new MyNodeIconGraphicsItem();
    nodeIconItem->addShapeItems(shapeStyles());
    items.push_back(nodeIconItem);
    
    return items;
}
