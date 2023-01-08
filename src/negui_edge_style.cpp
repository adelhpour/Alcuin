#include "negui_edge_style.h"
#include "negui_element_style_builder.h"
#include "negui_shape_style_builder.h"
#include "negui_element_icon_builder.h"
#include <QJsonObject>
#include <QJsonArray>

// MyEdgeStyle

MyEdgeStyle::MyEdgeStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "Edge";
    _connectableSourceNodeCategories.push_back("Node");
    _connectableTargetNodeCategories.push_back("Node");
    if (name == "Default")
        addDefaultShapeStyle();
    
    _arrowHeadStyle = createArrowHeadStyle(name + "_ArrowHead");
    _iconSize = QSize(75, 45);
}

const QString MyEdgeStyle::type() const {
    return "edgestyle";
}

bool MyEdgeStyle::isConnectableToNodeCategory(const QString& connectedNodeCategory, const QString& connectToNodeAs) {
    if (connectToNodeAs == "source") {
        if (isConnectableToSourceNodeCategory(connectedNodeCategory))
            return true;
    }
    else if (connectToNodeAs == "target") {
        if (isConnectableToTargetNodeCategory(connectedNodeCategory))
            return true;
    }
    
    return false;
}

bool MyEdgeStyle::isConnectableToSourceNodeCategory(const QString& connectedSourceNodeCategory) {
    for (QString connectableSourceNodeCategory : _connectableSourceNodeCategories) {
        if (connectableSourceNodeCategory == connectedSourceNodeCategory)
            return true;
    }
    
    return false;
}

bool MyEdgeStyle::isConnectableToTargetNodeCategory(const QString& connectedTargetNodeCategory) {
    for (QString connectableTargetNodeCategory : _connectableTargetNodeCategories) {
        if (connectableTargetNodeCategory == connectedTargetNodeCategory)
            return true;
    }
    
    return false;
}

QList<QString> MyEdgeStyle::connectableSourceNodeCategories() {
    return _connectableSourceNodeCategories;
}

QList<QString> MyEdgeStyle::connectableTargetNodeCategories() {
    return _connectableTargetNodeCategories;
}

MyElementStyleBase* MyEdgeStyle::arrowHeadStyle() {
    return _arrowHeadStyle;
}

void MyEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createLineStyle("line"));
}

MyShapeStyleBase* MyEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "line")
        return createLineStyle("line");
    
    return NULL;
}

QObject* MyEdgeStyle::createIconBuilder() {
    return new MyEdgeIconBuilder(this);
}

const QString MyEdgeStyle::toolTipText() {
    return toolTipText("source");
}

const QString MyEdgeStyle::toolTipText(const QString& connectToNodeAs) {
    if (connectToNodeAs == "source")
        return sourceToolTipText();
    else if (connectToNodeAs == "target")
        return targetToolTipText();
    
    return "Select Node";
}

const QString MyEdgeStyle::sourceToolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableSourceNodeCategories.size(); i++) {
        text.append(_connectableSourceNodeCategories.at(i));
        if (i < _connectableSourceNodeCategories.size() - 1)
            text.append("/");
    }
    return text;
}

const QString MyEdgeStyle::targetToolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableTargetNodeCategories.size(); i++) {
        text.append(_connectableTargetNodeCategories.at(i));
        if (i < _connectableTargetNodeCategories.size() - 1)
            text.append("/");
    }
    return text;
}

void MyEdgeStyle::read(const QJsonObject &json) {
    MyElementStyleBase::read(json);
    
    _connectableSourceNodeCategories.clear();
    if (json.contains("connectable-source-node-categories") && json["connectable-source-node-categories"].isArray()) {
        QJsonArray connectableSourceNodeCategoriesArray = json["connectable-source-node-categories"].toArray();
        for (int connectableSourceNodeCategoryIndex = 0; connectableSourceNodeCategoryIndex < connectableSourceNodeCategoriesArray.size(); ++connectableSourceNodeCategoryIndex) {
            if (connectableSourceNodeCategoriesArray[connectableSourceNodeCategoryIndex].isString())
                _connectableSourceNodeCategories.push_back(connectableSourceNodeCategoriesArray[connectableSourceNodeCategoryIndex].toString());
        }
    }
    
    _connectableTargetNodeCategories.clear();
    if (json.contains("connectable-target-node-categories") && json["connectable-target-node-categories"].isArray()) {
        QJsonArray connectableTargetNodeCategoriesArray = json["connectable-target-node-categories"].toArray();
        for (int connectableTargetNodeCategoryIndex = 0; connectableTargetNodeCategoryIndex < connectableTargetNodeCategoriesArray.size(); ++connectableTargetNodeCategoryIndex) {
            if (connectableTargetNodeCategoriesArray[connectableTargetNodeCategoryIndex].isString())
                _connectableTargetNodeCategories.push_back(connectableTargetNodeCategoriesArray[connectableTargetNodeCategoryIndex].toString());
        }
    }
    
    if (json.contains("arrow-head") && json["arrow-head"].isObject())
        arrowHeadStyle()->read(json["arrow-head"].toObject());
}

void MyEdgeStyle::write(QJsonObject &json) {
    MyElementStyleBase::write(json);
    
    QJsonArray connectableSourceNodeCategoriesArray;
    for (QString connectableSourceNodeCategory : connectableSourceNodeCategories())
        connectableSourceNodeCategoriesArray.append(connectableSourceNodeCategory);
    json["connectable-source-node-categories"] = connectableSourceNodeCategoriesArray;
    
    QJsonArray connectableTargetNodeCategoriesArray;
    for (QString connectableTargetNodeCategory : connectableTargetNodeCategories())
        connectableTargetNodeCategoriesArray.append(connectableTargetNodeCategory);
    json["connectable-target-node-categories"] = connectableTargetNodeCategoriesArray;
    
    QJsonObject arrowHeadObject;
    arrowHeadStyle()->write(arrowHeadObject);
    json["arrow-head"] = arrowHeadObject;
}
