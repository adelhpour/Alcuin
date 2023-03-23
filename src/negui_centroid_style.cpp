#include "negui_centroid_style.h"
#include <QJsonObject>

// MyCentroidStyleBase

MyCentroidStyleBase::MyCentroidStyleBase(const QString& name) : MyShapeStyleBase(name) {

}

MyShapeStyleBase::SHAPE_STYLE MyCentroidStyleBase::type() {
    return CENTROID_SHAPE_STYLE;
}

const QRectF MyCentroidStyleBase::getShapeExtents() {
    return QRectF(0.0, 0.0, 2 * radius(), 2 * radius());
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

#include "iostream"
void MyCentroidStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // radius
    if (json.contains("radius") && json["radius"].isDouble()) {
        parameter = findParameter("radius");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["radius"].toDouble());
    }
}

void MyCentroidStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // radius
    parameter = findParameter("radius");
    if (parameter)
        json["radius"] = ((MyDimensionalParameter*)parameter)->defaultValue();
}

// MyNodeCentroidStyle

MyNodeCentroidStyle::MyNodeCentroidStyle(const QString& name) : MyCentroidStyleBase(name) {
    // radius
    _parameters.push_back(new MyNodeCentroidRadiusParameter("radius"));

    // stroke-width
    _parameters.push_back(new MyCentroidStrokeWidthParameter());

    // stroke
    _parameters.push_back(new MyCentroidStrokeParameter());

    // fill
    _parameters.push_back(new MyCentroidFillParameter());

    reset();
}
