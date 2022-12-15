#include "negui_arrow_head_style.h"
#include "negui_shape_style_builder.h"
#include "negui_element_graphics_item_builder.h"

// MyArrowHeadStyle

MyArrowHeadStyle::MyArrowHeadStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "ArrowHead";
}

const QString MyArrowHeadStyle::type() const {
    return "arrowheadstyle";
}

void MyArrowHeadStyle::addDefaultShapeStyle() {
    
}

MyShapeStyleBase* MyArrowHeadStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return createArrowHeadEllipseStyle(shape);
    else if (shape == "rect")
        return createArrowHeadRectStyle(shape);
    else if (shape == "polygon")
        return createArrowHeadPolygonStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*> MyArrowHeadStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;
    
    // arrow head style
    MyElementGraphicsItemBase* arrowHeadIconItem = createArrowHeadIconGraphicsItem();
    arrowHeadIconItem->addShapeItems(shapeStyles());
    items.push_back(arrowHeadIconItem);
    
    return items;
}

const QString MyArrowHeadStyle::toolTipText() {
    return "";
}

const QString MyArrowHeadStyle::alternativeToolTipText() {
    return "";
}

void MyArrowHeadStyle::write(QJsonObject &json) {
    MyElementStyleBase::write(json);
    
    QRectF extents = getShapesExtents();
    
    QJsonObject positionObject;
    positionObject["x"] = extents.center().x();
    positionObject["y"] = extents.center().y();
    json["position"] = positionObject;
    
    QJsonObject dimensionsObject;
    dimensionsObject["width"] = extents.width();
    dimensionsObject["height"] = extents.height();
    json["dimensions"] = dimensionsObject;
}
