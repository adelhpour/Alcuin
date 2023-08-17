#include "negui_node_style.h"
#include "negui_shape_style_builder.h"
#include "negui_network_element_icon_builder.h"
#include <QJsonArray>

// MyNodeStyleBase

MyNodeStyleBase::MyNodeStyleBase(const QString& name) : MyNetworkElementStyleBase(name) {
    _category = "Node";
    _parentCategories.push_back("Node");
    _iconSize = QSize(40, 40);
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

QList<QString> MyNodeStyleBase::parentCategories() {
    return _parentCategories;
}

void MyNodeStyleBase::read(const QJsonObject &json) {
    MyNetworkElementStyleBase::read(json);
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
    QJsonArray parentCategoriesArray;
    for (QString parentCategory : parentCategories())
        parentCategoriesArray.append(parentCategory);
    json["parent-categories"] = parentCategoriesArray;
}

// MyComplexNodeStyle

MyComplexNodeStyle::MyComplexNodeStyle(const QString& name) : MyNodeStyleBase(name) {
    _convertibleParentCategory = "Node";
}

MyNodeStyleBase::NODE_STYLE_TYPE MyComplexNodeStyle::nodeStyleType() {
    return COMPLEX_NODE_STYLE;
}

void MyComplexNodeStyle::addDefaultShapeStyle() {
    _shapeStyles.push_back(createNodeEllipseStyle("ellipse"));
}

MyShapeStyleBase* MyComplexNodeStyle::createShapeStyle(const QString& shape) {
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


const QString& MyComplexNodeStyle::convertibleParentCategory() const {
    return _convertibleParentCategory;
}

bool MyComplexNodeStyle::isConvertibleToParentCategory(QList<QString> parentCategories) {
    for (QString parentCategory : parentCategories) {
        if (parentCategory == _convertibleParentCategory)
            return true;
    }

    return false;
}

void MyComplexNodeStyle::convertToParentCategory() {
    _category = convertibleParentCategory();
}

QWidget* MyComplexNodeStyle::addRemoveShapeStylesButtons() {
    QWidget* addRemoveShapeStylesButtons = new MyAddRemoveNodeShapeStylesButtons();
    ((MyAddRemoveNodeShapeStylesButtons*)addRemoveShapeStylesButtons)->setAddingMenu();
    return addRemoveShapeStylesButtons;
}

void MyComplexNodeStyle::read(const QJsonObject &json) {
    MyNodeStyleBase::read(json);
    _convertibleParentCategory.clear();
    if (json.contains("convertible-parent-category") && json["convertible-parent-category"].isString())
        _convertibleParentCategory = json["convertible-parent-category"].toString();
}

void MyComplexNodeStyle::write(QJsonObject &json) {
    MyNodeStyleBase::write(json);
    json["convertible-parent-category"] = convertibleParentCategory();
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

// MyAddRemoveNodeShapeStylesButtons

MyAddRemoveNodeShapeStylesButtons::MyAddRemoveNodeShapeStylesButtons(QWidget* parent) : MyAddRemoveShapeStylesButtonsBase(parent) {

}

void MyAddRemoveNodeShapeStylesButtons::setAddingMenu() {
    connect(_addingMenu->addAction("ellipse"), &QAction::triggered, this, [this] () {
        emit askForAddShapeStyle(createNodeEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("rect"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createNodeRectStyle("rect")); });
    connect(_addingMenu->addAction("polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createNodeDefaultPolygonStyle("polygon");
        emit askForAddShapeStyle(polygonShapeStyle); });
    connect(_addingMenu->addAction("text"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createTextStyle("text")); });
}
