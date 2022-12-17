#include "negui_node_style.h"
#include "negui_shape_style_builder.h"
#include "negui_element_graphics_item_builder.h"

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

const QString MyNodeStyle::toolTipText() {
    return "Add " + category();
}

const QString MyNodeStyle::alternativeToolTipText() {
    return "Add " + category();
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

