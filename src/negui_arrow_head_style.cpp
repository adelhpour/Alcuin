#include "negui_arrow_head_style.h"
#include "negui_shape_style_builder.h"
#include "negui_element_icon_builder.h"

// MyArrowHeadStyle

MyArrowHeadStyle::MyArrowHeadStyle(const QString& name) : MyElementStyleBase(name) {
    _category = "ArrowHead";
    _addRemoveShapeStylesButtons = new MyAddRemoveArrowHeadShapeStylesButtons();
    ((MyAddRemoveArrowHeadShapeStylesButtons*)_addRemoveShapeStylesButtons)->setAddingMenu();
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

QObject* MyArrowHeadStyle::createIconBuilder() {
    return new MyArrowHeadIconBuilder(this);
}

const QString MyArrowHeadStyle::toolTipText() {
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

QWidget* MyArrowHeadStyle::getAddRemoveShapeStylesButtons() {
    return _addRemoveShapeStylesButtons;
}

// MyAddRemoveArrowHeadShapeStylesButtons

MyAddRemoveArrowHeadShapeStylesButtons::MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent) : MyAddRemoveShapeStylesButtonsBase(parent) {
    
}

void MyAddRemoveArrowHeadShapeStylesButtons::setAddingMenu() {
    connect(_addingMenu->addAction("ellipse"), &QAction::triggered, this, [this] () {
        emit askForAddShapeStyle(createArrowHeadEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("rect"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createArrowHeadRectStyle("rect")); });
    connect(_addingMenu->addAction("polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createArrowHeadDefaultPolygonStyle("polygon");
        emit askForAddShapeStyle(polygonShapeStyle); });
}
