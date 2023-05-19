#include "negui_1d_shape_style_base.h"
#include <QJsonObject>

// My1DShapeStyleBase

My1DShapeStyleBase::My1DShapeStyleBase(const QString& name) : MyShapeStyleBase(name) {

}

My1DShapeStyleBase::~My1DShapeStyleBase() {

}

const QPen My1DShapeStyleBase::pen() const {
    QPen pen;

    MyParameterBase* parameter = NULL;
    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        pen.setWidth(((MyStrokeWidthParameter*)parameter)->defaultValue());

    // stroke
    parameter = findParameter("stroke");
    if (parameter)
        pen.setColor(QColor(((MyStrokeParameter*)parameter)->defaultValue()));

    return pen;
}

const QPen My1DShapeStyleBase::selectedPen() const {
    QPen pen;

    MyParameterBase* parameter = NULL;
    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        pen.setWidth(((MyStrokeWidthParameter*)parameter)->defaultValue());

    // stroke
    pen.setColor(QColor("#0000CD"));

    return pen;
}

void My1DShapeStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // stroke-width
    if (json.contains("stroke-width") && json["stroke-width"].isDouble()) {
        parameter = findParameter("stroke-width");
        if (parameter)
            ((MyStrokeWidthParameter*)parameter)->setDefaultValue(json["stroke-width"].toInt());
    }

    // stroke
    if (json.contains("stroke") && json["stroke"].isString()) {
        parameter = findParameter("stroke");
        if (parameter)
            ((MyStrokeParameter*)parameter)->setDefaultValue(json["stroke"].toString());
    }
}

void My1DShapeStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        json["stroke-width"] = ((MyStrokeWidthParameter*)parameter)->defaultValue();

    // stroke
    parameter = findParameter("stroke");
    if (parameter)
        json["stroke"] = ((MyStrokeParameter*)parameter)->defaultValue();
}
