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
    // border-width
    parameter = findParameter("border-width");
    if (parameter)
        pen.setWidth(((MyBorderWidthParameter*)parameter)->defaultValue());

    // border-color
    parameter = findParameter("border-color");
    if (parameter)
        pen.setColor(QColor(((MyBorderColorParameter*)parameter)->defaultValue()));

    return pen;
}

const QPen My1DShapeStyleBase::selectedPen() const {
    QPen pen;

    MyParameterBase* parameter = NULL;
    // border-width
    parameter = findParameter("border-width");
    if (parameter)
        pen.setWidth(((MyBorderWidthParameter*)parameter)->defaultValue());

    // border-color
    pen.setColor(QColor("#0000CD"));

    return pen;
}

void My1DShapeStyleBase::updatePen(const QPen& pen) {
    MyParameterBase* parameter = NULL;
    // border-width
    parameter = findParameter("border-width");
    if (parameter)
        ((MyBorderWidthParameter*)parameter)->setDefaultValue(pen.width());

    // border-color
    parameter = findParameter("border-color");
    if (parameter)
        ((MyBorderColorParameter*)parameter)->setDefaultValue(pen.color().name());
}

void My1DShapeStyleBase::update(MyShapeStyleBase* shapeStyle) {
    if (dynamic_cast<My1DShapeStyleBase*>(shapeStyle))
        updatePen(((My1DShapeStyleBase*)shapeStyle)->pen());
}

void My1DShapeStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // border-width
    if (json.contains("border-width") && json["border-width"].isDouble()) {
        parameter = findParameter("border-width");
        if (parameter)
            ((MyBorderWidthParameter*)parameter)->setDefaultValue(json["border-width"].toInt());
    }

    // border-color
    if (json.contains("border-color") && json["border-color"].isString()) {
        parameter = findParameter("border-color");
        if (parameter)
            ((MyBorderColorParameter*)parameter)->setDefaultValue(json["border-color"].toString());
    }
}

void My1DShapeStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // border-width
    parameter = findParameter("border-width");
    if (parameter)
        json["border-width"] = ((MyBorderWidthParameter*)parameter)->defaultValue();

    // border-color
    parameter = findParameter("border-color");
    if (parameter)
        json["border-color"] = ((MyBorderColorParameter*)parameter)->defaultValue();
}
