#include "negui_rectangle_style.h"
#include <QJsonObject>

// MyRectangleStyleBase

MyRectangleStyleBase::MyRectangleStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // border-width
    addParameter(new MyBorderWidthParameter());
    
    // border-color
    addParameter(new MyBorderColorParameter());
    
    // fill-color
    addParameter(new MyFillColorParameter());

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyRectangleStyleBase::type() {
    return RECTANGLE_SHAPE_STYLE;
}

const QRectF MyRectangleStyleBase::getShapeExtents() {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    if (x() < extents.x())
        extents.setX(x());
    if (y() < extents.y())
        extents.setY(y());
    if (width() > extents.width())
        extents.setWidth(width());
    if (height() > extents.height())
        extents.setHeight(height());
    
    return extents;
}

void MyRectangleStyleBase::updateShapeExtents(const QRectF& extents) {
    setX(extents.x());
    setY(extents.y());
    setWidth(extents.width());
    setHeight(extents.height());
}

void MyRectangleStyleBase::setX(const qreal& x) const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(x);
}

const qreal MyRectangleStyleBase::x() const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setY(const qreal& y) const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(y);
}

const qreal MyRectangleStyleBase::y() const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setWidth(const qreal& width) const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(width);
}

const qreal MyRectangleStyleBase::width() const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setHeight(const qreal& height) const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(height);
}

const qreal MyRectangleStyleBase::height() const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setBorderRadiusX(const qreal& borderRadiusX) const {
    MyParameterBase* parameter = findParameter("border-radius-x");
    if (parameter)
        return ((MyBorderRadiusParameter*)parameter)->setDefaultValue(borderRadiusX);
}

const qreal MyRectangleStyleBase::borderRadiusX() const {
    MyParameterBase* parameter = findParameter("border-radius-x");
    if (parameter)
        return ((MyBorderRadiusParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setBorderRadiusY(const qreal& borderRadiusY) const {
    MyParameterBase* parameter = findParameter("border-radius-y");
    if (parameter)
        return ((MyBorderRadiusParameter*)parameter)->setDefaultValue(borderRadiusY);
}

const qreal MyRectangleStyleBase::borderRadiusY() const {
    MyParameterBase* parameter = findParameter("border-radius-y");
    if (parameter)
        return ((MyBorderRadiusParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::read(const QJsonObject &json) {
    My2DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // x
    if (json.contains("x") && json["x"].isDouble()) {
        parameter = findParameter("x");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["x"].toDouble());
    }
    
    // y
    if (json.contains("y") && json["y"].isDouble()) {
        parameter = findParameter("y");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["y"].toDouble());
    }
    
    // width
    if (json.contains("width") && json["width"].isDouble()) {
        parameter = findParameter("width");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["width"].toDouble());
    }
    
    // height
    if (json.contains("height") && json["height"].isDouble()) {
        parameter = findParameter("height");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["height"].toDouble());
    }
    
    // border-radius-x
    if (json.contains("border-radius-x") && json["border-radius-x"].isDouble()) {
        parameter = findParameter("border-radius-x");
        if (parameter)
            ((MyBorderRadiusParameter*)parameter)->setDefaultValue(json["border-radius-x"].toDouble());
    }
    
    // border-radius-y
    if (json.contains("border-radius-y") && json["border-radius-y"].isDouble()) {
        parameter = findParameter("border-radius-y");
        if (parameter)
            ((MyBorderRadiusParameter*)parameter)->setDefaultValue(json["border-radius-y"].toDouble());
    }
}

void MyRectangleStyleBase::write(QJsonObject &json) {
    My2DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // x
    parameter = findParameter("x");
    if (parameter)
        json["x"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // y
    parameter = findParameter("y");
    if (parameter)
        json["y"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // width
    parameter = findParameter("width");
    if (parameter)
        json["width"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // height
    parameter = findParameter("height");
    if (parameter)
        json["height"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // border-radius-x
    parameter = findParameter("border-radius-x");
    if (parameter)
        json["border-radius-x"] = ((MyBorderRadiusParameter*)parameter)->defaultValue();
    
    // border-radius-y
    parameter = findParameter("border-radius-y");
    if (parameter)
        json["border-radius-y"] = ((MyBorderRadiusParameter*)parameter)->defaultValue();
}

// MyNodeRectangleStyle

MyNodeRectangleStyle::MyNodeRectangleStyle(const QString& name) : MyRectangleStyleBase(name) {
    // x
    addParameter(new MyNodeRectanglePositionalParameter("x", "Relative \"X\" value of rectangle bounding box"));
    
    // y
    addParameter(new MyNodeRectanglePositionalParameter("y", "Relative \"Y\" value of rectangle bounding box"));
    
    // width
    addParameter(new MyNodeRectangleDimensionalParameter("width", "\"Width\" value of rectangle bounding box"));
    
    // height
    addParameter(new MyNodeRectangleDimensionalParameter("height", "\"Height\" value of rectangle bounding box"));

    // border-radius-x
    addParameter(new MyBorderRadiusParameter("border-radius-x", "Border radius along x-axis"));

    // border-radius-y
    addParameter(new MyBorderRadiusParameter("border-radius-y", "Border radius along y-axis"));
    
    reset();
}

// MyArrowHeadRectangleStyle

MyArrowHeadRectangleStyle::MyArrowHeadRectangleStyle(const QString& name) : MyRectangleStyleBase(name) {
    // x
    addParameter(new MyArrowHeadRectangleXParameter());
    
    // y
    addParameter(new MyArrowHeadRectangleYParameter());
    
    // width
    addParameter(new MyArrowHeadRectangleDimensionalParameter("width", "\"Width\" value of rectangle bounding box"));
    
    // height
    addParameter(new MyArrowHeadRectangleDimensionalParameter("height", "\"Height\" value of rectangle bounding box"));

    // border-radius-x
    addParameter(new MyBorderRadiusParameter("border-radius-x", "Border radius along x-axis"));

    // border-radius-y
    addParameter(new MyBorderRadiusParameter("border-radius-y", "Border radius along y-axis"));
    
    reset();
}
