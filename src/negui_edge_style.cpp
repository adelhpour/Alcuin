#include "negui_edge_style.h"
#include "negui_element_style_builder.h"
#include "negui_shape_style_builder.h"
#include "negui_element_graphics_item_builder.h"
#include <QJsonObject>
#include <QJsonArray>

// MyEdgeStyle

MyEdgeStyle::MyEdgeStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "Edge";
    _connectableStartNodeCategories.push_back("Node");
    _connectableEndNodeCategories.push_back("Node");
    if (name == "Default")
        addDefaultShapeStyle();
    
    _arrowHeadStyle = createArrowHeadStyle(name + "_ArrowHead");
    _iconSize = QSize(75, 45);
}

const QString MyEdgeStyle::type() const {
    return "edgestyle";
}

QList<QString> MyEdgeStyle::connectableStartNodeCategories() {
    return _connectableStartNodeCategories;
}

bool MyEdgeStyle::isConnectableToStartNodeCategory(const QString& connectedStartNodeCategory) {
    for (QString connectableStartNodeCategory : _connectableStartNodeCategories) {
        if (connectedStartNodeCategory == connectableStartNodeCategory)
            return true;
    }
    
    return false;
}

QList<QString> MyEdgeStyle::connectableEndNodeCategories() {
    return _connectableEndNodeCategories;
}

bool MyEdgeStyle::isConnectableToEndNodeCategory(const QString& connectedEndNodeCategory) {
    for (QString connectableEndNodeCategory : _connectableEndNodeCategories) {
        if (connectedEndNodeCategory == connectableEndNodeCategory)
            return true;
    }
    
    return false;
}

MyElementStyleBase* MyEdgeStyle::arrowHeadStyle() {
    return _arrowHeadStyle;
}

void MyEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createLineStyle("line"));
}

MyShapeStyleBase* MyEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "line")
        return createLineStyle(shape);
    else if (shape == "bezier")
        return createBezierStyle(shape);
    
    return NULL;
}

QList<MyElementGraphicsItemBase*> MyEdgeStyle::getElementIconGraphicsItems() {
    QList<MyElementGraphicsItemBase*> items;

    // edge style
    MyElementGraphicsItemBase* edgeIconItem = createEdgeIconGraphicsItem();
    edgeIconItem->addShapeItems(shapeStyles());
    items.push_back(edgeIconItem);
    
    // arrow head style
    items += arrowHeadStyle()->getElementIconGraphicsItems();
    
    return items;
}

const QString MyEdgeStyle::toolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableStartNodeCategories.size(); i++) {
        text.append(_connectableStartNodeCategories.at(i));
        if (i < _connectableStartNodeCategories.size() - 1)
            text.append("/");
    }
        
    return text;
}

const QString MyEdgeStyle::alternativeToolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableEndNodeCategories.size(); i++) {
        text.append(_connectableEndNodeCategories.at(i));
        if (i < _connectableEndNodeCategories.size() - 1)
            text.append("/");
    }
        
    return text;
}

void MyEdgeStyle::read(const QJsonObject &json) {
    MyElementStyleBase::read(json);
    
    _connectableStartNodeCategories.clear();
    if (json.contains("connectable-start-node-categories") && json["connectable-start-node-categories"].isArray()) {
        QJsonArray connectableStartNodeCategoriesArray = json["connectable-start-node-categories"].toArray();
        for (int connectableStartNodeCategoryIndex = 0; connectableStartNodeCategoryIndex < connectableStartNodeCategoriesArray.size(); ++connectableStartNodeCategoryIndex) {
            if (connectableStartNodeCategoriesArray[connectableStartNodeCategoryIndex].isString())
                _connectableStartNodeCategories.push_back(connectableStartNodeCategoriesArray[connectableStartNodeCategoryIndex].toString());
        }
    }
    
    _connectableEndNodeCategories.clear();
    if (json.contains("connectable-end-node-categories") && json["connectable-end-node-categories"].isArray()) {
        QJsonArray connectableEndNodeCategoriesArray = json["connectable-end-node-categories"].toArray();
        for (int connectableEndNodeCategoryIndex = 0; connectableEndNodeCategoryIndex < connectableEndNodeCategoriesArray.size(); ++connectableEndNodeCategoryIndex) {
            if (connectableEndNodeCategoriesArray[connectableEndNodeCategoryIndex].isString())
                _connectableEndNodeCategories.push_back(connectableEndNodeCategoriesArray[connectableEndNodeCategoryIndex].toString());
        }
    }
    
    if (json.contains("arrow-head") && json["arrow-head"].isObject())
        arrowHeadStyle()->read(json["arrow-head"].toObject());
}

void MyEdgeStyle::write(QJsonObject &json) {
    MyElementStyleBase::write(json);
    
    QJsonArray connectableStartNodeCategoriesArray;
    for (QString connectableStartNodeCategory : connectableStartNodeCategories())
        connectableStartNodeCategoriesArray.append(connectableStartNodeCategory);
    json["connectable-start-node-categories"] = connectableStartNodeCategoriesArray;
    
    QJsonArray connectableEndNodeCategoriesArray;
    for (QString connectableEndNodeCategory : connectableEndNodeCategories())
        connectableEndNodeCategoriesArray.append(connectableEndNodeCategory);
    json["connectable-end-node-categories"] = connectableEndNodeCategoriesArray;
    
    QJsonObject arrowHeadObject;
    arrowHeadStyle()->write(arrowHeadObject);
    json["arrow-head"] = arrowHeadObject;
}
