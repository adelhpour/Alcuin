#include "negui_text_style.h"
#include <QJsonObject>

// MyTextStyleBase

MyTextStyleBase::MyTextStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // color
    addParameter(new MyColorParameter("color"));

    // font
    addParameter(new MyFontParameter());

    // x
    addParameter(new MyNodeTextPositionalParameter("x"));

    // y
    addParameter(new MyNodeTextPositionalParameter("y"));

    // width
    addParameter(new MyNodeTextDimensionalParameter("width"));

    // height
    addParameter(new MyNodeTextDimensionalParameter("height"));
    
    // horizontal alignment
    addParameter(new MyTextAnchorParameter());
    
    // vertical alignment
    addParameter(new MyVTextAnchorParameter());

    _whetherSetNameAsDefaultPlainText = false;
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyTextStyleBase::type() {
    return TEXT_SHAPE_STYLE;
}

const QString MyTextStyleBase::plainText() const {
    MyParameterBase* parameter = findParameter("plain-text");
    if (parameter)
        return ((MyStringParameter*)parameter)->defaultValue();

    return "";
}

void MyTextStyleBase::setPlainText(const QString& plainText) {
    MyParameterBase* parameter = findParameter("plain-text");
    if (parameter)
        ((MyStringParameter *) parameter)->setDefaultValue(plainText);
}

const QColor MyTextStyleBase::defaultTextColor() const {
    QColor color;
    
    MyParameterBase* parameter = findParameter("color");
    if (parameter)
        color.setNamedColor(((MyColorParameter*)parameter)->defaultValue());
    
    return color;
}

const QFont MyTextStyleBase::font() const {
    QFont font;

    MyParameterBase* parameter = findParameter("font");
    if (parameter)
        font = ((MyFontParameter*)parameter)->defaultValue();

    return font;
}

const QRectF MyTextStyleBase::getShapeExtents() {
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

void MyTextStyleBase::setX(const qreal& x) const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(x);
}

const qreal MyTextStyleBase::x() const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyleBase::setY(const qreal& y) const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(y);
}

const qreal MyTextStyleBase::y() const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyleBase::setWidth(const qreal& width) const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(width);
}

const qreal MyTextStyleBase::width() const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyTextStyleBase::setHeight(const qreal& height) const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(height);
}

const qreal MyTextStyleBase::height() const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

const Qt::Alignment MyTextStyleBase::horizontalAlignment() const {
    MyParameterBase* parameter = findParameter("text-anchor");
    if (parameter)
        return ((MyTextAnchorParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignHCenter;
}

const Qt::Alignment MyTextStyleBase::verticalAlignment() const {
    MyParameterBase* parameter = findParameter("vtext-anchor");
    if (parameter)
        return ((MyVTextAnchorParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignVCenter;
}

const qreal MyTextStyleBase::verticalPadding() const {
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

const bool& MyTextStyleBase::whetherSetNameAsDefaultPlainText() const {
    return _whetherSetNameAsDefaultPlainText;
}

void MyTextStyleBase::read(const QJsonObject &json) {
    My2DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;

    // plain-text
    if (json.contains("plain-text") && json["plain-text"].isString()) {
        parameter = findParameter("plain-text");
        if (parameter)
            ((MyStringParameter *) parameter)->setDefaultValue(json["plain-text"].toString());
    }
    
    // color
    if (json.contains("color") && json["color"].isString()) {
        parameter = findParameter("color");
        if (parameter)
            ((MyColorParameter*)parameter)->setDefaultValue(json["color"].toString());
    }
    
    // font-family
    if (json.contains("font-family") && json["font-family"].isString()) {
        parameter = findParameter("font");
        if (parameter)
            ((MyFontParameter*)parameter)->setDefaultFontFamilyValue(json["font-family"].toString());
    }
    
    // font-size
    if (json.contains("font-size") && json["font-size"].isDouble()) {
        parameter = findParameter("font");
        if (parameter)
            ((MyFontParameter*)parameter)->setDefaultFontSizeValue(json["font-size"].toDouble());
    }
    
    // font-weight
    if (json.contains("font-weight") && json["font-weight"].isString()) {
        parameter = findParameter("font");
        if (parameter)
            ((MyFontParameter*)parameter)->setDefaultFontWeightValue(json["font-weight"].toString());
    }
        
    // font-style
    if (json.contains("font-style") && json["font-style"].isString()) {
        parameter = findParameter("font");
        if (parameter)
            ((MyFontParameter*)parameter)->setDefaultFontStyleValue(json["font-style"].toString());
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

void MyTextStyleBase::write(QJsonObject &json) {
    My2DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;

    parameter = findParameter("plain-text");
    if (parameter)
        json["plain-text"] = ((MyStringParameter*)parameter)->defaultValue();

    // color
    parameter = findParameter("color");
    if (parameter)
        json["color"] = ((MyColorParameter*)parameter)->defaultValue();
    
    // font-family
    parameter = findParameter("font");
    if (parameter) {
        json["font-family"] = ((MyFontParameter*)parameter)->defaultFontFamilyValue();
        json["font-size"] = ((MyFontParameter*)parameter)->defaultFontSizeValue();
        json["font-weight"] = ((MyFontParameter*)parameter)->defaultFontWeightValue();
        json["font-style"] = ((MyFontParameter*)parameter)->defaultFontStyleValue();
    }
    
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

// MySimpleTextStyle

MySimpleTextStyle::MySimpleTextStyle(const QString& name) : MyTextStyleBase(name) {
    addOutsourcingParameter(new MyTextPlainTextParameter());
    reset();
    _whetherSetNameAsDefaultPlainText = true;
}

void MySimpleTextStyle::read(const QJsonObject &json) {
    MyTextStyleBase::read(json);
    _whetherSetNameAsDefaultPlainText = true;
    if (json.contains("set_name_as_default_plain_text") && json["set_name_as_default_plain_text"].isBool())
        _whetherSetNameAsDefaultPlainText = json["set_name_as_default_plain_text"].toBool();
}

void MySimpleTextStyle::write(QJsonObject &json) {
    MyTextStyleBase::write(json);
    json["set_name_as_default_plain_text"] = whetherSetNameAsDefaultPlainText();
}

// MyWithPlainTextTextStyle

MyWithPlainTextTextStyle::MyWithPlainTextTextStyle(const QString& name) : MyTextStyleBase(name) {
    addParameterToTheBeginningOfTheList(new MyTextPlainTextParameter());
    reset();
}