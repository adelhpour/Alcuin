#include "negui_parameters.h"
#include "negui_customized_menu_widgets.h"

// MyParameterBase

MyParameterBase::MyParameterBase(const QString& name) : MyBase(name) {
    _inputWidget = NULL;
}

QWidget* MyParameterBase::inputWidget() {
    _inputWidget = createInputWidget();
    updateInputWidget();
    connectInputWidget();
    return _inputWidget;
}

// MyDoubleParameter

MyDoubleParameter::MyDoubleParameter(const QString& name) : MyParameterBase(name) {
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
    connect(_inputWidget, SIGNAL(editingFinished()), this, SIGNAL(isUpdated()));
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

MyIntegerParameter::MyIntegerParameter(const QString& name) : MyParameterBase(name) {
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
    connect(_inputWidget, SIGNAL(editingFinished()), this, SIGNAL(isUpdated()));
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

MyBooleanParameter::MyBooleanParameter(const QString& name) : MyParameterBase(name) {
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

MyStringParameter::MyStringParameter(const QString& name) : MyParameterBase(name) {
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

MyNominalParameter::MyNominalParameter(const QString& name) : MyParameterBase(name) {
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

MyPointParameterBase::MyPointParameterBase(const QString& name) : MyParameterBase(name) {

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
    widget->setFixedWidth(0.5 * 1.2 * fixedWidth);

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
    /*
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // default value
        if (json.contains("default") && json["default"].isString())
            setDefaultValue(json["default"].toString());
    }
     */
}

void MyPointParameterBase::write(QJsonObject &json) {
    /*
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyColorPickerButton*)inputWidget())->currentColor();
    }
     */
}

// MyAbsolutePointParameter

MyAbsolutePointParameter::MyAbsolutePointParameter(const QString& name) : MyPointParameterBase(name) {
    _x = new MyPositionalParameter("x");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyPositionalParameter("y");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyRelativePointParameter

MyRelativePointParameter::MyRelativePointParameter(const QString& name) : MyPointParameterBase(name) {
    _x = new MyRelativePositionalParameter("x (%)");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyRelativePositionalParameter("y (%)");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyBasePointParameter

MyBasePointParameter::MyBasePointParameter(const QString& name) : MyPointParameterBase(name) {
    _x = new MyPositionalParameter("x (%)");
    connect(_x, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    _y = new MyPositionalParameter("y (%)");
    connect(_y, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
    reset();
}

// MyColorParameter

MyColorParameter::MyColorParameter(const QString& name) : MyParameterBase(name) {
    reset();
}

MyParameterBase::PARAMETER_TYPE MyColorParameter::type() {
    return COLOR_PARAMETER_TYPE;
}

void MyColorParameter::setDefaultValue(const QString& value) {
    _defaultValue = value;
}

void MyColorParameter::setDefaultValue() {
    setDefaultValue(((MyColorPickerButton*)_inputWidget)->currentColor());
}

const QString& MyColorParameter::defaultValue() const {
    return _defaultValue;
}

QWidget* MyColorParameter::createInputWidget() {
    return new MyColorPickerButton();
}

void MyColorParameter::updateInputWidget() {
    ((MyColorPickerButton*)_inputWidget)->setCurrentColor(defaultValue());
}

void MyColorParameter::connectInputWidget() {
    connect(((MyColorPickerMenu*)((MyColorPickerButton*)_inputWidget)->menu()), &MyColorPickerMenu::colorChosen, this, [this] (const QString& color) {emit isUpdated(); });
}

void MyColorParameter::reset() {
    setDefaultValue("black");
}

void MyColorParameter::read(const QJsonObject &json) {
    if (json.contains("parameter") && json["parameter"].isString() && json["parameter"].toString() == name()) {
        reset();

        // default value
        if (json.contains("default") && json["default"].isString())
            setDefaultValue(json["default"].toString());
    }
}

void MyColorParameter::write(QJsonObject &json) {
    if (_inputWidget) {
        json["parameter"] = name();
        json["value"] = ((MyColorPickerButton*)inputWidget())->currentColor();
    }
}

// MyStrokeWidthParameter

MyStrokeWidthParameter::MyStrokeWidthParameter() : MyIntegerParameter("stroke-width") {
    reset();
}

void MyStrokeWidthParameter::reset() {
    setDefaultValue(2);
    setMin(1);
    setMax(20);
    setStep(1);
}

// MyCentroidStrokeWidthParameter

MyCentroidStrokeWidthParameter::MyCentroidStrokeWidthParameter() : MyStrokeWidthParameter() {
    reset();
}

void MyCentroidStrokeWidthParameter::reset() {
    setDefaultValue(2);
    setMin(1);
    setMax(20);
    setStep(1);
}

// MyPositionalParameter

MyPositionalParameter::MyPositionalParameter(const QString& name) : MyDoubleParameter(name) {
    reset();
}

void MyPositionalParameter::reset() {
    setDefaultValue(0.0);
    setMin(-1000.0);
    setMax(1000.0);
    setStep(1.0);
}

// MyNodeRectPositionalParameter

MyNodeRectPositionalParameter::MyNodeRectPositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyNodeRectPositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-20.0);
}

// MyNodeEllipsePositionalParameter

MyNodeEllipsePositionalParameter::MyNodeEllipsePositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyNodeEllipsePositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(0.0);
}

// MyNodeTextPositionalParameter

MyNodeTextPositionalParameter::MyNodeTextPositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyNodeTextPositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-20.0);
}

// MyArrowHeadRectXParameter

MyArrowHeadRectXParameter::MyArrowHeadRectXParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyArrowHeadRectXParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-7.5);
}

// MyArrowHeadRectYParameter

MyArrowHeadRectYParameter::MyArrowHeadRectYParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyArrowHeadRectYParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-3.725);
}

// MyArrowHeadEllipseCxParameter

MyArrowHeadEllipseCxParameter::MyArrowHeadEllipseCxParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyArrowHeadEllipseCxParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(-5.0);
}

// MyArrowHeadEllipseCyParameter

MyArrowHeadEllipseCyParameter::MyArrowHeadEllipseCyParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyArrowHeadEllipseCyParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(0.0);
}

// MyRelativePositionalParameter

MyRelativePositionalParameter::MyRelativePositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyRelativePositionalParameter::reset() {
    setDefaultValue(0);
    setMin(-100);
    setMax(100);
    setStep(1.0);
}

// MyDimensionalParameter

MyDimensionalParameter::MyDimensionalParameter(const QString& name) : MyDoubleParameter(name) {
    reset();
}

void MyDimensionalParameter::reset() {
    setDefaultValue(20.0);
    setMin(0.0);
    setMax(2000.0);
    setStep(1.0);
}

// MyNodeRectDimensionalParameter

MyNodeRectDimensionalParameter::MyNodeRectDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyNodeRectDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(40.0);
}

// MyNodeEllipseDimensionalParameter

MyNodeEllipseDimensionalParameter::MyNodeEllipseDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyNodeEllipseDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(20.0);
}

// MyNodeTextDimensionalParameter

MyNodeTextDimensionalParameter::MyNodeTextDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyNodeTextDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(40.0);
}

// MyArrowHeadRectDimensionalParameter

MyArrowHeadRectDimensionalParameter::MyArrowHeadRectDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyArrowHeadRectDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(7.5);
}

// MyArrowHeadEllipseDimensionalParameter

MyArrowHeadEllipseDimensionalParameter::MyArrowHeadEllipseDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyArrowHeadEllipseDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(5.0);
}

// MyCornerCurvatureParameter

MyCornerCurvatureParameter::MyCornerCurvatureParameter(const QString& name) : MyDoubleParameter(name) {
    reset();
}

void MyCornerCurvatureParameter::reset() {
    setDefaultValue(0.0000);
    setMin(0.0);
    setMax(1000.0);
    setStep(1.0);
}

// MyNodeCentroidRadiusParameter

MyNodeCentroidRadiusParameter::MyNodeCentroidRadiusParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyNodeCentroidRadiusParameter::reset() {
    setDefaultValue(0.100);
    setMin(0.010);
    setMax(5.000);
    setStep(0.010);
}

// MyStrokeParameter

MyStrokeParameter::MyStrokeParameter() : MyColorParameter("stroke") {
    reset();
}

void MyStrokeParameter::reset() {
    setDefaultValue("black");
}

// MyCentroidStrokeParameter

MyCentroidStrokeParameter::MyCentroidStrokeParameter() : MyStrokeParameter() {
    reset();
}

void MyCentroidStrokeParameter::reset() {
    setDefaultValue("transparent");
}

// MyFillParameter

MyFillParameter::MyFillParameter() : MyColorParameter("fill") {
    reset();
}

void MyFillParameter::reset() {
    setDefaultValue("transparent");
}

// MyCentroidFillParameter

MyCentroidFillParameter::MyCentroidFillParameter() : MyFillParameter() {
    reset();
}

void MyCentroidFillParameter::reset() {
    setDefaultValue("white");
}

// MyFontFamilyParameter

MyFontFamilyParameter::MyFontFamilyParameter() : MyStringParameter("font-family") {
    reset();
}

void MyFontFamilyParameter::reset() {
    setDefaultValue("monospace");
}

// MyFontSizeParameter

MyFontSizeParameter::MyFontSizeParameter() : MyNominalParameter("font-size") {
    int i = 5;
    while (i <= 72) {
        _items.push_back(QString::number(i));

        if (i < 12)
            ++i;
        else if (i < 28)
            i += 2;
        else if (i < 36)
            i += 8;
        else if (i < 48)
            i += 12;
        else
            i += 24;
    }
}

void MyFontSizeParameter::reset() {
    setDefaultValue("10");
}

const qint32 MyFontSizeParameter::defaultSize() const {
    return defaultValue().toDouble();
}

// MyFontWeightParameter

MyFontWeightParameter::MyFontWeightParameter() : MyNominalParameter("font-weight") {
    _items.push_back("normal");
    _items.push_back("bold");
    reset();
}

const bool MyFontWeightParameter::defaultWeight() const {
    if (defaultValue() == "bold")
        return true;

    return false;
}

void MyFontWeightParameter::reset() {
    setDefaultValue("normal");
}

// MyFontStyleParameter

MyFontStyleParameter::MyFontStyleParameter() : MyNominalParameter("font-style") {
    _items.push_back("normal");
    _items.push_back("italic");
    reset();
}

const bool MyFontStyleParameter::defaultStyle() const {
    if (defaultValue() == "italic")
        return true;

    return false;
}

void MyFontStyleParameter::reset() {
    setDefaultValue("normal");
}

// MyTextAnchorParameter

MyTextAnchorParameter::MyTextAnchorParameter() : MyNominalParameter("text-anchor") {
    _items.push_back("start");
    _items.push_back("middle");
    _items.push_back("end");
    reset();
}

const Qt::Alignment MyTextAnchorParameter::defaultAlignment() const {
    if (defaultValue() == "start")
        return Qt::AlignLeft;
    else if (defaultValue() == "middle")
        return Qt::AlignHCenter;
    else if (defaultValue() == "end")
        return Qt::AlignRight;

    return Qt::AlignHCenter;
}

void MyTextAnchorParameter::reset() {
    setDefaultValue("middle");
}

// MyVTextAnchorParameter

MyVTextAnchorParameter::MyVTextAnchorParameter() : MyNominalParameter("vtext-anchor") {
    _items.push_back("top");
    _items.push_back("middle");
    _items.push_back("baseline");
    _items.push_back("bottom");
    reset();
}

const Qt::Alignment MyVTextAnchorParameter::defaultAlignment() const {
    if (defaultValue() == "top")
        return Qt::AlignTop;
    else if (defaultValue() == "middle")
        return Qt::AlignVCenter;
    else if (defaultValue() == "baseline")
        return Qt::AlignBaseline;
    else if (defaultValue() == "bottom")
        return Qt::AlignBottom;

    return Qt::AlignVCenter;
}

void MyVTextAnchorParameter::reset() {
    setDefaultValue("middle");
}