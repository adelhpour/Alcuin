#include "negui_centroid_style.h"
#include <QJsonObject>

// MyCentroidStyleBase

MyCentroidStyleBase::MyCentroidStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyCentroidStyleBase::type() {
    return CENTROID_SHAPE_STYLE;
}

const QRectF MyCentroidStyleBase::getShapeExtents() {
    return QRectF(0.0, 0.0, 2 * radius(), 2 * radius());
}

void MyCentroidStyleBase::updateShapeExtents(const QRectF& extents) {
    setRadius(0.5 * (extents.width() + extents.height()));
}

void MyCentroidStyleBase::setRadius(const qreal& radius) const {
    MyParameterBase* parameter = findParameter("radius");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(radius);
}

const qreal MyCentroidStyleBase::radius() const {
    MyParameterBase* parameter = findParameter("radius");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();

    return 0.000;
}

void MyCentroidStyleBase::read(const QJsonObject &json) {
    My2DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // radius
    if (json.contains("radius") && json["radius"].isDouble()) {
        parameter = findParameter("radius");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["radius"].toDouble());
    }
}

void MyCentroidStyleBase::write(QJsonObject &json) {
    My2DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // radius
    parameter = findParameter("radius");
    if (parameter)
        json["radius"] = ((MyDimensionalParameter*)parameter)->defaultValue();
}

// MyNodeCentroidStyle

MyNodeCentroidStyle::MyNodeCentroidStyle(const QString& name) : MyCentroidStyleBase(name) {
    // radius
    addParameter(new MyNodeCentroidRadiusParameter("radius"));

    // border-width
    addParameter(new MyCentroidBorderWidthParameter());

    // border-color
    addParameter(new MyCentroidBorderColorParameter());

    // fill-color
    addParameter(new MyCentroidFillColorParameter());

    reset();
}
