#include "negui_edge_style.h"
#include "negui_arrow_head_style_builder.h"
#include "negui_shape_style_builder.h"
#include "negui_network_element_icon_builder.h"
#include <QJsonObject>
#include <QJsonArray>

// MyEdgeStyleBase

MyEdgeStyleBase::MyEdgeStyleBase(const QString& name) : MyNetworkElementStyleBase(name) {
    _category = "Edge";
    _connectableSourceNodeCategories.push_back("Node");
    _connectableTargetNodeCategories.push_back("Node");
    QJsonObject arrowHeadStyleObject;
    arrowHeadStyleObject["name"] = name + "_ArrowHead";
    _arrowHeadStyle = createArrowHeadStyle(arrowHeadStyleObject);
    _iconSize = QSize(35, 35);
}

const QString MyEdgeStyleBase::type() const {
    return "edgestyle";
}

bool MyEdgeStyleBase::isConnectableToNodeCategory(const QString& connectedNodeCategory, const QString& connectToNodeAs) {
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

bool MyEdgeStyleBase::isConnectableToSourceNodeCategory(const QString& connectedSourceNodeCategory) {
    for (QString connectableSourceNodeCategory : _connectableSourceNodeCategories) {
        if (connectableSourceNodeCategory == connectedSourceNodeCategory)
            return true;
    }
    
    return false;
}

bool MyEdgeStyleBase::isConnectableToTargetNodeCategory(const QString& connectedTargetNodeCategory) {
    for (QString connectableTargetNodeCategory : _connectableTargetNodeCategories) {
        if (connectableTargetNodeCategory == connectedTargetNodeCategory)
            return true;
    }
    
    return false;
}

const QString& MyEdgeStyleBase::connectableSourceNodeTitle() {
    return _connectableSourceNodeTitle;
}

QList<QString> MyEdgeStyleBase::connectableSourceNodeCategories() {
    return _connectableSourceNodeCategories;
}

const QString& MyEdgeStyleBase::connectableTargetNodeTitle() {
    return _connectableTargetNodeTitle;
}

QList<QString> MyEdgeStyleBase::connectableTargetNodeCategories() {
    return _connectableTargetNodeCategories;
}

MyNetworkElementStyleBase* MyEdgeStyleBase::arrowHeadStyle() {
    return _arrowHeadStyle;
}

QObject* MyEdgeStyleBase::createIconBuilder() {
    return new MyEdgeIconBuilder(this);
}

const QString MyEdgeStyleBase::toolTipText() {
    return toolTipText("source");
}

const QString MyEdgeStyleBase::toolTipText(const QString& connectToNodeAs) {
    if (connectToNodeAs == "source")
        return sourceToolTipText();
    else if (connectToNodeAs == "target")
        return targetToolTipText();
    
    return "Select Node";
}

const QString MyEdgeStyleBase::sourceToolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableSourceNodeCategories.size(); i++) {
        text.append(_connectableSourceNodeCategories.at(i));
        if (i < _connectableSourceNodeCategories.size() - 1)
            text.append("/");
    }
    return text;
}

const QString MyEdgeStyleBase::targetToolTipText() {
    QString text = "Select ";
    for (int i = 0; i < _connectableTargetNodeCategories.size(); i++) {
        text.append(_connectableTargetNodeCategories.at(i));
        if (i < _connectableTargetNodeCategories.size() - 1)
            text.append("/");
    }
    return text;
}

void MyEdgeStyleBase::read(const QJsonObject &json) {
    MyNetworkElementStyleBase::read(json);

    _connectableSourceNodeTitle = "";
    if (json.contains("connectable-source-node-title") && json["connectable-source-node-title"].isString())
        _connectableSourceNodeTitle = json["connectable-source-node-title"].toString();
    
    _connectableSourceNodeCategories.clear();
    if (json.contains("connectable-source-node-categories") && json["connectable-source-node-categories"].isArray()) {
        QJsonArray connectableSourceNodeCategoriesArray = json["connectable-source-node-categories"].toArray();
        for (int connectableSourceNodeCategoryIndex = 0; connectableSourceNodeCategoryIndex < connectableSourceNodeCategoriesArray.size(); ++connectableSourceNodeCategoryIndex) {
            if (connectableSourceNodeCategoriesArray[connectableSourceNodeCategoryIndex].isString())
                _connectableSourceNodeCategories.push_back(connectableSourceNodeCategoriesArray[connectableSourceNodeCategoryIndex].toString());
        }
    }

    _connectableTargetNodeTitle = "";
    if (json.contains("connectable-target-node-title") && json["connectable-target-node-title"].isString())
        _connectableTargetNodeTitle = json["connectable-target-node-title"].toString();
    
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

void MyEdgeStyleBase::write(QJsonObject &json) {
    MyNetworkElementStyleBase::write(json);

    json["connectable-source-node-title"] = connectableSourceNodeTitle();
    
    QJsonArray connectableSourceNodeCategoriesArray;
    for (QString connectableSourceNodeCategory : connectableSourceNodeCategories())
        connectableSourceNodeCategoriesArray.append(connectableSourceNodeCategory);
    json["connectable-source-node-categories"] = connectableSourceNodeCategoriesArray;

    json["connectable-target-node-title"] = connectableTargetNodeTitle();
    
    QJsonArray connectableTargetNodeCategoriesArray;
    for (QString connectableTargetNodeCategory : connectableTargetNodeCategories())
        connectableTargetNodeCategoriesArray.append(connectableTargetNodeCategory);
    json["connectable-target-node-categories"] = connectableTargetNodeCategoriesArray;
    
    QJsonObject arrowHeadObject;
    arrowHeadStyle()->write(arrowHeadObject);
    json["arrow-head"] = arrowHeadObject;
}

// MyClassicEdgeStyle

MyClassicEdgeStyle::MyClassicEdgeStyle(const QString& name) : MyEdgeStyleBase(name) {

}

MyEdgeStyleBase::EDGE_STYLE_TYPE MyClassicEdgeStyle::edgeStyleType() {
    return  CLASSIC_EDGE_STYLE;
}

void MyClassicEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createClassicLineStyle("line"));
}

MyShapeStyleBase* MyClassicEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "line")
        return createClassicLineStyle("line");

    return NULL;
}

// MyConnectedToCentroidNodeEdgeStyleBase

MyConnectedToCentroidNodeEdgeStyleBase::MyConnectedToCentroidNodeEdgeStyleBase(const QString& name) : MyEdgeStyleBase(name) {

}

// MyConnectedToSourceCentroidNodeEdgeStyle

MyConnectedToSourceCentroidNodeEdgeStyle::MyConnectedToSourceCentroidNodeEdgeStyle(const QString& name) : MyConnectedToCentroidNodeEdgeStyleBase(name) {

}

MyEdgeStyleBase::EDGE_STYLE_TYPE MyConnectedToSourceCentroidNodeEdgeStyle::edgeStyleType() {
    return  CONNECTED_TO_SOURCE_CENTROID_NODE_EDGE_STYLE;
}

void MyConnectedToSourceCentroidNodeEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createConnectedToStartCentroidShapeLineStyle("line"));
}

MyShapeStyleBase* MyConnectedToSourceCentroidNodeEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "connected-to-source-centroid-shape-line")
        return createConnectedToStartCentroidShapeLineStyle("connected-to-source-centroid-shape-line");

    return NULL;
}

// MyConnectedToTargetCentroidNodeEdgeStyle

MyConnectedToTargetCentroidNodeEdgeStyle::MyConnectedToTargetCentroidNodeEdgeStyle(const QString& name) : MyConnectedToCentroidNodeEdgeStyleBase(name) {

}

MyEdgeStyleBase::EDGE_STYLE_TYPE MyConnectedToTargetCentroidNodeEdgeStyle::edgeStyleType() {
    return  CONNECTED_TO_TARGET_CENTROID_NODE_EDGE_STYLE;
}

void MyConnectedToTargetCentroidNodeEdgeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createConnectedToEndCentroidShapeLineStyle("line"));
}

MyShapeStyleBase* MyConnectedToTargetCentroidNodeEdgeStyle::createShapeStyle(const QString& shape) {
    if (shape == "connected-to-target-centroid-shape-line")
        return createConnectedToEndCentroidShapeLineStyle("connected-to-target-centroid-shape-line");

    return NULL;
}









