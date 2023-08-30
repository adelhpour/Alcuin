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
    
    // rx
    addParameter(new MyCornerCurvatureParameter("rx"));
    
    // ry
    addParameter(new MyCornerCurvatureParameter("ry"));

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

void MyRectangleStyleBase::setRx(const qreal& rx) const {
    MyParameterBase* parameter = findParameter("rx");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(rx);
}

const qreal MyRectangleStyleBase::rx() const {
    MyParameterBase* parameter = findParameter("rx");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectangleStyleBase::setRy(const qreal& ry) const {
    MyParameterBase* parameter = findParameter("ry");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(ry);
}

const qreal MyRectangleStyleBase::ry() const {
    MyParameterBase* parameter = findParameter("ry");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
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
    
    // rx
    if (json.contains("rx") && json["rx"].isDouble()) {
        parameter = findParameter("rx");
        if (parameter)
            ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(json["rx"].toDouble());
    }
    
    // ry
    if (json.contains("ry") && json["ry"].isDouble()) {
        parameter = findParameter("ry");
        if (parameter)
            ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(json["ry"].toDouble());
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
    
    // rx
    parameter = findParameter("rx");
    if (parameter)
        json["rx"] = ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
    // ry
    parameter = findParameter("ry");
    if (parameter)
        json["ry"] = ((MyCornerCurvatureParameter*)parameter)->defaultValue();
}

// MyNodeRectangleStyle

MyNodeRectangleStyle::MyNodeRectangleStyle(const QString& name) : MyRectangleStyleBase(name) {
    // x
    addParameter(new MyNodeRectanglePositionalParameter("x"));
    
    // y
    addParameter(new MyNodeRectanglePositionalParameter("y"));
    
    // width
    addParameter(new MyNodeRectangleDimensionalParameter("width"));
    
    // height
    addParameter(new MyNodeRectangleDimensionalParameter("height"));
    
    reset();
}

// MyArrowHeadRectangleStyle

MyArrowHeadRectangleStyle::MyArrowHeadRectangleStyle(const QString& name) : MyRectangleStyleBase(name) {
    // x
    addParameter(new MyArrowHeadRectangleXParameter());
    
    // y
    addParameter(new MyArrowHeadRectangleYParameter());
    
    // width
    addParameter(new MyArrowHeadRectangleDimensionalParameter("width"));
    
    // height
    addParameter(new MyArrowHeadRectangleDimensionalParameter("height"));
    
    reset();
}
