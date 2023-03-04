#include "negui_node_style.h"
#include "negui_shape_style_builder.h"
#include "negui_element_icon_builder.h"
#include <QJsonArray>

// MyNodeStyle

MyNodeStyle::MyNodeStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "Node";
    _convertibleParentCategory = "Node";
    _parentCategories.push_back("Node");
    if (name == "Default")
        addDefaultShapeStyle();
    
    _addRemoveShapeStylesButtons = new MyAddRemoveNodeShapeStylesButtons();
    ((MyAddRemoveNodeShapeStylesButtons*)_addRemoveShapeStylesButtons)->setAddingMenu();
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
    else if (shape == "centroid")
        return createNodeCentroidStyle(shape);
    else if (shape == "text")
        return createTextStyle(shape);
    
    return NULL;
}

QObject* MyNodeStyle::createIconBuilder() {
    return new MyNodeIconBuilder(this);
}

const QString MyNodeStyle::toolTipText() {
    return "Add " + category();
}

const QString& MyNodeStyle::convertibleParentCategory() const {
    return _convertibleParentCategory;
}

bool MyNodeStyle::isConvertibleToParentCategory(QList<QString> parentCategories) {
    for (QString parentCategory : parentCategories) {
        if (parentCategory == _convertibleParentCategory)
            return true;
    }
    
    return false;
}

void MyNodeStyle::convertToParentCategory() {
    _category = convertibleParentCategory();
}

QList<QString> MyNodeStyle::parentCategories() {
    return _parentCategories;
}

QWidget* MyNodeStyle::getAddRemoveShapeStylesButtons() {
    return _addRemoveShapeStylesButtons;
}

void MyNodeStyle::read(const QJsonObject &json) {
    MyElementStyleBase::read(json);
    _convertibleParentCategory.clear();
    if (json.contains("convertible-parent-category") && json["convertible-parent-category"].isString())
        _convertibleParentCategory = json["convertible-parent-category"].toString();

    _parentCategories.clear();
    if (json.contains("parent-categories") && json["parent-categories"].isArray()) {
        QJsonArray parentCategoriesArray = json["parent-categories"].toArray();
        for (int parentCategoryIndex = 0; parentCategoryIndex < parentCategoriesArray.size(); ++parentCategoryIndex) {
            if (parentCategoriesArray[parentCategoryIndex].isString())
                _parentCategories.push_back(parentCategoriesArray[parentCategoryIndex].toString());
        }
    }
}

void MyNodeStyle::write(QJsonObject &json) {
    MyElementStyleBase::write(json);
    json["convertible-parent-category"] = convertibleParentCategory();
    
    QJsonArray parentCategoriesArray;
    for (QString parentCategory : parentCategories())
        parentCategoriesArray.append(parentCategory);
    json["parent-categories"] = parentCategoriesArray;
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

