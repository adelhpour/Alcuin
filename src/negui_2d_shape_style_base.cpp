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

    MyParameterBase* parameter = findParameter("fill");
    if (parameter)
        brush.setColor(QColor(((MyFillParameter*)parameter)->defaultValue()));

    return brush;
}

const QBrush My2DShapeStyleBase::selectedBrush() const {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    return brush;
}

void My2DShapeStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // fill
    if (json.contains("fill") && json["fill"].isString()) {
        parameter = findParameter("fill");
        if (parameter)
            ((MyFillParameter*)parameter)->setDefaultValue(json["fill"].toString());
    }
}

void My2DShapeStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    // fill
    parameter = findParameter("fill");
    if (parameter)
        json["fill"] = ((MyFillParameter*)parameter)->defaultValue();
}
