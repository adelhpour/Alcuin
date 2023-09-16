#include "negui_parameters.h"
#include "negui_customized_feature_menu_widgets.h"

#include <QJsonArray>
#include <QGridLayout>

// MyParameterBase

MyParameterBase::MyParameterBase(const QString& name, const QString& hint) : MyBase(name) {
    _inputWidget = NULL;
    _hint = hint;
}

const QString MyParameterBase::hint() {
    return _hint;
}

QWidget* MyParameterBase::inputWidget() {
    _inputWidget = createInputWidget();
    updateInputWidget();
    connectInputWidget();
    return _inputWidget;
}

// MyDoubleParameter

MyDoubleParameter::MyDoubleParameter(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyDoubleParameter::type() {
    return DOUBLE_PARAMETER_TYPE;
}

void MyDoubleParameter::setDefaultValue(const qreal& value) {
    if (value >= min() && value <= max()) {
        _defaultValue = value;
        _isSetDefaultValue = true;
    }
}

void MyDoubleParameter::setDefaultValue() {
    setDefaultValue(((MyDoubleSpinBox*)_inputWidget)->value());
}

const qreal& MyDoubleParameter::defaultValue() const {
    return _defaultValue;
}

void MyDoubleParameter::setMin(const qreal& min) {
    _min = min;
    _isSetMin = true;
}

const qreal& MyDoubleParameter::min() const {
    return _min;
}

void MyDoubleParameter::setMax(const qreal& max) {
    _max = max;
    _isSetMax = true;
}

const qreal& MyDoubleParameter::max() const {
    return _max;
}

void MyDoubleParameter::setStep(const qreal& step) {
    _step = step;
    _isSetStep = true;
}

const qreal& MyDoubleParameter::step() const {
    return _step;
}

QWidget* MyDoubleParameter::createInputWidget() {
    return new MyDoubleSpinBox();
}

void MyDoubleParameter::updateInputWidget() {
    ((MyDoubleSpinBox*)_inputWidget)->setMinimum(min());
    ((MyDoubleSpinBox*)_inputWidget)->setMaximum(max());
    ((MyDoubleSpinBox*)_inputWidget)->setSingleStep(step());
    ((MyDoubleSpinBox*)_inputWidget)->setValue(defaultValue());
    QStringList stepNumbers = QString::number(step()).split('.');
    if (stepNumbers.size() == 2)
        ((MyDoubleSpinBox*)_inputWidget)->setDecimals(stepNumbers.at(1).size());
}

void MyDoubleParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(valueChanged(double)), this, SIGNAL(isUpdated()));
}

void MyDoubleParameter::reset() {
    _defaultValue = 0.0;
    _min = 0.0;
    _max = 1.0;
    _step = 1.0;
    _isSetDefaultValue = false;
    _isSetMin = false;
    _isSetMax = false;
    _isSetStep = false;
}

void MyDoubleParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // min
        if (json.contains("min") && json["min"].isDouble())
            setMin(json["min"].toDouble());

        // max
        if (json.contains("max") && json["max"].isDouble())
            setMax(json["max"].toDouble());

        // step
        if (json.contains("step") && json["step"].isDouble())
            setStep(json["step"].toDouble());

        // default value
        if (json.contains("default") && json["default"].isDouble())
            setDefaultValue(json["default"].toDouble());
    }
}

void MyDoubleParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyDoubleSpinBox*)_inputWidget)->value();
    }
}

// MyIntegerParameter

MyIntegerParameter::MyIntegerParameter(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyIntegerParameter::type() {
    return INTEGER_PARAMETER_TYPE;
}

void MyIntegerParameter::setDefaultValue(const qint32& value) {
    if (value >= min() && value <= max()) {
        _defaultValue = value;
        _isSetDefaultValue = true;
    }
}

void MyIntegerParameter::setDefaultValue() {
    setDefaultValue(((MySpinBox*)_inputWidget)->value());
}

const qint32& MyIntegerParameter::defaultValue() const {
    return _defaultValue;
}

void MyIntegerParameter::setMin(const qint32& min) {
    _min = min;
    _isSetMin = true;
}

const qint32& MyIntegerParameter::min() const {
    return _min;
}

void MyIntegerParameter::setMax(const qint32& max) {
    _max = max;
    _isSetMax = true;
}

const qint32& MyIntegerParameter::max() const {
    return _max;
}

void MyIntegerParameter::setStep(const qint32& step) {
    _step = step;
    _isSetStep = true;
}

const qint32& MyIntegerParameter::step() const {
    return _step;
}

QWidget* MyIntegerParameter::createInputWidget() {
    return new MySpinBox();
}

void MyIntegerParameter::updateInputWidget() {
    ((MySpinBox*)_inputWidget)->setMinimum(min());
    ((MySpinBox*)_inputWidget)->setMaximum(max());
    ((MySpinBox*)_inputWidget)->setSingleStep(step());
    ((MySpinBox*)_inputWidget)->setValue(defaultValue());
}

void MyIntegerParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(valueChanged(int)), this, SIGNAL(isUpdated()));
}

void MyIntegerParameter::reset() {
    _defaultValue = 0;
    _min = 0;
    _max = 1;
    _step = 1;
    _isSetDefaultValue = false;
    _isSetMin = false;
    _isSetMax = false;
    _isSetStep = false;
}

void MyIntegerParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // min
        if (json.contains("min") && json["min"].isDouble())
            setMin(json["min"].toInt());

        // max
        if (json.contains("max") && json["max"].isDouble())
            setMax(json["max"].toInt());

        // step
        if (json.contains("step") && json["step"].isDouble())
            setStep(json["step"].toInt());

        // default value
        if (json.contains("default") && json["default"].isDouble())
            setDefaultValue(json["default"].toInt());
    }
}

void MyIntegerParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyDoubleSpinBox*)_inputWidget)->value();
    }
}

// MyBooleanParameter

MyBooleanParameter::MyBooleanParameter(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyBooleanParameter::type() {
    return BOOLEAN_PARAMETER_TYPE;
}

void MyBooleanParameter::setDefaultValue(const bool& value) {
    _defaultValue = value;
    _isSetDefaultValue = true;
}

void MyBooleanParameter::setDefaultValue() {
    if (((MyComboBox*)_inputWidget)->currentText() == "true")
        setDefaultValue(true);
    else
        setDefaultValue(false);
}

const bool& MyBooleanParameter::defaultValue() const {
    return _defaultValue;
}

QWidget* MyBooleanParameter::createInputWidget() {
    return new MyComboBox();
}

void MyBooleanParameter::updateInputWidget() {
    ((MyComboBox*)_inputWidget)->clear();
    ((MyComboBox*)_inputWidget)->addItem("true");
    ((MyComboBox*)_inputWidget)->addItem("false");
    if (defaultValue())
        ((MyComboBox*)_inputWidget)->setCurrentText("true");
    else
        ((MyComboBox*)_inputWidget)->setCurrentText("false");
}

void MyBooleanParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(currentIndexChanged(int)), this, SIGNAL(isUpdated()));
}

void MyBooleanParameter::reset() {
    _defaultValue = true;
    _isSetDefaultValue = false;
}

void MyBooleanParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // default value
        if (json.contains("default") && json["default"].isBool())
            setDefaultValue(json["default"].toBool());
    }
}

void MyBooleanParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        if (((MyComboBox*)_inputWidget)->currentText() == "true")
            json["value"] = true;
        else if (((MyComboBox*)_inputWidget)->currentText() == "false")
            json["value"] = false;
    }
}

// MyStringParameter

MyStringParameter::MyStringParameter(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyStringParameter::type() {
    return STRING_PARAMETER_TYPE;
}

void MyStringParameter::setDefaultValue(const QString& value) {
    _defaultValue = value;
    _isSetDefaultValue = true;
}

void MyStringParameter::setDefaultValue() {
    if (_inputWidget)
        setDefaultValue(((MyLineEdit*)_inputWidget)->text());
}

const QString& MyStringParameter::defaultValue() const {
    return _defaultValue;
}

QWidget* MyStringParameter::createInputWidget() {
    return new MyLineEdit();
}

void MyStringParameter::updateInputWidget() {
    ((MyLineEdit*)_inputWidget)->setText(defaultValue());
}

void MyStringParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(editingFinished()), this, SIGNAL(isUpdated()));
}

void MyStringParameter::reset() {
    _defaultValue = "";
    _isSetDefaultValue = false;
}

void MyStringParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // default value
        if (json.contains("default") && json["default"].isString())
            setDefaultValue(json["default"].toString());
    }
}

void MyStringParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((QLineEdit*)_inputWidget)->text();
    }
}

// MyNominalParameter

MyNominalParameter::MyNominalParameter(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyNominalParameter::type() {
    return NOMINAL_PARAMETER_TYPE;
}

void MyNominalParameter::setDefaultValue(const QString& value) {
    for (QString item : qAsConst(items())) {
        if (value == item) {
            _defaultValue = value;
            _isSetDefaultValue = true;
            break;
        }
    }
}

void MyNominalParameter::setDefaultValue() {
    setDefaultValue(((MyComboBox*)_inputWidget)->currentText());
}

const QString& MyNominalParameter::defaultValue() const {
    return _defaultValue;
}

const QStringList& MyNominalParameter::items() const {
    return _items;
}

QWidget* MyNominalParameter::createInputWidget() {
    return new MyComboBox();
}

void MyNominalParameter::updateInputWidget() {
    ((MyComboBox*)_inputWidget)->clear();
    ((MyComboBox*)_inputWidget)->addItems(items());
    ((MyComboBox*)_inputWidget)->setCurrentText(defaultValue());
}

void MyNominalParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(currentIndexChanged(int)), this, SIGNAL(isUpdated()));
}

void MyNominalParameter::reset() {
    _defaultValue = "";
    _isSetDefaultValue = false;
    _items.clear();
}

void MyNominalParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();
        // items
        if (json.contains("items") && json["items"].isArray()) {
            QJsonArray itemsArray = json["items"].toArray();
            for (int itemIndex = 0; itemIndex < itemsArray.size(); ++itemIndex) {
                if (itemsArray[itemIndex].isString())
                    _items.push_back(itemsArray[itemIndex].toString());
            }
        }
        // default value
        if (json.contains("default") && json["default"].isString())
            setDefaultValue(json["default"].toString());
    }
}

void MyNominalParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyComboBox*)_inputWidget)->currentText();
    }
}

// MyPointParameterBase

MyPointParameterBase::MyPointParameterBase(const QString& name, const QString& hint) : MyParameterBase(name, hint) {

}

MyParameterBase::PARAMETER_TYPE MyPointParameterBase::type() {
    return POINT_PARAMETER_TYPE;
}

void MyPointParameterBase::setDefaultValueX(const qreal& x) {
    ((MyPositionalParameter*)_x)->setDefaultValue(x);
}

const qreal& MyPointParameterBase::defaultValueX() const {
    return ((MyPositionalParameter*)_x)->defaultValue();
}

void MyPointParameterBase::setDefaultValueY(const qreal& y) {
    ((MyPositionalParameter*)_y)->setDefaultValue(y);
}

const qreal& MyPointParameterBase::defaultValueY() const {
    return ((MyPositionalParameter*)_y)->defaultValue();
}

void MyPointParameterBase::setDefaultValue() {
    ((MyPositionalParameter*)_x)->setDefaultValue();
    ((MyPositionalParameter*)_y)->setDefaultValue();
}

QWidget* MyPointParameterBase::createInputWidget() {
    QWidget* widget = new QWidget();
    qint32 fixedWidth = 0;
    QGridLayout* contentLayout = new QGridLayout(widget);

    // x
    MyLabel* xLabel = new MyLabel(_x->name());
    xLabel->setFixedWidth(40);
    xLabel->setAlignment(Qt::AlignLeft);
    contentLayout->addWidget(xLabel, 0, 0);
    fixedWidth += xLabel->size().width();
    contentLayout->addWidget(_x->inputWidget(), 0 , 1);
    fixedWidth += _x->inputWidget()->size().width();

    // y
    MyLabel* yLabel = new MyLabel(_y->name());
    yLabel->setFixedWidth(40);
    yLabel->setAlignment(Qt::AlignLeft);
    contentLayout->addWidget(yLabel, 1, 0);
    fixedWidth += yLabel->size().width();
    contentLayout->addWidget(_y->inputWidget(), 1 , 1);
    fixedWidth += _y->inputWidget()->size().width();

    widget->setLayout(contentLayout);

    return widget;
}

void MyPointParameterBase::updateInputWidget() {

}

void MyPointParameterBase::connectInputWidget() {

}

void MyPointParameterBase::reset() {
    _x->reset();
    _y->reset();
}

void MyPointParameterBase::read(const QJsonObject &json) {

}

void MyPointParameterBase::write(QJsonObject &json) {

}

// MyAbsolutePointParameter

MyAbsolutePointParameter::MyAbsolutePointParameter(const QString& name, const QString& hint) : MyPointParameterBase(name, hint) {
    _x = new MyPositionalParameter("x");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyPositionalParameter("y");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyRelativePointParameter

MyRelativePointParameter::MyRelativePointParameter(const QString& name, const QString& hint) : MyPointParameterBase(name, hint) {
    _x = new MyRelativePositionalParameter("x (%)");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyRelativePositionalParameter("y (%)");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyControlPointParameter

MyControlPointParameter::MyControlPointParameter(const QString& name, const QString& hint) : MyPointParameterBase(name, hint) {
    _x = new MyPositionalParameter("x (%)");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyPositionalParameter("y (%)");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyColorParameterBase

MyColorParameterBase::MyColorParameterBase(const QString& name, const QString& hint) : MyParameterBase(name, hint) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyColorParameterBase::type() {
    return COLOR_PARAMETER_TYPE;
}

void MyColorParameterBase::setDefaultValue(const QString& value) {
    _defaultValue = value;
}

void MyColorParameterBase::setDefaultValue() {
    setDefaultValue(((MyColorPickerButton*)_inputWidget)->currentColor());
}

const QString& MyColorParameterBase::defaultValue() const {
    return _defaultValue;
}

QWidget* MyColorParameterBase::createInputWidget() {
    return new MyColorPickerButton(colorName());
}

void MyColorParameterBase::updateInputWidget() {
    ((MyColorPickerButton*)_inputWidget)->setCurrentColor(defaultValue());
}

void MyColorParameterBase::connectInputWidget() {
    connect(_inputWidget, SIGNAL(colorIsChanged()), this, SIGNAL(isUpdated()));
}

void MyColorParameterBase::reset() {
    setDefaultValue("black");
}

void MyColorParameterBase::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // default value
        if (json.contains("default") && json["default"].isString())
            setDefaultValue(json["default"].toString());
    }
}

void MyColorParameterBase::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyColorPickerButton*)inputWidget())->currentColor();
    }
}

// MyBorderWidthParameter

MyBorderWidthParameter::MyBorderWidthParameter() : MyIntegerParameter("border-width", "Width of element borders") {
    reset();
}

void MyBorderWidthParameter::reset() {
    setDefaultValue(2);
    setMin(1);
    setMax(20);
    setStep(1);
}

// MyCentroidBorderWidthParameter

MyCentroidBorderWidthParameter::MyCentroidBorderWidthParameter() : MyBorderWidthParameter() {
    reset();
}

void MyCentroidBorderWidthParameter::reset() {
    setDefaultValue(2);
    setMin(1);
    setMax(20);
    setStep(1);
}

// MyPositionalParameter

MyPositionalParameter::MyPositionalParameter(const QString& name, const QString& hint) : MyDoubleParameter(name, hint) {
    reset();
}

void MyPositionalParameter::reset() {
    setDefaultValue(0.0);
    setMin(-10000.0);
    setMax(10000.0);
    setStep(1.0);
}

// MyNodeRectanglePositionalParameter

MyNodeRectanglePositionalParameter::MyNodeRectanglePositionalParameter(const QString& name, const QString& hint) : MyPositionalParameter(name, hint) {
    reset();
}

void MyNodeRectanglePositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-20.0);
}

// MyNodeEllipsePositionalParameter

MyNodeEllipsePositionalParameter::MyNodeEllipsePositionalParameter(const QString& name, const QString& hint) : MyPositionalParameter(name, hint) {
    reset();
}

void MyNodeEllipsePositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(0.0);
}

// MyNodeTextPositionalParameter

MyNodeTextPositionalParameter::MyNodeTextPositionalParameter(const QString& name, const QString& hint) : MyPositionalParameter(name, hint) {
    reset();
}

void MyNodeTextPositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-20.0);
}

// MyArrowHeadRectangleXParameter

MyArrowHeadRectangleXParameter::MyArrowHeadRectangleXParameter() : MyPositionalParameter("x", "Relative \"X\" value of rectangle bounding box") {
    reset();
}

void MyArrowHeadRectangleXParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-7.5);
}

// MyArrowHeadRectangleYParameter

MyArrowHeadRectangleYParameter::MyArrowHeadRectangleYParameter() : MyPositionalParameter("y", "Relative \"Y\" value of rectangle bounding box") {
    reset();
}

void MyArrowHeadRectangleYParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-3.725);
}

// MyArrowHeadEllipseCxParameter

MyArrowHeadEllipseCxParameter::MyArrowHeadEllipseCxParameter() : MyPositionalParameter("cx", "Relative center of ellipse along x-axis") {
    reset();
}

void MyArrowHeadEllipseCxParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-5.0);
}

// MyArrowHeadEllipseCyParameter

MyArrowHeadEllipseCyParameter::MyArrowHeadEllipseCyParameter() : MyPositionalParameter("cy", "Relative center of ellipse along y-axis") {
    reset();
}

void MyArrowHeadEllipseCyParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(0.0);
}

// MyRelativePositionalParameter

MyRelativePositionalParameter::MyRelativePositionalParameter(const QString& name, const QString& hint) : MyPositionalParameter(name, hint) {
    reset();
}

void MyRelativePositionalParameter::reset() {
    setDefaultValue(0);
    setMin(-100);
    setMax(100);
    setStep(1.0);
}

// MyDimensionalParameter

MyDimensionalParameter::MyDimensionalParameter(const QString& name, const QString& hint) : MyDoubleParameter(name, hint) {
    reset();
}

void MyDimensionalParameter::reset() {
    setDefaultValue(20.0);
    setMin(0.0);
    setMax(2000.0);
    setStep(1.0);
}

// MyNodeRectangleDimensionalParameter

MyNodeRectangleDimensionalParameter::MyNodeRectangleDimensionalParameter(const QString& name, const QString& hint) : MyDimensionalParameter(name, hint) {
    reset();
}

void MyNodeRectangleDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(40.0);
}

// MyNodeEllipseDimensionalParameter

MyNodeEllipseDimensionalParameter::MyNodeEllipseDimensionalParameter(const QString& name, const QString& hint) : MyDimensionalParameter(name, hint) {
    reset();
}

void MyNodeEllipseDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(20.0);
}

// MyNodeTextDimensionalParameter

MyNodeTextDimensionalParameter::MyNodeTextDimensionalParameter(const QString& name, const QString& hint) : MyDimensionalParameter(name, hint) {
    reset();
}

void MyNodeTextDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(40.0);
}

// MyArrowHeadRectangleDimensionalParameter

MyArrowHeadRectangleDimensionalParameter::MyArrowHeadRectangleDimensionalParameter(const QString& name, const QString& hint) : MyDimensionalParameter(name, hint) {
    reset();
}

void MyArrowHeadRectangleDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(7.5);
}

// MyArrowHeadEllipseDimensionalParameter

MyArrowHeadEllipseDimensionalParameter::MyArrowHeadEllipseDimensionalParameter(const QString& name, const QString& hint) : MyDimensionalParameter(name, hint) {
    reset();
}

void MyArrowHeadEllipseDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(5.0);
}

// MyCornerCurvatureParameter

MyCornerCurvatureParameter::MyCornerCurvatureParameter(const QString& name, const QString& hint) : MyDoubleParameter(name, hint) {
    reset();
}

void MyCornerCurvatureParameter::reset() {
    setDefaultValue(0.0000);
    setMin(0.0);
    setMax(10000.0);
    setStep(1.0);
}

// MyNodeCentroidRadiusParameter

MyNodeCentroidRadiusParameter::MyNodeCentroidRadiusParameter() : MyDimensionalParameter("radius", "\"Radius\" value of centroid shape") {
    reset();
}

void MyNodeCentroidRadiusParameter::reset() {
    setDefaultValue(0.100);
    setMin(0.010);
    setMax(5.000);
    setStep(0.010);
}

// MyBorderColorParameter

MyBorderColorParameter::MyBorderColorParameter() : MyColorParameterBase("border-color", "Color of element borders") {
    reset();
}

const QString MyBorderColorParameter::colorName() {
    return "Border Color";
}

void MyBorderColorParameter::reset() {
    setDefaultValue("black");
}

// MyCentroidBorderColorParameter

MyCentroidBorderColorParameter::MyCentroidBorderColorParameter() : MyBorderColorParameter() {
    reset();
}

void MyCentroidBorderColorParameter::reset() {
    setDefaultValue("transparent");
}

// MyFillColorParameter

MyFillColorParameter::MyFillColorParameter() : MyColorParameterBase("fill-color", "Fill color of element") {
    reset();
}

const QString MyFillColorParameter::colorName() {
    return "Fill Color";
}

void MyFillColorParameter::reset() {
    setDefaultValue("transparent");
}

// MyCentroidFillColorParameter

MyCentroidFillColorParameter::MyCentroidFillColorParameter() : MyFillColorParameter() {
    reset();
}

void MyCentroidFillColorParameter::reset() {
    setDefaultValue("white");
}

// MyTextPlainTextParameter

MyTextPlainTextParameter::MyTextPlainTextParameter() : MyStringParameter("plain-text", "Text being displayed on element") {
    reset();
}

void MyTextPlainTextParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(textChanged(const QString&)), this, SIGNAL(isUpdated()));
}


void MyTextPlainTextParameter::reset() {
    MyStringParameter::reset();
    _defaultValue = "text";
}

// MyTextColorParameter

MyTextColorParameter::MyTextColorParameter() : MyColorParameterBase("color", "Text \"Color\"") {
    reset();
}

const QString MyTextColorParameter::colorName() {
    return "Text Color";
}

void MyTextColorParameter::reset() {
    setDefaultValue("black");
}

// MyFontParameter

MyFontParameter::MyFontParameter() : MyParameterBase("font", "Text \"Font\"") {

}

MyParameterBase::PARAMETER_TYPE MyFontParameter::type() {
    return FONT_PARAMETER_TYPE;
}

void MyFontParameter::setDefaultValue(const QFont& font) {
    _defaultValue = font;
}

void MyFontParameter::setDefaultFontFamilyValue(const QString& fontFamily) {
    _defaultValue.setFamily(fontFamily);
}

void MyFontParameter::setDefaultFontSizeValue(const qreal& fontSize) {
    _defaultValue.setPointSize(fontSize);
}

void MyFontParameter::setDefaultFontWeightValue(const QString& fontWeight) {
    if (fontWeight == "bold")
        _defaultValue.setBold(true);
    else
        _defaultValue.setBold(false);
}
void MyFontParameter::setDefaultFontStyleValue(const QString& fontStyle) {
    if (fontStyle == "italic")
        _defaultValue.setItalic(true);
    else
        _defaultValue.setItalic(false);
}

void MyFontParameter::setDefaultValue() {
    setDefaultValue(((MyFontPickerButton*)_inputWidget)->currentFont());
}

const QFont& MyFontParameter::defaultValue() const {
    return _defaultValue;
}

const QString MyFontParameter::defaultFontFamilyValue() const {
    return _defaultValue.family();
}

const qreal MyFontParameter::defaultFontSizeValue() const {
    return _defaultValue.pointSizeF();
}

const QString MyFontParameter::defaultFontWeightValue() const {
    if (_defaultValue.bold())
        return "bold";
    else
        return "normal";
}

const QString MyFontParameter::defaultFontStyleValue() const {
    if (_defaultValue.italic())
        return "italic";
    else
        return "normal";
}

QWidget* MyFontParameter::createInputWidget() {
    return new MyFontPickerButton();
}

void MyFontParameter::updateInputWidget() {
    ((MyFontPickerButton*)_inputWidget)->setCurrentFont(defaultValue());
}

void MyFontParameter::connectInputWidget() {
    connect(_inputWidget, SIGNAL(fontIsChanged()), this, SIGNAL(isUpdated()));
}

void MyFontParameter::reset() {
    setDefaultValue(QFont("Arial", 8));
}

void MyFontParameter::read(const QJsonObject &json) {

}

void MyFontParameter::write(QJsonObject &json) {

}

// MyHorizontalAlignmentParameter

MyHorizontalAlignmentParameter::MyHorizontalAlignmentParameter() : MyNominalParameter("horizontal-alignment", "Text \"Horizontal Alignment\"") {
    _items.push_back("start");
    _items.push_back("middle");
    _items.push_back("end");
    reset();
}

const Qt::Alignment MyHorizontalAlignmentParameter::defaultAlignment() const {
    if (defaultValue() == "start")
        return Qt::AlignLeft;
    else if (defaultValue() == "middle")
        return Qt::AlignHCenter;
    else if (defaultValue() == "end")
        return Qt::AlignRight;

    return Qt::AlignHCenter;
}

void MyHorizontalAlignmentParameter::reset() {
    setDefaultValue("middle");
}

// MyVerticalAlignmentParameter

MyVerticalAlignmentParameter::MyVerticalAlignmentParameter() : MyNominalParameter("vertical-alignment", "Text \"Vertical Alignment\"") {
    _items.push_back("top");
    _items.push_back("middle");
    _items.push_back("bottom");
    reset();
}

const Qt::Alignment MyVerticalAlignmentParameter::defaultAlignment() const {
    if (defaultValue() == "top")
        return Qt::AlignTop;
    else if (defaultValue() == "middle")
        return Qt::AlignVCenter;
    else if (defaultValue() == "bottom")
        return Qt::AlignBottom;

    return Qt::AlignVCenter;
}

void MyVerticalAlignmentParameter::reset() {
    setDefaultValue("middle");
}

MyNodeDistributionNumberOfGridColumnsParameter::MyNodeDistributionNumberOfGridColumnsParameter(const qint32 numberOfSelectedNodes) : MyIntegerParameter("Number of Grid Columns") {
    _numberOfSelectedNodes = numberOfSelectedNodes;
    reset();
}

void MyNodeDistributionNumberOfGridColumnsParameter::reset() {
    setMin(1);
    setMax(_numberOfSelectedNodes);
    setStep(1);
    setDefaultValue(1);
    if (_numberOfSelectedNodes >= 2)
        setDefaultValue(qint32(0.5 * _numberOfSelectedNodes));
}
