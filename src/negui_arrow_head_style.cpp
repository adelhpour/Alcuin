#include "negui_arrow_head_style.h"
#include "negui_shape_style_builder.h"
#include "negui_network_element_icon_builder.h"

// MyArrowHeadStyleBase

MyArrowHeadStyleBase::MyArrowHeadStyleBase(const QString& name) : MyNetworkElementStyleBase(name) {
    _category = "ArrowHead";
}

const QString MyArrowHeadStyleBase::type() const {
    return "arrowheadstyle";
}

QObject* MyArrowHeadStyleBase::createIconBuilder() {
    return new MyArrowHeadIconBuilder(this);
}

const QString MyArrowHeadStyleBase::toolTipText() {
    return "";
}

void MyArrowHeadStyleBase::write(QJsonObject &json) {
    MyNetworkElementStyleBase::write(json);
    
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

// MyClassicArrowHeadStyle

MyClassicArrowHeadStyle::MyClassicArrowHeadStyle(const QString& name) : MyArrowHeadStyleBase(name) {

}

MyArrowHeadStyleBase::ARROW_HEAD_STYLE_TYPE MyClassicArrowHeadStyle::arrowHeadStyleType() {
    return  CLASSIC_ARROW_HEAD_STYLE;
}

void MyClassicArrowHeadStyle::addDefaultShapeStyle() {

}

MyShapeStyleBase* MyClassicArrowHeadStyle::createShapeStyle(const QString& shape) {
    if (shape == "ellipse")
        return createArrowHeadEllipseStyle(shape);
    else if (shape == "rectangle")
        return createArrowHeadRectangleStyle(shape);
    else if (shape == "polygon")
        return createArrowHeadPolygonStyle(shape);

    return NULL;
}

QWidget* MyClassicArrowHeadStyle::shapeStylesButtons() {
    QWidget* shapeStylesButtons = new MyChangeArrowHeadShapeStylesButton();
    ((MyChangeArrowHeadShapeStylesButton*)shapeStylesButtons)->setMenu();
    return shapeStylesButtons;
}

// MyChangeArrowHeadShapeStylesButton

MyChangeArrowHeadShapeStylesButton::MyChangeArrowHeadShapeStylesButton(QWidget* parent) : MyChangeShapeStylesButtonsBase(parent) {

}

void MyChangeArrowHeadShapeStylesButton::setMenu() {
    connect(_menu->addAction("Ellipse"), &QAction::triggered, this, [this] () {
        emit askForChangeShapeStyle(createArrowHeadEllipseStyle("ellipse")); });
    connect(_menu->addAction("Rectangle"), &QAction::triggered, this, [this] () { emit askForChangeShapeStyle(createArrowHeadRectangleStyle("rectangle")); });
    connect(_menu->addAction("Polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createArrowHeadDefaultPolygonStyle("polygon");
        emit askForChangeShapeStyle(polygonShapeStyle); });
}

// MyAddRemoveArrowHeadShapeStylesButtons

MyAddRemoveArrowHeadShapeStylesButtons::MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent) : MyAddRemoveShapeStylesButtonsBase(parent) {
    
}

void MyAddRemoveArrowHeadShapeStylesButtons::setAddingMenu() {
    connect(_addingMenu->addAction("ellipse"), &QAction::triggered, this, [this] () {
        emit askForAddShapeStyle(createArrowHeadEllipseStyle("ellipse")); });
    connect(_addingMenu->addAction("rectangle"), &QAction::triggered, this, [this] () { emit askForAddShapeStyle(createArrowHeadRectangleStyle("rectangle")); });
    connect(_addingMenu->addAction("polygon"), &QAction::triggered, this, [this] () { MyShapeStyleBase* polygonShapeStyle = createArrowHeadDefaultPolygonStyle("polygon");
        emit askForAddShapeStyle(polygonShapeStyle); });
}
