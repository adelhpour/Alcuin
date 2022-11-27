#include "negui_node_style.h"
#include "negui_shape_style_builder.h"
#include "negui_element_graphics_item_builder.h"

// MyNodeStyle

MyNodeStyle::MyNodeStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "Node";
    _convertibleParentCategory = "Node";
    _parentCategories.push_back("Node");
    _connectableCategories.push_back("Node");
    if (name == "Default")
        addDefaultShapeStyle();
    
    _iconSize = QSize(40, 40);
}

const QString MyNodeStyle::type() const {
    return "nodestyle";
}

void MyNodeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createNodeEllipseStyle("ellipse"));
}

MyShapeStyleBase* MyNodeStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return createNodeEllipseStyle(shape);
    else if (shape == "rect")
        return createNodeRectStyle(shape);
    else if (shape == "polygon")
        return createNodePolygonStyle(shape);
    else if (shape == "text")
        return createTextStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*>  MyNodeStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;
    
    // node style
    MyElementGraphicsItemBase* nodeIconItem = createNodeIconGraphicsItem();
    nodeIconItem->addShapeItems(shapeStyles());
    items.push_back(nodeIconItem);
    
    return items;
}
