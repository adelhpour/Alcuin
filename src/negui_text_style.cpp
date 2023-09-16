#include "negui_text_style.h"
#include <QJsonObject>

#include "math.h"

// MyTextStyleBase

MyTextStyleBase::MyTextStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // color
    addParameter(new MyTextColorParameter());

    // font
    addParameter(new MyFontParameter());
    
    // horizontal alignment
    addParameter(new MyHorizontalAlignmentParameter());
    
    // vertical alignment
    addParameter(new MyVerticalAlignmentParameter());

    reset();
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
        color.setNamedColor(((MyColorParameterBase*)parameter)->defaultValue());
    
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

void MyTextStyleBase::updateShapeExtents(const QRectF& extents) {
    setX(extents.x());
    setY(extents.y());
    setWidth(extents.width());
    setHeight(extents.height());
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
    MyParameterBase* parameter = findParameter("horizontal-alignment");
    if (parameter)
        return ((MyHorizontalAlignmentParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignHCenter;
}

const Qt::Alignment MyTextStyleBase::verticalAlignment() const {
    MyParameterBase* parameter = findParameter("vertical-alignment");
    if (parameter)
        return ((MyVerticalAlignmentParameter*)parameter)->defaultAlignment();
    
    return Qt::AlignVCenter;
}

const qreal MyTextStyleBase::verticalPadding() const {
    QFontMetrics fontMetrics(font());
    if (height() > fontMetrics.height()) {
        if (verticalAlignment() == Qt::AlignVCenter) {
#if defined(Q_OS_WIN)
            return 0.5 * height() - 0.375 * fontMetrics.height() - calculateTopPaddingRatioToFontHeight(font().pointSize()) * fontMetrics.height();
#elif defined(Q_OS_MAC)
            return 0.5 * height() - 0.5 * fontMetrics.height() - calculateTopPaddingRatioToFontHeight(font().pointSize()) * fontMetrics.height();
#endif
        }
        else if (verticalAlignment() == Qt::AlignBottom) {
#if defined(Q_OS_WIN)
            return height() - 1.5 * fontMetrics.height();
#elif defined(Q_OS_MAC)
            return height() - 2 * fontMetrics.height();
#endif
        }
    }

    return 0.000;
}

const qreal MyTextStyleBase::calculateTopPaddingRatioToFontHeight(const qreal& pointSize) const {
    qreal topPaddingRatioToFontHeight = 1;
    if (pointSize > 3) {
        qreal numberOfStepsBaseFontSizeIsDoubled = log2(pointSize / 3.0);
        for (int i = 0; i < numberOfStepsBaseFontSizeIsDoubled; i++)
            topPaddingRatioToFontHeight -= 0.4 / pow(2, i);
    }

    return topPaddingRatioToFontHeight;
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
            ((MyColorParameterBase*)parameter)->setDefaultValue(json["color"].toString());
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
    if (json.contains("horizontal-alignment") && json["horizontal-alignment"].isString()) {
        parameter = findParameter("horizontal-alignment");
        if (parameter)
            ((MyHorizontalAlignmentParameter*)parameter)->setDefaultValue(json["horizontal-alignment"].toString());
    }
    
    // vertical alignment
    if (json.contains("vertical-alignment") && json["vertical-alignment"].isString()) {
        parameter = findParameter("vertical-alignment");
        if (parameter)
            ((MyVerticalAlignmentParameter*)parameter)->setDefaultValue(json["vertical-alignment"].toString());
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
        json["color"] = ((MyColorParameterBase*)parameter)->defaultValue();

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
    parameter = findParameter("horizontal-alignment");
    if (parameter)
        json["horizontal-alignment"] = ((MyHorizontalAlignmentParameter*)parameter)->defaultValue();
    
    // vertical alignment
    parameter = findParameter("vertical-alignment");
    if (parameter)
        json["vertical-alignment"] = ((MyVerticalAlignmentParameter*)parameter)->defaultValue();
}

// MySimpleTextStyle

MySimpleTextStyle::MySimpleTextStyle(const QString& name) : MyTextStyleBase(name) {
    // plain-text
    addOutsourcingParameter(new MyTextPlainTextParameter());

    // x
    addHiddenParameter(new MyNodeTextPositionalParameter("x", "Relative \"X\" value of text bounding box"));

    // y
    addHiddenParameter(new MyNodeTextPositionalParameter("y", "Relative \"Y\" value of text bounding box"));

    // width
    addHiddenParameter(new MyNodeTextDimensionalParameter("width", "\"Width\" value of text bounding box"));

    // height
    addHiddenParameter(new MyNodeTextDimensionalParameter("height", "\"Height\" value of text bounding box"));

    _whetherSetNameAsDefaultPlainText = true;
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MySimpleTextStyle::type() {
    return SIMPLE_TEXT_SHAPE_STYLE;
}

const bool& MySimpleTextStyle::whetherSetNameAsDefaultPlainText() const {
    return _whetherSetNameAsDefaultPlainText;
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
    // plain text
    addParameterToTheBeginningOfTheList(new MyTextPlainTextParameter());

    // x
    addParameter(new MyNodeTextPositionalParameter("x", "Relative \"X\" value of text bounding box"));

    // y
    addParameter(new MyNodeTextPositionalParameter("y", "Relative \"Y\" value of text bounding box"));

    // width
    addParameter(new MyNodeTextDimensionalParameter("width", "\"Width\" value of text bounding box"));

    // height
    addParameter(new MyNodeTextDimensionalParameter("height", "\"Height\" value of text bounding box"));

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyWithPlainTextTextStyle::type() {
    return WITH_PLAIN_TEXT_TEXT_SHAPE_STYLE;
}
