#include "negui_edge_style.h"

// MyEdgeStyle

MyEdgeStyle::MyEdgeStyle(const QString& name) : MyElementStyleBase(name) {
    if (name == "Default")
        addDefaultShapeStyle();
    
    _arrowHeadStyle = new MyArrowHeadStyle(name + "_ArrowHead");
    _iconSize = QSize(75, 45);
}

MyElementStyleBase* MyEdgeStyle::arrowHeadStyle() {
    return _arrowHeadStyle;
}

void MyEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(new MyLineStyle("line"));
}

MyShapeStyleBase* MyEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "line")
        return new MyLineStyle(shape);
    else if (shape == "bezier")
        return new MyBezierStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*> MyEdgeStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;

    // edge style
    MyElementGraphicsItemBase* edgeIconItem = new MyEdgeIconGraphicsItem();
    edgeIconItem->addShapeItems(shapeStyles());
    items.push_back(edgeIconItem);
    
    // arrow head style
    items += arrowHeadStyle()->getElementIconGraphicsItems();
    
    return items;
}

void MyEdgeStyle::read(const QJsonObject &json) {
    MyElementStyleBase::read(json);
    if (json.contains("arrow-head") && json["arrow-head"].isObject())
        arrowHeadStyle()->read(json["arrow-head"].toObject());
}

void MyEdgeStyle::write(QJsonObject &json) {
    MyElementStyleBase::write(json);
    QJsonObject arrowHeadObject;
    arrowHeadStyle()->write(arrowHeadObject);
    json["arrow-head"] = arrowHeadObject;
}
