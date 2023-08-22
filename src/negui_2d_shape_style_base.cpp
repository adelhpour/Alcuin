#include "negui_2d_shape_style_base.h"
#include <QJsonObject>

// My2DShapeStyleBase

My2DShapeStyleBase::My2DShapeStyleBase(const QString& name) : My1DShapeStyleBase(name) {

}

My2DShapeStyleBase::~My2DShapeStyleBase() {

}

const QBrush My2DShapeStyleBase::brush() const {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    MyParameterBase* parameter = findParameter("fill-color");
    if (parameter)
        brush.setColor(QColor(((MyFillColorParameter*)parameter)->defaultValue()));

    return brush;
}

const QBrush My2DShapeStyleBase::selectedBrush() const {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    return brush;
}

void My2DShapeStyleBase::read(const QJsonObject &json) {
    My1DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // fill-color
    if (json.contains("fill-color") && json["fill-color"].isString()) {
        parameter = findParameter("fill-color");
        if (parameter)
            ((MyFillColorParameter*)parameter)->setDefaultValue(json["fill-color"].toString());
    }
}

void My2DShapeStyleBase::write(QJsonObject &json) {
    My1DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // fill-color
    parameter = findParameter("fill-color");
    if (parameter)
        json["fill-color"] = ((MyFillColorParameter*)parameter)->defaultValue();
}
