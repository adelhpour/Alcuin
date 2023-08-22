#include "negui_node_style.h"
#include "negui_shape_style_builder.h"
#include "negui_network_element_icon_builder.h"
#include <QJsonArray>

// MyNodeStyleBase

MyNodeStyleBase::MyNodeStyleBase(const QString& name) : MyNetworkElementStyleBase(name) {
    _category = "Node";
    _parentCategories.push_back("Node");
    _iconSize = QSize(35, 35);
}

const QString MyNodeStyleBase::type() const {
    return "nodestyle";
}

QObject* MyNodeStyleBase::createIconBuilder() {
    return new MyNodeIconBuilder(this);
}

const QString MyNodeStyleBase::toolTipText() {
    return "Add " + category();
}

const QString& MyNodeStyleBase::parentTitle() {
    return _parentTitle;
}

QList<QString> MyNodeStyleBase::parentCategories() {
    return _parentCategories;
}

void MyNodeStyleBase::read(const QJsonObject &json) {
    MyNetworkElementStyleBase::read(json);

    _parentTitle = "";
    if (json.contains("parent-title") && json["parent-title"].isString())
        _parentTitle = json["parent-title"].toString();

    _parentCategories.clear();
    if (json.contains("parent-categories") && json["parent-categories"].isArray()) {
        QJsonArray parentCategoriesArray = json["parent-categories"].toArray();
        for (int parentCategoryIndex = 0; parentCategoryIndex < parentCategoriesArray.size(); ++parentCategoryIndex) {
            if (parentCategoriesArray[parentCategoryIndex].isString())
                _parentCategories.push_back(parentCategoriesArray[parentCategoryIndex].toString());
        }
    }
}

void MyNodeStyleBase::write(QJsonObject &json) {
    MyNetworkElementStyleBase::write(json);

    json["parent-title"] = parentTitle();

    QJsonArray parentCategoriesArray;
    for (QString parentCategory : parentCategories())
        parentCategoriesArray.append(parentCategory);
    json["parent-categories"] = parentCategoriesArray;
}

// MyClassicNodeStyleBase

MyClassicNodeStyleBase::MyClassicNodeStyleBase(const QString& name) : MyNodeStyleBase(name) {
    _convertibleParentCategory = "Node";
}

void MyClassicNodeStyleBase::addDefaultShapeStyle() {
    _shapeStyles.push_back(createNodeEllipseStyle("ellipse"));
}

const QString& MyClassicNodeStyleBase::convertibleParentCategory() const {
    return _convertibleParentCategory;
}

bool MyClassicNodeStyleBase::isConvertibleToParentCategory(QList<QString> parentCategories) {
    for (QString parentCategory : parentCategories) {
        if (parentCategory == _convertibleParentCategory)
            return true;
    }

    return false;
}

void MyClassicNodeStyleBase::convertToParentCategory() {
    _category = convertibleParentCategory();
}

void MyClassicNodeStyleBase::read(const QJsonObject &json) {
    MyNodeStyleBase::read(json);
    _convertibleParentCategory.clear();
    if (json.contains("convertible-parent-category") && json["convertible-parent-category"].isString())
        _convertibleParentCategory = json["convertible-parent-category"].toString();
}

void MyClassicNodeStyleBase::write(QJsonObject &json) {
    MyNodeStyleBase::write(json);
    json["convertible-parent-category"] = convertibleParentCategory();
}

// MySimpleClassicNodeStyle

MySimpleClassicNodeStyle::MySimpleClassicNodeStyle(const QString& name) : MyClassicNodeStyleBase(name) {

}

MyNodeStyleBase::NODE_STYLE_TYPE MySimpleClassicNodeStyle::nodeStyleType() {
    return SIMPLE_CLASSIC_NODE_STYLE;
}

MyShapeStyleBase* MySimpleClassicNodeStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse" && !whetherAnotherGeometricShapeAlreadyExists())
        return createNodeEllipseStyle(shape);
    else if (shape == "rect" && !whetherAnotherGeometricShapeAlreadyExists())
        return createNodeRectStyle(shape);
    else if (shape == "polygon" && !whetherAnotherGeometricShapeAlreadyExists())
        return createNodePolygonStyle(shape);
    else if (shape == "text" && !whetherAnotherTextShapeAlreadyExists())
        return createSimpleTextStyle(shape);

    return NULL;
}

QWidget* MySimpleClassicNodeStyle::shapeStylesButtons() {
    QWidget* shapeStylesButtons = new MyChangeNodeShapeStylesButton();
    ((MyChangeNodeShapeStylesButton*)shapeStylesButtons)->setMenu();
    return shapeStylesButtons;
}

const bool MySimpleClassicNodeStyle::whetherAnotherGeometricShapeAlreadyExists() {
    for (MyShapeStyleBase* shapeStyle : shapeStyles()) {
        if (shapeStyle->name() != "text")
            return true;
    }

    return false;
}

const bool MySimpleClassicNodeStyle::whetherAnotherTextShapeAlreadyExists() {
    for (MyShapeStyleBase* shapeStyle : shapeStyles()) {
        if (shapeStyle->name() == "text")
        return true;
    }

    return false;
}

// MyComplexClassicNodeStyle

MyComplexClassicNodeStyle::MyComplexClassicNodeStyle(const QString& name) : MyClassicNodeStyleBase(name) {

}

MyNodeStyleBase::NODE_STYLE_TYPE MyComplexClassicNodeStyle::nodeStyleType() {
    return COMPLEX_CLASSIC_NODE_STYLE;
}

MyShapeStyleBase* MyComplexClassicNodeStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return createNodeEllipseStyle(shape);
    else if (shape == "rect")
        return createNodeRectStyle(shape);
    else if (shape == "polygon")
        return createNodePolygonStyle(shape);
    else if (shape == "text")
        return createWithPlainTextTextStyle(shape);

    return NULL;
}

QWidget* MyComplexClassicNodeStyle::shapeStylesButtons() {
    QWidget* shapeStylesButtons = new MyAddRemoveNodeShapeStylesButtons();
    ((MyAddRemoveNodeShapeStylesButtons*)shapeStylesButtons)->setAddingMenu();
    return shapeStylesButtons;
}

// MyCentroidNodeStyle

MyCentroidNodeStyle::MyCentroidNodeStyle(const QString& name) : MyNodeStyleBase(name) {
    _parentCategories.push_back("Node");
}

MyNodeStyleBase::NODE_STYLE_TYPE MyCentroidNodeStyle::nodeStyleType() {
    return CENTROID_NODE_STYLE;
}

void MyCentroidNodeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createNodeCentroidStyle("centroid"));
}

MyShapeStyleBase* MyCentroidNodeStyle::createShapeStyle(const QString& shape) {
    if (shape == "centroid")
        return createNodeCentroidStyle(shape);

    return NULL;
}

// MyChangeNodeShapeStylesButton

MyChangeNodeShapeStylesButton::MyChangeNodeShapeStylesButton(QWidget* parent) : MyChangeShapeStylesButtonsBase(parent) {

}

void MyChangeNodeShapeStylesButton::setMenu() {
    connect(_menu->addAction("Ellipse"), &QAction::triggered, this, [this] () {
        emit askForChangeShapeStyle(createNodeEllipseStyle("ellipse")); });
    connect(_menu->addAction("Rect"), &QAction::triggered, this, [this] () { emit askForChangeShapeStyle(createNodeRectStyle("rect")); });
    connect(_menu->addAction("Polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createNodeDefaultPolygonStyle("polygon");
        emit askForChangeShapeStyle(polygonShapeStyle); });
}

// MyAddRemoveNodeShapeStylesButtons

MyAddRemoveNodeShapeStylesButtons::MyAddRemoveNodeShapeStylesButtons(QWidget* parent) : MyAddRemoveShapeStylesButtonsBase(parent) {

}

void MyAddRemoveNodeShapeStylesButtons::setAddingMenu() {
    connect(_addingMenu->addAction("Ellipse"), &QAction::triggered, this, [this] () {
        emit askForAddShapeStyle(createNodeEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("Rect"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createNodeRectStyle("rect")); });
    connect(_addingMenu->addAction("Polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createNodeDefaultPolygonStyle("polygon");
        emit askForAddShapeStyle(polygonShapeStyle); });
    connect(_addingMenu->addAction("Text"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(
            createWithPlainTextTextStyle("text")); });
}
