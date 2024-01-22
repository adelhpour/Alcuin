#include "negui_ellipse_style.h"
#include <QJsonObject>

// MyEllipseStyleBase

MyEllipseStyleBase::MyEllipseStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // border-width
    addParameter(new MyBorderWidthParameter());
    
    // border-color
    addParameter(new MyBorderColorParameter());
    
    // fill-color
    addParameter(new MyFillColorParameter());

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyEllipseStyleBase::type() {
    return ELLIPSE_SHAPE_STYLE;
}

const QRectF MyEllipseStyleBase::getShapeExtents() {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    if (centerX() - radiusX() < extents.x())
        extents.setX(centerX() - radiusX());
    if (centerY() - radiusY() < extents.y())
        extents.setY(centerY() - radiusY());
    if (2 * radiusX() > extents.width())
        extents.setWidth(2 * radiusX());
    if (2 * radiusY() > extents.height())
        extents.setHeight(2 * radiusY());
    
    return extents;
}

void MyEllipseStyleBase::updateShapeExtents(const QRectF& extents) {
    setCenterX(extents.center().x());
    setCenterY(extents.center().y());
    setRadiusX(0.5 * extents.width());
    setRadiusY(0.5 * extents.height());
}

void MyEllipseStyleBase::setCenterX(const qreal& centerX) const {
    MyParameterBase* parameter = findParameter("center-x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(centerX);
}

const qreal MyEllipseStyleBase::centerX() const {
    MyParameterBase* parameter = findParameter("center-x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setCenterY(const qreal& centerY) const {
    MyParameterBase* parameter = findParameter("center-y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(centerY);
}

const qreal MyEllipseStyleBase::centerY() const {
    MyParameterBase* parameter = findParameter("center-y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setRadiusX(const qreal& radiusX) const {
    MyParameterBase* parameter = findParameter("radius-x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(radiusX);
}

const qreal MyEllipseStyleBase::radiusX() const {
    MyParameterBase* parameter = findParameter("radius-x");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setRadiusY(const qreal& radiusY) const {
    MyParameterBase* parameter = findParameter("radius-y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(radiusY);
}

const qreal MyEllipseStyleBase::radiusY() const {
    MyParameterBase* parameter = findParameter("radius-y");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::read(const QJsonObject &json) {
    My2DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // center-x
    if (json.contains("center-x") && json["center-x"].isDouble()) {
        parameter = findParameter("center-x");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["center-x"].toDouble());
    }
    
    // center-y
    if (json.contains("center-y") && json["center-y"].isDouble()) {
        parameter = findParameter("center-y");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["center-y"].toDouble());
    }
    
    // radius-x
    if (json.contains("radius-x") && json["radius-x"].isDouble()) {
        parameter = findParameter("radius-x");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["radius-x"].toDouble());
    }
    
    // radius-y
    if (json.contains("radius-y") && json["radius-y"].isDouble()) {
        parameter = findParameter("radius-y");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["radius-y"].toDouble());
    }
}

void MyEllipseStyleBase::write(QJsonObject &json) {
    My2DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // center-x
    parameter = findParameter("center-x");
    if (parameter)
        json["center-x"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // center-y
    parameter = findParameter("center-y");
    if (parameter)
        json["center-y"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // radius-x
    parameter = findParameter("radius-x");
    if (parameter)
        json["radius-x"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // radius-y
    parameter = findParameter("radius-y");
    if (parameter)
        json["radius-y"] = ((MyDimensionalParameter*)parameter)->defaultValue();
}

// MyNodeEllipseStyle

MyNodeEllipseStyle::MyNodeEllipseStyle(const QString& name) : MyEllipseStyleBase(name) {
    // center-x
    addHiddenParameter(new MyNodeEllipsePositionalParameter("center-x", "Relative center of ellipse along x-axis"));
    
    // center-y
    addHiddenParameter(new MyNodeEllipsePositionalParameter("center-y", "Relative center of ellipse along y-axis"));
    
    // radius-x
    addParameter(new MyNodeEllipseDimensionalParameter("radius-x", "Radius of ellipse along x-axis"));
    
    // radius-y
    addParameter(new MyNodeEllipseDimensionalParameter("radius-y", "Radius of ellipse along y-axis"));
    
    reset();
}

// MyArrowHeadEllipseStyle

MyArrowHeadEllipseStyle::MyArrowHeadEllipseStyle(const QString& name) : MyEllipseStyleBase(name) {
    // center-x
    addHiddenParameter(new MyArrowHeadEllipseCenterXParameter());
    
    // center-y
    addHiddenParameter(new MyArrowHeadEllipseCenterYParameter());
    
    // radius-x
    addParameter(new MyArrowHeadEllipseDimensionalParameter("radius-x", "Radius of ellipse along x-axis"));
    
    // radius-y
    addParameter(new MyArrowHeadEllipseDimensionalParameter("radius-y", "Radius of ellipse along y-axis"));
    
    reset();
}
