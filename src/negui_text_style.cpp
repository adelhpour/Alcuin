#include "negui_text_style.h"
#include <QJsonObject>

// MyTextStyle

MyTextStyle::MyTextStyle(const QString& name) : MyShapeStyleBase(name) {
    // plain-text
    _parameters.push_back(new MyStringParameter("plain-text"));
    
    // stroke
    _parameters.push_back(new MyColorParameter("color"));
    
    // font-family
    _parameters.push_back(new MyFontFamilyParameter());
    
    // font-size
    _parameters.push_back(new MyFontSizeParameter());
    
    // font-weight
    _parameters.push_back(new MyFontWeightParameter());
    
    // font-style
    _parameters.push_back(new MyFontStyleParameter());
    
    // x
    _parameters.push_back(new MyNodeTextPositionalParameter("x"));
    
    // y
    _parameters.push_back(new MyNodeTextPositionalParameter("y"));
    
    // width
    _parameters.push_back(new MyNodeTextDimensionalParameter("width"));
    
    // height
    _parameters.push_back(new MyNodeTextDimensionalParameter("height"));
    
    // horizontal alignment
    _parameters.push_back(new MyTextAnchorParameter());
    
    // vertical alignment
    _parameters.push_back(new MyVTextAnchorParameter());
    
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyTextStyle::type() {
    return TEXT_SHAPE_STYLE;
}

const QString MyTextStyle::plainText() const {
    MyParameterBase* parameter = findParameter("plain-text");
    if (parameter)
        return ((MyStringParameter*)parameter)->defaultValue();
    
    return "";
}

const QColor MyTextStyle::defaultTextColor() const {
    QColor color;
    
    MyParameterBase* parameter = findParameter("color");
    if (parameter)
        color.setNamedColor(((MyColorParameter*)parameter)->defaultValue());
    
    return color;
}

const QFont MyTextStyle::font() const {
    QFont font;
    
    MyParameterBase* parameter = NULL;
    // font-family
    parameter = findParameter("font-family");
    if (parameter)
        font.setFamily(((MyFontFamilyParameter*)parameter)->defaultValue());
    
    // font-size
    parameter = findParameter("font-size");
    if (parameter)
        font.setPointSize(((MyFontSizeParameter*)parameter)->defaultSize());
    
    // font-weight
    parameter = findParameter("font-weight");
    if (parameter)
        font.setBold(((MyFontWeightParameter*)parameter)->defaultWeight());
    
    // font-style
    parameter = findParameter("font-style");
    if (parameter)
        font.setItalic(((MyFontStyleParameter*)parameter)->defaultStyle());
    
    return font;
}

const QRectF MyTextStyle::getShapeExtents() {
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

void MyTextStyle::setX(const qreal& x) const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(x);
}

const qreal MyTextStyle::x() const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyle::setY(const qreal& y) const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(y);
}

const qreal MyTextStyle::y() const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyle::setWidth(const qreal& width) const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(width);
}

const qreal MyTextStyle::width() const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyle::setHeight(const qreal& height) const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(height);
}

const qreal MyTextStyle::height() const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

const Qt::Alignment MyTextStyle::horizontalAlignment() const {
    MyParameterBase* parameter = findParameter("text-anchor");
    if (parameter)
        return ((MyTextAnchorParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignHCenter;
}

const Qt::Alignment MyTextStyle::verticalAlignment() const {
    MyParameterBase* parameter = findParameter("vtext-anchor");
    if (parameter)
        return ((MyVTextAnchorParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignVCenter;
}

const qreal MyTextStyle::verticalPadding() const {
    QFontMetrics fontMetrics(font());
    if (height() > fontMetrics.height()) {
        if (verticalAlignment() == Qt::AlignVCenter)
            return 0.5 * height() - 0.75 * fontMetrics.height();
        else if (verticalAlignment() == Qt::AlignBaseline)
            return 0.5 * height() - 0.5 * fontMetrics.height();
        else if (verticalAlignment() == Qt::AlignBottom)
            return height() - 1.5 * fontMetrics.height();
    }
    
    return 0.000;
}

void MyTextStyle::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // plain-text
    if (json.contains("plain-text") && json["plain-text"].isString()) {
        parameter = findParameter("plain-text");
        if (parameter)
            ((MyStringParameter*)parameter)->setDefaultValue(json["plain-text"].toString());
    }
    
    // color
    if (json.contains("color") && json["color"].isString()) {
        parameter = findParameter("color");
        if (parameter)
            ((MyColorParameter*)parameter)->setDefaultValue(json["color"].toString());
    }
    
    // font-family
    if (json.contains("font-family") && json["font-family"].isString()) {
        parameter = findParameter("font-family");
        if (parameter)
            ((MyFontFamilyParameter*)parameter)->setDefaultValue(json["font-family"].toString());
    }
    
    // font-size
    if (json.contains("font-size") && json["font-size"].isDouble()) {
        parameter = findParameter("font-size");
        if (parameter)
            ((MyFontSizeParameter*)parameter)->setDefaultValue(QString::number(json["font-size"].toDouble()));
    }
    
    // font-weight
    if (json.contains("font-weight") && json["font-weight"].isString()) {
        parameter = findParameter("font-weight");
        if (parameter)
            ((MyFontWeightParameter*)parameter)->setDefaultValue(json["font-weight"].toString());
    }
        
    // font-style
    if (json.contains("font-style") && json["font-style"].isString()) {
        parameter = findParameter("font-style");
        if (parameter)
            ((MyFontStyleParameter*)parameter)->setDefaultValue(json["font-style"].toString());
    }
    
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
    
    // horizontal alignment
    if (json.contains("text-anchor") && json["text-anchor"].isString()) {
        parameter = findParameter("text-anchor");
        if (parameter)
            ((MyTextAnchorParameter*)parameter)->setDefaultValue(json["text-anchor"].toString());
    }
    
    // vertical alignment
    if (json.contains("vtext-anchor") && json["vtext-anchor"].isString()) {
        parameter = findParameter("vtext-anchor");
        if (parameter)
            ((MyVTextAnchorParameter*)parameter)->setDefaultValue(json["vtext-anchor"].toString());
    }
}

void MyTextStyle::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // plain-text
    parameter = findParameter("plain-text");
    if (parameter)
        json["plain-text"] = ((MyStringParameter*)parameter)->defaultValue();
    
    // color
    parameter = findParameter("color");
    if (parameter)
        json["color"] = ((MyColorParameter*)parameter)->defaultValue();
    
    // font-family
    parameter = findParameter("font-family");
    if (parameter)
        json["font-family"] = ((MyFontFamilyParameter*)parameter)->defaultValue();
    
    // font-size
    parameter = findParameter("font-size");
    if (parameter)
        json["font-size"] = ((MyFontSizeParameter*)parameter)->defaultValue().toDouble();
    
    // font-weight
    parameter = findParameter("font-weight");
    if (parameter)
        json["font-weight"] = ((MyFontWeightParameter*)parameter)->defaultValue();
    
    // font-style
    parameter = findParameter("font-style");
    if (parameter)
        json["font-style"] = ((MyFontStyleParameter*)parameter)->defaultValue();
    
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
    
    // horizontal alignment
    parameter = findParameter("text-anchor");
    if (parameter)
        json["text-anchor"] = ((MyTextAnchorParameter*)parameter)->defaultValue();
    
    // vertical alignment
    parameter = findParameter("vtext-anchor");
    if (parameter)
        json["vtext-anchor"] = ((MyVTextAnchorParameter*)parameter)->defaultValue();
}
