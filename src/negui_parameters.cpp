#include "negui_parameters.h"
#include <QWidgetAction>
#include <QtMath>
#include <QJsonObject>
#include <QJsonArray>

// MyParameterBase

MyParameterBase::MyParameterBase(const QString& name) : MyBase(name) {
    _inputWidget = NULL;
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

QWidget* MyDoubleParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MyDoubleSpinBox();
    ((MyDoubleSpinBox*)_inputWidget)->setMinimum(min());
    ((MyDoubleSpinBox*)_inputWidget)->setMaximum(max());
    ((MyDoubleSpinBox*)_inputWidget)->setSingleStep(step());
    ((MyDoubleSpinBox*)_inputWidget)->setValue(defaultValue());
    QStringList stepNumbers = QString::number(step()).split('.');
    if (stepNumbers.size() == 2)
        ((MyDoubleSpinBox*)_inputWidget)->setDecimals(stepNumbers.at(1).size());
    
    return _inputWidget;
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

QWidget* MyIntegerParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MySpinBox();
    ((MySpinBox*)_inputWidget)->setMinimum(min());
    ((MySpinBox*)_inputWidget)->setMaximum(max());
    ((MySpinBox*)_inputWidget)->setSingleStep(step());
    ((MySpinBox*)_inputWidget)->setValue(defaultValue());
    
    return _inputWidget;
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

QWidget* MyBooleanParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MyComboBox();
    ((MyComboBox*)_inputWidget)->clear();
    ((MyComboBox*)_inputWidget)->addItem("true");
    ((MyComboBox*)_inputWidget)->addItem("false");
    if (defaultValue())
        ((MyComboBox*)_inputWidget)->setCurrentText("true");
    else
        ((MyComboBox*)_inputWidget)->setCurrentText("false");
    
    return _inputWidget;
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

QWidget* MyStringParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MyLineEdit();
    ((MyLineEdit*)_inputWidget)->setText(defaultValue());

    return _inputWidget;
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

QWidget* MyNominalParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MyComboBox();
    ((MyComboBox*)_inputWidget)->clear();
    ((MyComboBox*)_inputWidget)->addItems(items());
    ((MyComboBox*)_inputWidget)->setCurrentText(defaultValue());
    
    return _inputWidget;
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

QWidget* MyPointParameterBase::inputWidget() {
    if (_inputWidget)
        _inputWidget->deleteLater();
    qint32 fixedWidth = 0;
    _inputWidget = new QWidget();
    QHBoxLayout* contentLayout = new QHBoxLayout(_inputWidget);
    MyLabel* xLabel = new MyLabel(_x->name());
    xLabel->setFixedWidth(60);
    xLabel->setAlignment(Qt::AlignRight);
    contentLayout->addWidget(xLabel);
    fixedWidth += xLabel->size().width();
    contentLayout->addItem(new MySpacerItem(7, 0));
    fixedWidth += 7;
    contentLayout->addWidget(_x->inputWidget());
    fixedWidth += _x->inputWidget()->size().width();
    contentLayout->addItem(new MySpacerItem(25, 0));
    fixedWidth += 25;
    MyLabel* yLabel = new MyLabel(_y->name());
    yLabel->setFixedWidth(60);
    yLabel->setAlignment(Qt::AlignRight);
    contentLayout->addWidget(yLabel);
    fixedWidth += yLabel->size().width();
    contentLayout->addItem(new MySpacerItem(7, 0));
    fixedWidth += 7;
    contentLayout->addWidget(_y->inputWidget());
    fixedWidth += _y->inputWidget()->size().width();
    _inputWidget->setLayout(contentLayout);
    _inputWidget->setFixedSize(fixedWidth, 35);
    
    return _inputWidget;
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
    _y = new MyPositionalParameter("y");
    reset();
}

// MyRelativePointParameter

MyRelativePointParameter::MyRelativePointParameter(const QString& name) : MyPointParameterBase(name) {
    _x = new MyRelativePositionalParameter("x (%)");
    _y = new MyRelativePositionalParameter("y (%)");
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

QWidget* MyColorParameter::inputWidget() {
    if (!_inputWidget)
        _inputWidget = new MyColorPickerButton();
    ((MyColorPickerButton*)_inputWidget)->setCurrentColor(defaultValue());
    
    return _inputWidget;
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

// MyNodeEllipsePositionalParameter

MyNodeEllipsePositionalParameter::MyNodeEllipsePositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyNodeEllipsePositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(25.0);
}

// MyArrowHeadEllipsePositionalParameter

MyArrowHeadEllipsePositionalParameter::MyArrowHeadEllipsePositionalParameter(const QString& name) : MyPositionalParameter(name) {
    reset();
}

void MyArrowHeadEllipsePositionalParameter::reset() {
    MyPositionalParameter::reset();
    setDefaultValue(7.5);
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
    setDefaultValue(25.0);
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
    setDefaultValue(50.0 * qSin(qDegreesToRadians(45.0)));
}

// MyArrowHeadEllipseDimensionalParameter

MyArrowHeadEllipseDimensionalParameter::MyArrowHeadEllipseDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyArrowHeadEllipseDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(7.5);
}

// MyArrowHeadRectDimensionalParameter

MyArrowHeadRectDimensionalParameter::MyArrowHeadRectDimensionalParameter(const QString& name) : MyDimensionalParameter(name) {
    reset();
}

void MyArrowHeadRectDimensionalParameter::reset() {
    MyDimensionalParameter::reset();
    setDefaultValue(15.0 * qSin(qDegreesToRadians(45.0)));
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

// MyStrokeParameter

MyStrokeParameter::MyStrokeParameter() : MyColorParameter("stroke") {
    reset();
}

void MyStrokeParameter::reset() {
    setDefaultValue("black");
}

// MyFillParameter

MyFillParameter::MyFillParameter() : MyColorParameter("fill") {
    reset();
}

void MyFillParameter::reset() {
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

// MyColorPickerButton

MyColorPickerButton::MyColorPickerButton(QWidget *parent) : QToolButton(parent) {
    MyColorPickerMenu* colorPickerMenu = new MyColorPickerMenu();
    setMenu(colorPickerMenu);
    connect(colorPickerMenu, SIGNAL(colorChosen(const QString&)), this, SLOT(setCurrentColor(const QString&)));
    setArrowType(Qt::NoArrow);
    setPopupMode(QToolButton::InstantPopup);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setStyleSheet("QToolButton { border: 1px solid LightSlateGray; border-radius: 6px; background-color: white} QToolButton::menu-indicator { image: none; }" );
    connect(this, SIGNAL(clicked()), this, SLOT(showMenu()));
    setFixedSize(QSize(20, 20));
}

void MyColorPickerButton::setBackgroundColor(const QString& color)  {
    if (!color.isEmpty())
        setStyleSheet("QToolButton { border: 1px solid LightSlateGray; border-radius: 5px; background-color:" + color + " } QToolButton::menu-indicator { image: none; } " );
}

void MyColorPickerButton::setCurrentColor(const QString& color)  {
    _currentColor = color;
    setBackgroundColor(color);
}

const QString& MyColorPickerButton::currentColor() const {
    return _currentColor;
}
 
// MyColorPickerMenu

MyColorPickerMenu::MyColorPickerMenu() {
    /// all colors
    QWidgetAction* allColors = new QWidgetAction(this);
    QWidget* allColorsWidget = new QWidget(this);
    QGridLayout* allColorsContentLayout = new QGridLayout(allColorsWidget);
    allColorsContentLayout->setSpacing(1);
    int rowCount = 2;
    int columnCount = 0;
    
    // label
    allColorsContentLayout->addWidget(new MyLabel("Colors"), allColorsContentLayout->rowCount() - 1, 0, 1, 14, Qt::AlignLeft);
    
    allColorsContentLayout->addItem(new QSpacerItem(1, 10), allColorsContentLayout->rowCount(), 0, 1, 14);

    // black
    MyColorTileButton* blackButton = new MyColorTileButton("Black", "#000000");
    allColorsContentLayout->addWidget(blackButton, rowCount++, columnCount);
    connect(blackButton, &QPushButton::clicked, this, [this, blackButton] () { this->colorTileButtonPicked(blackButton); });
    
    // darkslategray
    MyColorTileButton* darkslategrayButton = new MyColorTileButton("DarkSlateGray", "#2F4F4F");
    allColorsContentLayout->addWidget(darkslategrayButton, rowCount++, columnCount);
    connect(darkslategrayButton, &QPushButton::clicked, this, [this, darkslategrayButton] () { this->colorTileButtonPicked(darkslategrayButton); });
    
    // dimgray
    MyColorTileButton* dimgrayButton = new MyColorTileButton("DimGray" , "#696969");
    allColorsContentLayout->addWidget(dimgrayButton, rowCount++, columnCount);
    connect(dimgrayButton, &QPushButton::clicked, this, [this, dimgrayButton] () { this->colorTileButtonPicked(dimgrayButton); });
    
    // slategray
    MyColorTileButton* slategrayButton = new MyColorTileButton("SlateGray", "#708090");
    allColorsContentLayout->addWidget(slategrayButton, rowCount++, columnCount);
    connect(slategrayButton, &QPushButton::clicked, this, [this, slategrayButton] () { this->colorTileButtonPicked(slategrayButton); });
    
    // gray
    MyColorTileButton* grayButton = new MyColorTileButton("Gray", "#808080");
    allColorsContentLayout->addWidget(grayButton, rowCount++, columnCount);
    connect(grayButton, &QPushButton::clicked, this, [this, grayButton] () { this->colorTileButtonPicked(grayButton); });
    
    // lightslategray
    MyColorTileButton* lightslategrayButton = new MyColorTileButton("LightSlateGray", "#778899");
    allColorsContentLayout->addWidget(lightslategrayButton, rowCount++, columnCount);
    connect(lightslategrayButton, &QPushButton::clicked, this, [this, lightslategrayButton] () { this->colorTileButtonPicked(lightslategrayButton); });
    
    // darkgray
    MyColorTileButton* darkgrayButton = new MyColorTileButton("DarkGray", "#A9A9A9");
    allColorsContentLayout->addWidget(darkgrayButton, rowCount++, columnCount);
    connect(darkgrayButton, &QPushButton::clicked, this, [this, darkgrayButton] () { this->colorTileButtonPicked(darkgrayButton); });
    
    // silver
    MyColorTileButton* silverButton = new MyColorTileButton("Silver", "#C0C0C0");
    allColorsContentLayout->addWidget(silverButton, rowCount++, columnCount);
    connect(silverButton, &QPushButton::clicked, this, [this, silverButton] () { this->colorTileButtonPicked(silverButton); });
    
    // lightgray
    MyColorTileButton* lightgrayButton = new MyColorTileButton("LightGray", "#D3D3D3");
    allColorsContentLayout->addWidget(lightgrayButton, rowCount++, columnCount);
    connect(lightgrayButton, &QPushButton::clicked, this, [this, lightgrayButton] () { this->colorTileButtonPicked(lightgrayButton); });
    
    // gainsboro
    MyColorTileButton* gainsboroButton = new MyColorTileButton("Gainsboro", "#DCDCDC");
    allColorsContentLayout->addWidget(gainsboroButton, rowCount++, columnCount);
    connect(gainsboroButton, &QPushButton::clicked, this, [this, gainsboroButton] () { this->colorTileButtonPicked(gainsboroButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // maroon
    MyColorTileButton* maroonButton = new MyColorTileButton("Maroon", "#800000");
    allColorsContentLayout->addWidget(maroonButton, rowCount++, columnCount);
    connect(maroonButton, &QPushButton::clicked, this, [this, maroonButton] () { this->colorTileButtonPicked(maroonButton); });
    
    // brown
    MyColorTileButton* brownButton = new MyColorTileButton("Brown", "#A52A2A");
    allColorsContentLayout->addWidget(brownButton, rowCount++, columnCount);
    connect(brownButton, &QPushButton::clicked, this, [this, brownButton] () { this->colorTileButtonPicked(brownButton); });
    
    // saddlebrown
    MyColorTileButton* saddlebrownButton = new MyColorTileButton("SaddleBrown", "#8B4513");
    allColorsContentLayout->addWidget(saddlebrownButton, rowCount++, columnCount);
    connect(saddlebrownButton, &QPushButton::clicked, this, [this, saddlebrownButton] () { this->colorTileButtonPicked(saddlebrownButton); });
    
    // sienna
    MyColorTileButton* siennaButton = new MyColorTileButton("Sienna", "#A0522D");
    allColorsContentLayout->addWidget(siennaButton, rowCount++, columnCount);
    connect(siennaButton, &QPushButton::clicked, this, [this, siennaButton] () { this->colorTileButtonPicked(siennaButton); });
    
    // chocolate
    MyColorTileButton* chocolateButton = new MyColorTileButton("Chocolate", "#D2691E");
    allColorsContentLayout->addWidget(chocolateButton, rowCount++, columnCount);
    connect(chocolateButton, &QPushButton::clicked, this, [this, chocolateButton] () { this->colorTileButtonPicked(chocolateButton); });
    
    // darkgoldenrod
    MyColorTileButton* darkgoldenrodButton = new MyColorTileButton("DarkGoldenrod", "#B8860B");
    allColorsContentLayout->addWidget(darkgoldenrodButton, rowCount++, columnCount);
    connect(darkgoldenrodButton, &QPushButton::clicked, this, [this, darkgoldenrodButton] () { this->colorTileButtonPicked(darkgoldenrodButton); });
    
    // peru
    MyColorTileButton* peruButton = new MyColorTileButton("Peru", "#CD853F");
    allColorsContentLayout->addWidget(peruButton, rowCount++, columnCount);
    connect(peruButton, &QPushButton::clicked, this, [this, peruButton] () { this->colorTileButtonPicked(peruButton); });
    
    // rosybrown
    MyColorTileButton* rosybrownButton = new MyColorTileButton("RosyBrown", "#BC8F8F");
    allColorsContentLayout->addWidget(rosybrownButton, rowCount++, columnCount);
    connect(rosybrownButton, &QPushButton::clicked, this, [this, rosybrownButton] () { this->colorTileButtonPicked(rosybrownButton); });
    
    // goldenrod
    MyColorTileButton* goldenrodButton = new MyColorTileButton("Goldenrod", "#DAA520");
    allColorsContentLayout->addWidget(goldenrodButton, rowCount++, columnCount);
    connect(goldenrodButton, &QPushButton::clicked, this, [this, goldenrodButton] () { this->colorTileButtonPicked(goldenrodButton); });
    
    // sandybrown
    MyColorTileButton* sandybrownButton = new MyColorTileButton("SandyBrown", "#F4A460");
    allColorsContentLayout->addWidget(sandybrownButton, rowCount++, columnCount);
    connect(sandybrownButton, &QPushButton::clicked, this, [this, sandybrownButton] () { this->colorTileButtonPicked(sandybrownButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // tan
    MyColorTileButton* tanButton = new MyColorTileButton("Tan", "#D2B48C");
    allColorsContentLayout->addWidget(tanButton, rowCount++, columnCount);
    connect(tanButton, &QPushButton::clicked, this, [this, tanButton] () { this->colorTileButtonPicked(tanButton); });
    
    // burlywood
    MyColorTileButton* burlywoodButton = new MyColorTileButton("Burlywood", "#DEB887");
    allColorsContentLayout->addWidget(burlywoodButton, rowCount++, columnCount);
    connect(burlywoodButton, &QPushButton::clicked, this, [this, burlywoodButton] () { this->colorTileButtonPicked(burlywoodButton); });
    
    // wheat
    MyColorTileButton* wheatButton = new MyColorTileButton("Wheat", "#F5DEB3");
    allColorsContentLayout->addWidget(wheatButton, rowCount++, columnCount);
    connect(wheatButton, &QPushButton::clicked, this, [this, wheatButton] () { this->colorTileButtonPicked(wheatButton); });
    
    // navajowhite
    MyColorTileButton* navajowhiteButton = new MyColorTileButton("NavajoWhite", "#FFDEAD");
    allColorsContentLayout->addWidget(navajowhiteButton, rowCount++, columnCount);
    connect(navajowhiteButton, &QPushButton::clicked, this, [this, navajowhiteButton] () { this->colorTileButtonPicked(navajowhiteButton); });
    
    // bisque
    MyColorTileButton* bisqueButton = new MyColorTileButton("Bisque", "#FFE4C4");
    allColorsContentLayout->addWidget(bisqueButton, rowCount++, columnCount);
    connect(bisqueButton, &QPushButton::clicked, this, [this, bisqueButton] () { this->colorTileButtonPicked(bisqueButton); });
    
    // blanchedalmond
    MyColorTileButton* blanchedalmondButton = new MyColorTileButton("BlanchedAlmond", "#FFEBCD");
    allColorsContentLayout->addWidget(blanchedalmondButton, rowCount++, columnCount);
    connect(blanchedalmondButton, &QPushButton::clicked, this, [this, blanchedalmondButton] () { this->colorTileButtonPicked(blanchedalmondButton); });
    
    // cornsilk
    MyColorTileButton* cornsilkButton = new MyColorTileButton("Cornsilk", "#FFF8DC");
    allColorsContentLayout->addWidget(cornsilkButton, rowCount++, columnCount);
    connect(cornsilkButton, &QPushButton::clicked, this, [this, cornsilkButton] () { this->colorTileButtonPicked(cornsilkButton); });
    
    // darkred
    MyColorTileButton* darkredButton = new MyColorTileButton("DarkRed", "#8B0000");
    allColorsContentLayout->addWidget(darkredButton, rowCount++, columnCount);
    connect(darkredButton, &QPushButton::clicked, this, [this, darkredButton] () { this->colorTileButtonPicked(darkredButton); });
    
    // red
    MyColorTileButton* redButton = new MyColorTileButton("Red", "#FF0000");
    allColorsContentLayout->addWidget(redButton, rowCount++, columnCount);
    connect(redButton, &QPushButton::clicked, this, [this, redButton] () { this->colorTileButtonPicked(redButton); });
    
    // firebrick
    MyColorTileButton* firebrickButton = new MyColorTileButton("Firebrick", "#B22222");
    allColorsContentLayout->addWidget(firebrickButton, rowCount++, columnCount);
    connect(firebrickButton, &QPushButton::clicked, this, [this, firebrickButton] () { this->colorTileButtonPicked(firebrickButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // crimson
    MyColorTileButton* crimsonButton = new MyColorTileButton("Crimson", "#DC143C");
    allColorsContentLayout->addWidget(crimsonButton, rowCount++, columnCount);
    connect(crimsonButton, &QPushButton::clicked, this, [this, crimsonButton] () { this->colorTileButtonPicked(crimsonButton); });
    
    // indianred
    MyColorTileButton* indianredButton = new MyColorTileButton("IndianRed", "#CD5C5C");
    allColorsContentLayout->addWidget(indianredButton, rowCount++, columnCount);
    connect(indianredButton, &QPushButton::clicked, this, [this, indianredButton] () { this->colorTileButtonPicked(indianredButton); });
    
    // lightcoral
    MyColorTileButton* lightcoralButton = new MyColorTileButton("LightCoral", "#F08080");
    allColorsContentLayout->addWidget(lightcoralButton, rowCount++, columnCount);
    connect(lightcoralButton, &QPushButton::clicked, this, [this, lightcoralButton] () { this->colorTileButtonPicked(lightcoralButton); });
    
    // salmon
    MyColorTileButton* salmonButton = new MyColorTileButton("Salmon", "#FA8072");
    allColorsContentLayout->addWidget(salmonButton, rowCount++, columnCount);
    connect(salmonButton, &QPushButton::clicked, this, [this, salmonButton] () { this->colorTileButtonPicked(salmonButton); });
    
    // darksalmon
    MyColorTileButton* darksalmonButton = new MyColorTileButton("DarkSalmon", "#E9967A");
    allColorsContentLayout->addWidget(darksalmonButton, rowCount++, columnCount);
    connect(darksalmonButton, &QPushButton::clicked, this, [this, darksalmonButton] () { this->colorTileButtonPicked(darksalmonButton); });
    
    // lightsalmon
    MyColorTileButton* lightsalmonButton = new MyColorTileButton("LightSalmon", "#FFA07A");
    allColorsContentLayout->addWidget(lightsalmonButton, rowCount++, columnCount);
    connect(lightsalmonButton, &QPushButton::clicked, this, [this, lightsalmonButton] () { this->colorTileButtonPicked(lightsalmonButton); });
    
    // orangered
    MyColorTileButton* orangeredButton = new MyColorTileButton("OrangeRed", "#FF4500");
    allColorsContentLayout->addWidget(orangeredButton, rowCount++, columnCount);
    connect(orangeredButton, &QPushButton::clicked, this, [this, orangeredButton] () { this->colorTileButtonPicked(orangeredButton); });
    
    // tomato
    MyColorTileButton* tomatoButton = new MyColorTileButton("Tomato", "#FF6347");
    allColorsContentLayout->addWidget(tomatoButton, rowCount++, columnCount);
    connect(tomatoButton, &QPushButton::clicked, this, [this, tomatoButton] () { this->colorTileButtonPicked(tomatoButton); });
    
    // darkorange
    MyColorTileButton* darkorangeButton = new MyColorTileButton("DarkOrange", "#FF8C00");
    allColorsContentLayout->addWidget(darkorangeButton, rowCount++, columnCount);
    connect(darkorangeButton, &QPushButton::clicked, this, [this, darkorangeButton] () { this->colorTileButtonPicked(darkorangeButton); });
    
    // coral
    MyColorTileButton* coralButton = new MyColorTileButton("Coral", "#FF7F50");
    allColorsContentLayout->addWidget(coralButton, rowCount++, columnCount);
    connect(coralButton, &QPushButton::clicked, this, [this, coralButton] () { this->colorTileButtonPicked(coralButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // orange
    MyColorTileButton* orangeButton = new MyColorTileButton("Orange", "#FFA500");
    allColorsContentLayout->addWidget(orangeButton, rowCount++, columnCount);
    connect(orangeButton, &QPushButton::clicked, this, [this, orangeButton] () { this->colorTileButtonPicked(orangeButton); });
    
    // mediumvioletred
    MyColorTileButton* mediumvioletredButton = new MyColorTileButton("MediumVioletRed", "#C71585");
    allColorsContentLayout->addWidget(mediumvioletredButton, rowCount++, columnCount);
    connect(mediumvioletredButton, &QPushButton::clicked, this, [this, mediumvioletredButton] () { this->colorTileButtonPicked(mediumvioletredButton); });
    
    // deeppink
    MyColorTileButton* deeppinkButton = new MyColorTileButton("DeepPink", "#FF1493");
    allColorsContentLayout->addWidget(deeppinkButton, rowCount++, columnCount);
    connect(deeppinkButton, &QPushButton::clicked, this, [this, deeppinkButton] () { this->colorTileButtonPicked(deeppinkButton); });
    
    // palevioletred
    MyColorTileButton* palevioletredButton = new MyColorTileButton("PaleVioletRed", "#DB7093");
    allColorsContentLayout->addWidget(palevioletredButton, rowCount++, columnCount);
    connect(palevioletredButton, &QPushButton::clicked, this, [this, palevioletredButton] () { this->colorTileButtonPicked(palevioletredButton); });
    
    // hotpink
    MyColorTileButton* hotpinkButton = new MyColorTileButton("HotPink", "#DB7093");
    allColorsContentLayout->addWidget(hotpinkButton, rowCount++, columnCount);
    connect(hotpinkButton, &QPushButton::clicked, this, [this, hotpinkButton] () { this->colorTileButtonPicked(hotpinkButton); });
    
    // lightpink
    MyColorTileButton* lightpinkButton = new MyColorTileButton("LightPink", "#FFB6C1");
    allColorsContentLayout->addWidget(lightpinkButton, rowCount++, columnCount);
    connect(lightpinkButton, &QPushButton::clicked, this, [this, lightpinkButton] () { this->colorTileButtonPicked(lightpinkButton); });
    
    // pink
    MyColorTileButton* pinkButton = new MyColorTileButton("Pink", "#FFC0CB");
    allColorsContentLayout->addWidget(pinkButton, rowCount++, columnCount);
    connect(pinkButton, &QPushButton::clicked, this, [this, pinkButton] () { this->colorTileButtonPicked(pinkButton); });
    
    // indigo
    MyColorTileButton* indigoButton = new MyColorTileButton("Indigo", "#4B0082");
    allColorsContentLayout->addWidget(indigoButton, rowCount++, columnCount);
    connect(indigoButton, &QPushButton::clicked, this, [this, indigoButton] () { this->colorTileButtonPicked(indigoButton); });
    
    // purple
    MyColorTileButton* purpleButton = new MyColorTileButton("Purple", "#800080");
    allColorsContentLayout->addWidget(purpleButton, rowCount++, columnCount);
    connect(purpleButton, &QPushButton::clicked, this, [this, purpleButton] () { this->colorTileButtonPicked(purpleButton); });
    
    // rebeccapurple
    //MyColorTileButton* rebeccapurpleButton = new MyColorTileButton("RebeccaPurple", "#663399");
    //allColorsContentLayout->addWidget(rebeccapurpleButton, rowCount++, columnCount);
    //connect(rebeccapurpleButton, &QPushButton::clicked, this, [this, rebeccapurpleButton] () { this->colorTileButtonPicked(rebeccapurpleButton); });
    
    // darkmagenta
    MyColorTileButton* darkmagentaButton = new MyColorTileButton("DarkMagenta", "#800080");
    allColorsContentLayout->addWidget(darkmagentaButton, rowCount++, columnCount);
    connect(darkmagentaButton, &QPushButton::clicked, this, [this, darkmagentaButton] () { this->colorTileButtonPicked(darkmagentaButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // darkviolet
    MyColorTileButton* darkvioletButton = new MyColorTileButton("DarkViolet", "#9400D3");
    allColorsContentLayout->addWidget(darkvioletButton, rowCount++, columnCount);
    connect(darkvioletButton, &QPushButton::clicked, this, [this, darkvioletButton] () { this->colorTileButtonPicked(darkvioletButton); });
    
    // darkslateblue
    MyColorTileButton* darkslateblueButton = new MyColorTileButton("DarkSlateBlue", "#483D8B");
    allColorsContentLayout->addWidget(darkslateblueButton, rowCount++, columnCount);
    connect(darkslateblueButton, &QPushButton::clicked, this, [this, darkslateblueButton] () { this->colorTileButtonPicked(darkslateblueButton); });
    
    // blueviolet
    MyColorTileButton* bluevioletButton = new MyColorTileButton("BlueViolet", "#8A2BE2");
    allColorsContentLayout->addWidget(bluevioletButton, rowCount++, columnCount);
    connect(bluevioletButton, &QPushButton::clicked, this, [this, bluevioletButton] () { this->colorTileButtonPicked(bluevioletButton); });
    
    // darkorchid
    MyColorTileButton* darkorchidButton = new MyColorTileButton("DarkOrchid", "#9932CC");
    allColorsContentLayout->addWidget(darkorchidButton, rowCount++, columnCount);
    connect(darkorchidButton, &QPushButton::clicked, this, [this, darkorchidButton] () { this->colorTileButtonPicked(darkorchidButton); });
    
    // fuchsia
    MyColorTileButton* fuchsiaButton = new MyColorTileButton("Fuchsia", "#FF00FF");
    allColorsContentLayout->addWidget(fuchsiaButton, rowCount++, columnCount);
    connect(fuchsiaButton, &QPushButton::clicked, this, [this, fuchsiaButton] () { this->colorTileButtonPicked(fuchsiaButton); });
    
    // magenta
    MyColorTileButton* magentaButton = new MyColorTileButton("Magenta", "#FF00FF");
    allColorsContentLayout->addWidget(magentaButton, rowCount++, columnCount);
    connect(magentaButton, &QPushButton::clicked, this, [this, magentaButton] () { this->colorTileButtonPicked(magentaButton); });
    
    // slateblue
    MyColorTileButton* slateblueButton = new MyColorTileButton("SlateBlue", "#6A5ACD");
    allColorsContentLayout->addWidget(slateblueButton, rowCount++, columnCount);
    connect(slateblueButton, &QPushButton::clicked, this, [this, slateblueButton] () { this->colorTileButtonPicked(slateblueButton); });
    
    // mediumslateblue
    MyColorTileButton* mediumslateblueButton = new MyColorTileButton("MediumSlateBlue", "#7B68EE");
    allColorsContentLayout->addWidget(mediumslateblueButton, rowCount++, columnCount);
    connect(mediumslateblueButton, &QPushButton::clicked, this, [this, mediumslateblueButton] () { this->colorTileButtonPicked(mediumslateblueButton); });
    
    // mediumorchid
    MyColorTileButton* mediumorchidButton = new MyColorTileButton("MediumOrchid", "#BA55D3");
    allColorsContentLayout->addWidget(mediumorchidButton, rowCount++, columnCount);
    connect(mediumorchidButton, &QPushButton::clicked, this, [this, mediumorchidButton] () { this->colorTileButtonPicked(mediumorchidButton); });
    
    // mediumpurple
    MyColorTileButton* mediumpurpleButton = new MyColorTileButton("MediumPurple", "#9370DB");
    allColorsContentLayout->addWidget(mediumpurpleButton, rowCount++, columnCount);
    connect(mediumpurpleButton, &QPushButton::clicked, this, [this, mediumpurpleButton] () { this->colorTileButtonPicked(mediumpurpleButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // orchid
    MyColorTileButton* orchidButton = new MyColorTileButton("Orchid", "#DA70D6");
    allColorsContentLayout->addWidget(orchidButton, rowCount++, columnCount);
    connect(orchidButton, &QPushButton::clicked, this, [this, orchidButton] () { this->colorTileButtonPicked(orchidButton); });
    
    // violet
    MyColorTileButton* violetButton = new MyColorTileButton("Violet", "#EE82EE");
    allColorsContentLayout->addWidget(violetButton, rowCount++, columnCount);
    connect(violetButton, &QPushButton::clicked, this, [this, violetButton] () { this->colorTileButtonPicked(violetButton); });
    
    // plum
    MyColorTileButton* plumButton = new MyColorTileButton("Plum", "#DDA0DD");
    allColorsContentLayout->addWidget(plumButton, rowCount++, columnCount);
    connect(plumButton, &QPushButton::clicked, this, [this, plumButton] () { this->colorTileButtonPicked(plumButton); });
    
    // thistle
    MyColorTileButton* thistleButton = new MyColorTileButton("Thistle", "#D8BFD8");
    allColorsContentLayout->addWidget(thistleButton, rowCount++, columnCount);
    connect(thistleButton, &QPushButton::clicked, this, [this, thistleButton] () { this->colorTileButtonPicked(thistleButton); });
    
    // lavender
    MyColorTileButton* lavenderButton = new MyColorTileButton("Lavender", "#E6E6FA");
    allColorsContentLayout->addWidget(lavenderButton, rowCount++, columnCount);
    connect(lavenderButton, &QPushButton::clicked, this, [this, lavenderButton] () { this->colorTileButtonPicked(lavenderButton); });
    
    // navy
    MyColorTileButton* navyButton = new MyColorTileButton("Navy", "#000080");
    allColorsContentLayout->addWidget(navyButton, rowCount++, columnCount);
    connect(navyButton, &QPushButton::clicked, this, [this, navyButton] () { this->colorTileButtonPicked(navyButton); });
    
    // darkblue
    MyColorTileButton* darkblueButton = new MyColorTileButton("DarkBlue", "#00008B");
    allColorsContentLayout->addWidget(darkblueButton, rowCount++, columnCount);
    connect(darkblueButton, &QPushButton::clicked, this, [this, darkblueButton] () { this->colorTileButtonPicked(darkblueButton); });
    
    // mediumblue
    MyColorTileButton* mediumblueButton = new MyColorTileButton("MediumBlue", "#0000CD");
    allColorsContentLayout->addWidget(mediumblueButton, rowCount++, columnCount);
    connect(mediumblueButton, &QPushButton::clicked, this, [this, mediumblueButton] () { this->colorTileButtonPicked(mediumblueButton); });
    
    // blue
    MyColorTileButton* blueButton = new MyColorTileButton("Blue", "#0000FF");
    allColorsContentLayout->addWidget(blueButton, rowCount++, columnCount);
    connect(blueButton, &QPushButton::clicked, this, [this, blueButton] () { this->colorTileButtonPicked(blueButton); });
    
    // midnightblue
    MyColorTileButton* midnightblueButton = new MyColorTileButton("MidnightBlue", "#191970");
    allColorsContentLayout->addWidget(midnightblueButton, rowCount++, columnCount);
    connect(midnightblueButton, &QPushButton::clicked, this, [this, midnightblueButton] () { this->colorTileButtonPicked(midnightblueButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // royalblue
    MyColorTileButton* royalblueButton = new MyColorTileButton("RoyalBlue", "#4169E1");
    allColorsContentLayout->addWidget(royalblueButton, rowCount++, columnCount);
    connect(royalblueButton, &QPushButton::clicked, this, [this, royalblueButton] () { this->colorTileButtonPicked(royalblueButton); });
    
    // steelblue
    MyColorTileButton* steelblueButton = new MyColorTileButton("SteelBlue", "#4682B4");
    allColorsContentLayout->addWidget(steelblueButton, rowCount++, columnCount);
    connect(steelblueButton, &QPushButton::clicked, this, [this, steelblueButton] () { this->colorTileButtonPicked(steelblueButton); });
    
    // dodgerblue
    MyColorTileButton* dodgerblueButton = new MyColorTileButton("DodgerBlue", "#1E90FF");
    allColorsContentLayout->addWidget(dodgerblueButton, rowCount++, columnCount);
    connect(dodgerblueButton, &QPushButton::clicked, this, [this, dodgerblueButton] () { this->colorTileButtonPicked(dodgerblueButton); });
    
    // deepskyblue
    MyColorTileButton* deepskyblueButton = new MyColorTileButton("DeepSkyBlue", "#00BFFF");
    allColorsContentLayout->addWidget(deepskyblueButton, rowCount++, columnCount);
    connect(deepskyblueButton, &QPushButton::clicked, this, [this, deepskyblueButton] () { this->colorTileButtonPicked(deepskyblueButton); });
    
    // cornflowerblue
    MyColorTileButton* cornflowerblueButton = new MyColorTileButton("CornflowerBlue", "#6495ED");
    allColorsContentLayout->addWidget(cornflowerblueButton, rowCount++, columnCount);
    connect(cornflowerblueButton, &QPushButton::clicked, this, [this, cornflowerblueButton] () { this->colorTileButtonPicked(cornflowerblueButton); });
    
    // skyblue
    MyColorTileButton* skyblueButton = new MyColorTileButton("SkyBlue", "#87CEEB");
    allColorsContentLayout->addWidget(skyblueButton, rowCount++, columnCount);
    connect(skyblueButton, &QPushButton::clicked, this, [this, skyblueButton] () { this->colorTileButtonPicked(skyblueButton); });
    
    // lightskyblue
    MyColorTileButton* lightskyblueButton = new MyColorTileButton("LightSkyBlue", "#87CEFA");
    allColorsContentLayout->addWidget(lightskyblueButton, rowCount++, columnCount);
    connect(lightskyblueButton, &QPushButton::clicked, this, [this, lightskyblueButton] () { this->colorTileButtonPicked(lightskyblueButton); });
    
    // lightsteelblue
    MyColorTileButton* lightsteelblueButton = new MyColorTileButton("LightSteelBlue", "#B0C4DE");
    allColorsContentLayout->addWidget(lightsteelblueButton, rowCount++, columnCount);
    connect(lightsteelblueButton, &QPushButton::clicked, this, [this, lightsteelblueButton] () { this->colorTileButtonPicked(lightsteelblueButton); });
    
    // lightblue
    MyColorTileButton* lightblueButton = new MyColorTileButton("LightBlue", "#ADD8E6");
    allColorsContentLayout->addWidget(lightblueButton, rowCount++, columnCount);
    connect(lightblueButton, &QPushButton::clicked, this, [this, lightblueButton] () { this->colorTileButtonPicked(lightblueButton); });
    
    // powderblue
    MyColorTileButton* powderblueButton = new MyColorTileButton("PowderBlue", "#B0E0E6");
    allColorsContentLayout->addWidget(powderblueButton, rowCount++, columnCount);
    connect(powderblueButton, &QPushButton::clicked, this, [this, powderblueButton] () { this->colorTileButtonPicked(powderblueButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // teal
    MyColorTileButton* tealButton = new MyColorTileButton("Teal", "#008080");
    allColorsContentLayout->addWidget(tealButton, rowCount++, columnCount);
    connect(tealButton, &QPushButton::clicked, this, [this, tealButton] () { this->colorTileButtonPicked(tealButton); });
    
    // darkcyan
    MyColorTileButton* darkcyanButton = new MyColorTileButton("DarkCyan", "#008B8B");
    allColorsContentLayout->addWidget(darkcyanButton, rowCount++, columnCount);
    connect(darkcyanButton, &QPushButton::clicked, this, [this, darkcyanButton] () { this->colorTileButtonPicked(darkcyanButton); });
    
    // lightseagreen
    MyColorTileButton* lightseagreenButton = new MyColorTileButton("LightSeaGreen", "#20B2AA");
    allColorsContentLayout->addWidget(lightseagreenButton, rowCount++, columnCount);
    connect(lightseagreenButton, &QPushButton::clicked, this, [this, lightseagreenButton] () { this->colorTileButtonPicked(lightseagreenButton); });
    
    // cadetblue
    MyColorTileButton* cadetblueButton = new MyColorTileButton("CadetBlue", "#5F9EA0");
    allColorsContentLayout->addWidget(cadetblueButton, rowCount++, columnCount);
    connect(cadetblueButton, &QPushButton::clicked, this, [this, cadetblueButton] () { this->colorTileButtonPicked(cadetblueButton); });
    
    // darkturquoise
    MyColorTileButton* darkturquoiseButton = new MyColorTileButton("DarkTurquoise", "#00CED1");
    allColorsContentLayout->addWidget(darkturquoiseButton, rowCount++, columnCount);
    connect(darkturquoiseButton, &QPushButton::clicked, this, [this, darkturquoiseButton] () { this->colorTileButtonPicked(darkturquoiseButton); });
    
    // mediumturquoise
    MyColorTileButton* mediumturquoiseButton = new MyColorTileButton("MediumTurquoise", "#48D1CC");
    allColorsContentLayout->addWidget(mediumturquoiseButton, rowCount++, columnCount);
    connect(mediumturquoiseButton, &QPushButton::clicked, this, [this, mediumturquoiseButton] () { this->colorTileButtonPicked(mediumturquoiseButton); });
    
    // turquoise
    MyColorTileButton* turquoiseButton = new MyColorTileButton("Turquoise", "#40E0D0");
    allColorsContentLayout->addWidget(turquoiseButton, rowCount++, columnCount);
    connect(turquoiseButton, &QPushButton::clicked, this, [this, turquoiseButton] () { this->colorTileButtonPicked(turquoiseButton); });
    
    // aqua
    MyColorTileButton* aquaButton = new MyColorTileButton("Aqua", "#00FFFF");
    allColorsContentLayout->addWidget(aquaButton, rowCount++, columnCount);
    connect(aquaButton, &QPushButton::clicked, this, [this, aquaButton] () { this->colorTileButtonPicked(aquaButton); });
    
    // cyan
    MyColorTileButton* cyanButton = new MyColorTileButton("Cyan", "#00FFFF");
    allColorsContentLayout->addWidget(cyanButton, rowCount++, columnCount);
    connect(cyanButton, &QPushButton::clicked, this, [this, cyanButton] () { this->colorTileButtonPicked(cyanButton); });
    
    // aquamarine
    MyColorTileButton* aquamarineButton = new MyColorTileButton("Aquamarine", "#7FFFD4");
    allColorsContentLayout->addWidget(aquamarineButton, rowCount++, columnCount);
    connect(aquamarineButton, &QPushButton::clicked, this, [this, aquamarineButton] () { this->colorTileButtonPicked(aquamarineButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // paleturquoise
    MyColorTileButton* paleturquoiseButton = new MyColorTileButton("PaleTurquoise", "#AFEEEE");
    allColorsContentLayout->addWidget(paleturquoiseButton, rowCount++, columnCount);
    connect(paleturquoiseButton, &QPushButton::clicked, this, [this, paleturquoiseButton] () { this->colorTileButtonPicked(paleturquoiseButton); });
    
    // lightcyan
    MyColorTileButton* lightcyanButton = new MyColorTileButton("LightCyan", "#E0FFFF");
    allColorsContentLayout->addWidget(lightcyanButton, rowCount++, columnCount);
    connect(lightcyanButton, &QPushButton::clicked, this, [this, lightcyanButton] () { this->colorTileButtonPicked(lightcyanButton); });
    
    // darkgreen
    MyColorTileButton* darkgreenButton = new MyColorTileButton("DarkGreen", "#006400");
    allColorsContentLayout->addWidget(darkgreenButton, rowCount++, columnCount);
    connect(darkgreenButton, &QPushButton::clicked, this, [this, darkgreenButton] () { this->colorTileButtonPicked(darkgreenButton); });
    
    // green
    MyColorTileButton* greenButton = new MyColorTileButton("Green", "#008000");
    allColorsContentLayout->addWidget(greenButton, rowCount++, columnCount);
    connect(greenButton, &QPushButton::clicked, this, [this, greenButton] () { this->colorTileButtonPicked(greenButton); });
    
    // darkolivegreen
    MyColorTileButton* darkolivegreenButton = new MyColorTileButton("DarkOliveGreen", "#556B2F");
    allColorsContentLayout->addWidget(darkolivegreenButton, rowCount++, columnCount);
    connect(darkolivegreenButton, &QPushButton::clicked, this, [this, darkolivegreenButton] () { this->colorTileButtonPicked(darkolivegreenButton); });
    
    // forestgreen
    MyColorTileButton* forestgreenButton = new MyColorTileButton("ForestGreen", "#228B22");
    allColorsContentLayout->addWidget(forestgreenButton, rowCount++, columnCount);
    connect(forestgreenButton, &QPushButton::clicked, this, [this, forestgreenButton] () { this->colorTileButtonPicked(forestgreenButton); });
    
    // seagreen
    MyColorTileButton* seagreenButton = new MyColorTileButton("SeaGreen", "#2E8B57");
    allColorsContentLayout->addWidget(seagreenButton, rowCount++, columnCount);
    connect(seagreenButton, &QPushButton::clicked, this, [this, seagreenButton] () { this->colorTileButtonPicked(seagreenButton); });
    
    // olive
    MyColorTileButton* oliveButton = new MyColorTileButton("Olive", "#808000");
    allColorsContentLayout->addWidget(oliveButton, rowCount++, columnCount);
    connect(oliveButton, &QPushButton::clicked, this, [this, oliveButton] () { this->colorTileButtonPicked(oliveButton); });
    
    // olivedrab
    MyColorTileButton* olivedrabButton = new MyColorTileButton("OliveDrab", "#6B8E23");
    allColorsContentLayout->addWidget(olivedrabButton, rowCount++, columnCount);
    connect(olivedrabButton, &QPushButton::clicked, this, [this, olivedrabButton] () { this->colorTileButtonPicked(olivedrabButton); });
    
    // mediumseagreen
    MyColorTileButton* mediumseagreenButton = new MyColorTileButton("MediumSeaGreen", "#3CB371");
    allColorsContentLayout->addWidget(mediumseagreenButton, rowCount++, columnCount);
    connect(mediumseagreenButton, &QPushButton::clicked, this, [this, mediumseagreenButton] () { this->colorTileButtonPicked(mediumseagreenButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // limegreen
    MyColorTileButton* limegreenButton = new MyColorTileButton("LimeGreen", "#32CD32");
    allColorsContentLayout->addWidget(limegreenButton, rowCount++, columnCount);
    connect(limegreenButton, &QPushButton::clicked, this, [this, limegreenButton] () { this->colorTileButtonPicked(limegreenButton); });
    
    // lime
    MyColorTileButton* limeButton = new MyColorTileButton("Lime", "#00FF00");
    allColorsContentLayout->addWidget(limeButton, rowCount++, columnCount);
    connect(limeButton, &QPushButton::clicked, this, [this, limeButton] () { this->colorTileButtonPicked(limeButton); });
    
    // springgreen
    MyColorTileButton* springgreenButton = new MyColorTileButton("SpringGreen", "#00FF7F");
    allColorsContentLayout->addWidget(springgreenButton, rowCount++, columnCount);
    connect(springgreenButton, &QPushButton::clicked, this, [this, springgreenButton] () { this->colorTileButtonPicked(springgreenButton); });
    
    // mediumspringgreen
    MyColorTileButton* mediumspringgreenButton = new MyColorTileButton("MediumSpringGreen", "#00FA9A");
    allColorsContentLayout->addWidget(mediumspringgreenButton, rowCount++, columnCount);
    connect(mediumspringgreenButton, &QPushButton::clicked, this, [this, mediumspringgreenButton] () { this->colorTileButtonPicked(mediumspringgreenButton); });
    
    // darkseagreen
    MyColorTileButton* darkseagreenButton = new MyColorTileButton("DarkSeaGreen", "#8FBC8F");
    allColorsContentLayout->addWidget(darkseagreenButton, rowCount++, columnCount);
    connect(darkseagreenButton, &QPushButton::clicked, this, [this, darkseagreenButton] () { this->colorTileButtonPicked(darkseagreenButton); });
    
    // mediumaquamarine
    MyColorTileButton* mediumaquamarineButton = new MyColorTileButton("MediumAquamarine", "#66CDAA");
    allColorsContentLayout->addWidget(mediumaquamarineButton, rowCount++, columnCount);
    connect(mediumaquamarineButton, &QPushButton::clicked, this, [this, mediumaquamarineButton] () { this->colorTileButtonPicked(mediumaquamarineButton); });
    
    // yellowgreen
    MyColorTileButton* yellowgreenButton = new MyColorTileButton("YellowGreen", "#9ACD32");
    allColorsContentLayout->addWidget(yellowgreenButton, rowCount++, columnCount);
    connect(yellowgreenButton, &QPushButton::clicked, this, [this, yellowgreenButton] () { this->colorTileButtonPicked(yellowgreenButton); });
    
    // lawngreen
    MyColorTileButton* lawngreenButton = new MyColorTileButton("LawnGreen", "#7CFC00");
    allColorsContentLayout->addWidget(lawngreenButton, rowCount++, columnCount);
    connect(lawngreenButton, &QPushButton::clicked, this, [this, lawngreenButton] () { this->colorTileButtonPicked(lawngreenButton); });
    
    // chartreuse
    MyColorTileButton* chartreuseButton = new MyColorTileButton("Chartreuse", "#7FFF00");
    allColorsContentLayout->addWidget(chartreuseButton, rowCount++, columnCount);
    connect(chartreuseButton, &QPushButton::clicked, this, [this, chartreuseButton] () { this->colorTileButtonPicked(chartreuseButton); });
    
    // lightgreen
    MyColorTileButton* lightgreenButton = new MyColorTileButton("LightGreen", "#90EE90");
    allColorsContentLayout->addWidget(lightgreenButton, rowCount++, columnCount);
    connect(lightgreenButton, &QPushButton::clicked, this, [this, lightgreenButton] () { this->colorTileButtonPicked(lightgreenButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // greenyellow
    MyColorTileButton* greenyellowButton = new MyColorTileButton("GreenYellow", "#ADFF2F");
    allColorsContentLayout->addWidget(greenyellowButton, rowCount++, columnCount);
    connect(greenyellowButton, &QPushButton::clicked, this, [this, greenyellowButton] () { this->colorTileButtonPicked(greenyellowButton); });
    
    // palegreen
    MyColorTileButton* palegreenButton = new MyColorTileButton("PaleGreen", "#98FB98");
    allColorsContentLayout->addWidget(palegreenButton, rowCount++, columnCount);
    connect(palegreenButton, &QPushButton::clicked, this, [this, palegreenButton] () { this->colorTileButtonPicked(palegreenButton); });
    
    // darkkhaki
    MyColorTileButton* darkkhakiButton = new MyColorTileButton("DarkKhaki", "#BDB76B");
    allColorsContentLayout->addWidget(darkkhakiButton, rowCount++, columnCount);
    connect(darkkhakiButton, &QPushButton::clicked, this, [this, darkkhakiButton] () { this->colorTileButtonPicked(darkkhakiButton); });
    
    // gold
    MyColorTileButton* goldButton = new MyColorTileButton("Gold", "#FFD700");
    allColorsContentLayout->addWidget(goldButton, rowCount++, columnCount);
    connect(goldButton, &QPushButton::clicked, this, [this, goldButton] () { this->colorTileButtonPicked(goldButton); });
    
    // khaki
    MyColorTileButton* khakiButton = new MyColorTileButton("Khaki", "#F0E68C");
    allColorsContentLayout->addWidget(khakiButton, rowCount++, columnCount);
    connect(khakiButton, &QPushButton::clicked, this, [this, khakiButton] () { this->colorTileButtonPicked(khakiButton); });
    
    // peachpuff
    MyColorTileButton* peachpuffButton = new MyColorTileButton("PeachPuff", "#FFDAB9");
    allColorsContentLayout->addWidget(peachpuffButton, rowCount++, columnCount);
    connect(peachpuffButton, &QPushButton::clicked, this, [this, peachpuffButton] () { this->colorTileButtonPicked(peachpuffButton); });
    
    // yellow
    MyColorTileButton* yellowButton = new MyColorTileButton("Yellow", "#FFFF00");
    allColorsContentLayout->addWidget(yellowButton, rowCount++, columnCount);
    connect(yellowButton, &QPushButton::clicked, this, [this, yellowButton] () { this->colorTileButtonPicked(yellowButton); });
    
    // palegoldenrod
    MyColorTileButton* palegoldenrodButton = new MyColorTileButton("PaleGoldenrod", "#EEE8AA");
    allColorsContentLayout->addWidget(palegoldenrodButton, rowCount++, columnCount);
    connect(palegoldenrodButton, &QPushButton::clicked, this, [this, palegoldenrodButton] () { this->colorTileButtonPicked(palegoldenrodButton); });
    
    // moccasin
    MyColorTileButton* moccasinButton = new MyColorTileButton("Moccasin", "#FFE4B5");
    allColorsContentLayout->addWidget(moccasinButton, rowCount++, columnCount);
    connect(moccasinButton, &QPushButton::clicked, this, [this, moccasinButton] () { this->colorTileButtonPicked(moccasinButton); });
    
    // papayawhip
    MyColorTileButton* papayawhipButton = new MyColorTileButton("PapayaWhip", "#FFEFD5");
    allColorsContentLayout->addWidget(papayawhipButton, rowCount++, columnCount);
    connect(papayawhipButton, &QPushButton::clicked, this, [this, papayawhipButton] () { this->colorTileButtonPicked(papayawhipButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // lightgoldenrodyellow
    MyColorTileButton* lightgoldenrodyellowButton = new MyColorTileButton("LightGoldenrodYellow", "#FAFAD2");
    allColorsContentLayout->addWidget(lightgoldenrodyellowButton, rowCount++, columnCount);
    connect(lightgoldenrodyellowButton, &QPushButton::clicked, this, [this, lightgoldenrodyellowButton] () { this->colorTileButtonPicked(lightgoldenrodyellowButton); });
    
    // lemonchiffon
    MyColorTileButton* lemonchiffonButton = new MyColorTileButton("LemonChiffon", "#FFFACD");
    allColorsContentLayout->addWidget(lemonchiffonButton, rowCount++, columnCount);
    connect(lemonchiffonButton, &QPushButton::clicked, this, [this, lemonchiffonButton] () { this->colorTileButtonPicked(lemonchiffonButton); });
    
    // lightyellow
    MyColorTileButton* lightyellowButton = new MyColorTileButton("LightYellow", "#FFFFE0");
    allColorsContentLayout->addWidget(lightyellowButton, rowCount++, columnCount);
    connect(lightyellowButton, &QPushButton::clicked, this, [this, lightyellowButton] () { this->colorTileButtonPicked(lightyellowButton); });
    
    // mistyrose
    MyColorTileButton* mistyroseButton = new MyColorTileButton("MistyRose", "#FFE4E1");
    allColorsContentLayout->addWidget(mistyroseButton, rowCount++, columnCount);
    connect(mistyroseButton, &QPushButton::clicked, this, [this, mistyroseButton] () { this->colorTileButtonPicked(mistyroseButton); });
    
    // antiquewhite
    MyColorTileButton* antiquewhiteButton = new MyColorTileButton("AntiqueWhite", "#FAEBD7");
    allColorsContentLayout->addWidget(antiquewhiteButton, rowCount++, columnCount);
    connect(antiquewhiteButton, &QPushButton::clicked, this, [this, antiquewhiteButton] () { this->colorTileButtonPicked(antiquewhiteButton); });
    
    // linen
    MyColorTileButton* linenButton = new MyColorTileButton("Linen", "#FAF0E6");
    allColorsContentLayout->addWidget(linenButton, rowCount++, columnCount);
    connect(linenButton, &QPushButton::clicked, this, [this, linenButton] () { this->colorTileButtonPicked(linenButton); });
    
    // beige
    MyColorTileButton* beigeButton = new MyColorTileButton("Beige", "#F5F5DC");
    allColorsContentLayout->addWidget(beigeButton, rowCount++, columnCount);
    connect(beigeButton, &QPushButton::clicked, this, [this, beigeButton] () { this->colorTileButtonPicked(beigeButton); });
    
    // whitesmoke
    MyColorTileButton* whitesmokeButton = new MyColorTileButton("WhiteSmoke", "#F5F5F5");
    allColorsContentLayout->addWidget(whitesmokeButton, rowCount++, columnCount);
    connect(whitesmokeButton, &QPushButton::clicked, this, [this, whitesmokeButton] () { this->colorTileButtonPicked(whitesmokeButton); });
    
    // lavenderblush
    MyColorTileButton* lavenderblushButton = new MyColorTileButton("LavenderBlush", "#FFF0F5");
    allColorsContentLayout->addWidget(lavenderblushButton, rowCount++, columnCount);
    connect(lavenderblushButton, &QPushButton::clicked, this, [this, lavenderblushButton] () { this->colorTileButtonPicked(lavenderblushButton); });
    
    // oldlace
    MyColorTileButton* oldlaceButton = new MyColorTileButton("OldLace", "#FDF5E6");
    allColorsContentLayout->addWidget(oldlaceButton, rowCount++, columnCount);
    connect(oldlaceButton, &QPushButton::clicked, this, [this, oldlaceButton] () { this->colorTileButtonPicked(oldlaceButton); });
    
    rowCount = 2;
    ++columnCount;
    
    // aliceblue
    MyColorTileButton* aliceblueButton = new MyColorTileButton("AliceBlue", "#F0F8FF");
    allColorsContentLayout->addWidget(aliceblueButton, rowCount++, columnCount);
    connect(aliceblueButton, &QPushButton::clicked, this, [this, aliceblueButton] () { this->colorTileButtonPicked(aliceblueButton); });
    
    // seashell
    MyColorTileButton* seashellButton = new MyColorTileButton("Seashell", "#FFF5EE");
    allColorsContentLayout->addWidget(seashellButton, rowCount++, columnCount);
    connect(seashellButton, &QPushButton::clicked, this, [this, seashellButton] () { this->colorTileButtonPicked(seashellButton); });
    
    // ghostwhite
    MyColorTileButton* ghostwhiteButton = new MyColorTileButton("GhostWhite", "#F8F8FF");
    allColorsContentLayout->addWidget(ghostwhiteButton, rowCount++, columnCount);
    connect(ghostwhiteButton, &QPushButton::clicked, this, [this, ghostwhiteButton] () { this->colorTileButtonPicked(ghostwhiteButton); });
    
    // honeydew
    MyColorTileButton* honeydewButton = new MyColorTileButton("Honeydew", "#F0FFF0");
    allColorsContentLayout->addWidget(honeydewButton, rowCount++, columnCount);
    connect(honeydewButton, &QPushButton::clicked, this, [this, honeydewButton] () { this->colorTileButtonPicked(honeydewButton); });
    
    // floralwhite
    MyColorTileButton* floralwhiteButton = new MyColorTileButton("FloralWhite", "#FFFAF0");
    allColorsContentLayout->addWidget(floralwhiteButton, rowCount++, columnCount);
    connect(floralwhiteButton, &QPushButton::clicked, this, [this, floralwhiteButton] () { this->colorTileButtonPicked(floralwhiteButton); });
    
    // azure
    MyColorTileButton* azureButton = new MyColorTileButton("Azure", "#F0FFFF");
    allColorsContentLayout->addWidget(azureButton, rowCount++, columnCount);
    connect(azureButton, &QPushButton::clicked, this, [this, azureButton] () { this->colorTileButtonPicked(azureButton); });
    
    // mintcream
    MyColorTileButton* mintcreamButton = new MyColorTileButton("MintCream", "#F5FFFA");
    allColorsContentLayout->addWidget(mintcreamButton, rowCount++, columnCount);
    connect(mintcreamButton, &QPushButton::clicked, this, [this, mintcreamButton] () { this->colorTileButtonPicked(mintcreamButton); });
    
    // snow
    MyColorTileButton* snowButton = new MyColorTileButton("Snow", "#FFFAFA");
    allColorsContentLayout->addWidget(snowButton, rowCount++, columnCount);
    connect(snowButton, &QPushButton::clicked, this, [this, snowButton] () { this->colorTileButtonPicked(snowButton); });
    
    // ivory
    MyColorTileButton* ivoryButton = new MyColorTileButton("Ivory", "#FFFFF0");
    allColorsContentLayout->addWidget(ivoryButton, rowCount++, columnCount);
    connect(ivoryButton, &QPushButton::clicked, this, [this, ivoryButton] () { this->colorTileButtonPicked(ivoryButton); });
    
    // white
    MyColorTileButton* whiteButton = new MyColorTileButton("White", "#FFFFFF");
    allColorsContentLayout->addWidget(whiteButton, rowCount++, columnCount);
    connect(whiteButton, &QPushButton::clicked, this, [this, whiteButton] () { this->colorTileButtonPicked(whiteButton); });
    
    allColorsWidget->setLayout(allColorsContentLayout);
    allColorsWidget->setContentsMargins(0, 0, 0, 0);
    allColors->setDefaultWidget(allColorsWidget);
    addAction(allColors);
    
    /// frequent colors
    QWidgetAction* frequentColors = new QWidgetAction(this);
    QWidget* frequentColorsWidget = new QWidget(this);
    QGridLayout* frequentColorsContentLayout = new QGridLayout(frequentColorsWidget);
    frequentColorsContentLayout->setSpacing(1);
    
    // label
    frequentColorsContentLayout->addWidget(new MyLabel("Frequent Colors"), frequentColorsContentLayout->rowCount() - 1, 0, 1, 14, Qt::AlignLeft);
    
    frequentColorsContentLayout->addItem(new QSpacerItem(1, 10), frequentColorsContentLayout->rowCount(), 0, 1, 14);
    
    rowCount = 2;
    columnCount = 0;
    
    // black
    MyColorTileButton* balckMainButton = new MyColorTileButton("Black", "#000000");
    frequentColorsContentLayout->addWidget(balckMainButton, rowCount, columnCount++);
    connect(balckMainButton, &QPushButton::clicked, this, [this, balckMainButton] () { this->colorTileButtonPicked(balckMainButton); });
    
    // gray
    MyColorTileButton* grayMainButton = new MyColorTileButton("Gray", "#808080");
    frequentColorsContentLayout->addWidget(grayMainButton, rowCount, columnCount++);
    connect(grayMainButton, &QPushButton::clicked, this, [this, grayMainButton] () { this->colorTileButtonPicked(grayMainButton); });
    
    // brown
    MyColorTileButton* brownMainButton = new MyColorTileButton("Brown", "#A52A2A");
    frequentColorsContentLayout->addWidget(brownMainButton, rowCount, columnCount++);
    connect(brownMainButton, &QPushButton::clicked, this, [this, brownMainButton] () { this->colorTileButtonPicked(brownMainButton); });
    
    // red
    MyColorTileButton* redMainButton = new MyColorTileButton("Red", "#FF0000");
    frequentColorsContentLayout->addWidget(redMainButton, rowCount, columnCount++);
    connect(redMainButton, &QPushButton::clicked, this, [this, redMainButton] () { this->colorTileButtonPicked(redMainButton); });
    
    // orange
    MyColorTileButton* orangeMainButton = new MyColorTileButton("Orange", "#FFA500");
    frequentColorsContentLayout->addWidget(orangeMainButton, rowCount, columnCount++);
    connect(orangeMainButton, &QPushButton::clicked, this, [this, orangeMainButton] () { this->colorTileButtonPicked(orangeMainButton); });
    
    // pink
    MyColorTileButton* pinkMainButton = new MyColorTileButton("Pink", "#FFC0CB");
    frequentColorsContentLayout->addWidget(pinkMainButton, rowCount, columnCount++);
    connect(pinkMainButton, &QPushButton::clicked, this, [this, pinkMainButton] () { this->colorTileButtonPicked(pinkMainButton); });
    
    // purple
    MyColorTileButton* purpleMainButton = new MyColorTileButton("Purple", "#800080");
    frequentColorsContentLayout->addWidget(purpleMainButton, rowCount, columnCount++);
    connect(purpleMainButton, &QPushButton::clicked, this, [this, purpleMainButton] () { this->colorTileButtonPicked(purpleMainButton); });
    
    // magenta
    MyColorTileButton* magentaMainButton = new MyColorTileButton("Magenta", "#FF00FF");
    frequentColorsContentLayout->addWidget(magentaMainButton, rowCount, columnCount++);
    connect(magentaMainButton, &QPushButton::clicked, this, [this, magentaMainButton] () { this->colorTileButtonPicked(magentaMainButton); });
    
    // violet
    MyColorTileButton* violetMainButton = new MyColorTileButton("Violet", "#EE82EE");
    frequentColorsContentLayout->addWidget(violetMainButton, rowCount, columnCount++);
    connect(violetMainButton, &QPushButton::clicked, this, [this, violetMainButton] () { this->colorTileButtonPicked(violetMainButton); });
    
    // blue
    MyColorTileButton* blueMainButton = new MyColorTileButton("Blue", "#0000FF");
    frequentColorsContentLayout->addWidget(blueMainButton, rowCount, columnCount++);
    connect(blueMainButton, &QPushButton::clicked, this, [this, blueMainButton] () { this->colorTileButtonPicked(blueMainButton); });
    
    // cyan
    MyColorTileButton* cyanMainButton = new MyColorTileButton("Cyan", "#00FFFF");
    frequentColorsContentLayout->addWidget(cyanMainButton, rowCount, columnCount++);
    connect(cyanMainButton, &QPushButton::clicked, this, [this, cyanMainButton] () { this->colorTileButtonPicked(cyanMainButton); });
    
    // green
    MyColorTileButton* greenMainButton = new MyColorTileButton("Green", "#008000");
    frequentColorsContentLayout->addWidget(greenMainButton, rowCount, columnCount++);
    connect(greenMainButton, &QPushButton::clicked, this, [this, greenMainButton] () { this->colorTileButtonPicked(greenMainButton); });
    
    // yellow
    MyColorTileButton* yellowMainButton = new MyColorTileButton("Yellow", "#FFFF00");
    frequentColorsContentLayout->addWidget(yellowMainButton, rowCount, columnCount++);
    connect(yellowMainButton, &QPushButton::clicked, this, [this, yellowMainButton] () { this->colorTileButtonPicked(yellowMainButton); });
    
    // white
    MyColorTileButton* whiteMainButton = new MyColorTileButton("White", "#FFFFFF");
    frequentColorsContentLayout->addWidget(whiteMainButton, rowCount, columnCount++);
    connect(whiteMainButton, &QPushButton::clicked, this, [this, whiteMainButton] () { this->colorTileButtonPicked(whiteMainButton); });
    
    frequentColorsWidget->setLayout(frequentColorsContentLayout);
    frequentColorsWidget->setContentsMargins(0, 0, 0, 0);
    frequentColors->setDefaultWidget(frequentColorsWidget);
    addAction(frequentColors);
}

void MyColorPickerMenu::colorTileButtonPicked(QPushButton* colorTileButton) {
    if (colorTileButton) {
        close();
        emit colorChosen(((MyColorTileButton*)colorTileButton)->color());
    }
}

// MyColorTileButton

MyColorTileButton::MyColorTileButton(const QString& color, const QString& value, QWidget* parent) : QPushButton(parent) {
    _color = color;
    _value = value;
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet(" QPushButton {background-color: " + _color + "; border-radius: 1px;} QToolTip { background-color: white;}");
    setToolTip(_color);
    setFixedWidth(12);
    setFixedHeight(12);
}

const QString& MyColorTileButton::color() const {
    return _color;
}

const QString& MyColorTileButton::value() const {
    return _value;
}

// MySpacerItem

MySpacerItem::MySpacerItem(int w, int h) : QSpacerItem(w, h, QSizePolicy::Fixed, QSizePolicy::Fixed) {
    
}

// MyLabel

MyLabel::MyLabel(const QString& text, QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont _font;
    _font.setBold(true);
    _font.setCapitalization(QFont::Capitalize);
    _font.setPointSize(12);
    setFont(_font);
    
    if (!text.isEmpty())
        setText(text);
    
    setFixedSize(120, 20);
}

// MyTitleLabel

MyTitleLabel::MyTitleLabel(const QString& text, QWidget* parent) : MyLabel(text, parent) {
    QFont _font = font();
    _font.setPointSize(18);
    setFont(_font);
    setFixedSize(160, 25);
    setAlignment(Qt::AlignCenter);
}

// MyLineEdit

MyLineEdit::MyLineEdit(const QString &contents, QWidget* parent) : QLineEdit(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QLineEdit {border: 1px solid LightSlateGray; border-radius: 10px; padding: 0 8px; background: GhostWhite; selection-background-color: LightGray;} QLineEdit::read-only {color: gray}");
    setText(contents);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

void MyLineEdit::setText(const QString &contents) {
    QLineEdit::setText(contents);
    setToolTip(contents);
    setCursorPosition(0);
}



MyReadOnlyLineEdit::MyReadOnlyLineEdit(const QString &contents, QWidget* parent) : MyLineEdit(contents, parent) {
    setReadOnly(true);
}

// MySpinBox

MySpinBox::MySpinBox(QWidget* parent) : QSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QSpinBox::up-button { background-color: transparent; }" "QSpinBox::down-button { background-color: transparent;}" "QSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyDoubleSpinBox

MyDoubleSpinBox::MyDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QDoubleSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QDoubleSpinBox::up-button { background-color: transparent; }" "QDoubleSpinBox::down-button { background-color: transparent;}" "QDoubleSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QDoubleSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyComboBox

MyComboBox::MyComboBox(QWidget* parent) : QComboBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QComboBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; color: black; padding: 1px 18px 1px 3px; }" "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left-width: 1px; border-left-color: LightSlateGray;border-left-style: solid; border-top-right-radius: 5px; border-bottom-right-radius: 5px; }" "QComboBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; } QComboBox::disabled { color: gray;} QComboBox QAbstractItemView { border: 1px solid LightSlateGray; background-color: white; min-width: 100px; }");
    
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyGroupBox

MyGroupBox::MyGroupBox(QWidget* parent) : QGroupBox(parent) {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setStyleSheet("QGroupBox { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

// MyMenuItemGroupBox

MyMenuItemGroupBox::MyMenuItemGroupBox(QWidget* parent) : MyGroupBox(parent) {
    setLayout(new QGridLayout());
}

const QSize MyMenuItemGroupBox::extents() const {
    qint32 totalWidth = 0;
    qint32 totalHeight = 0;
    qint32 rowWidth = 0;
    qint32 rowHeight = 0;
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QLayoutItem* item = NULL;
    for (qint32 row = 0; row < contentLayout->rowCount(); ++row) {
        rowWidth = 0;
        rowHeight = 0;
        for (qint32 column = 0; column < contentLayout->columnCount(); ++column) {
            item = contentLayout->itemAtPosition(row, column);
            if (item && item->widget()) {
                rowWidth += item->widget()->size().width();
                if (item->widget()->size().height() > rowHeight)
                    rowHeight = item->widget()->size().height();
            }
        }
        
        if (rowWidth > totalWidth)
            totalWidth = rowWidth;
        totalHeight+= rowHeight;
    }

    return QSize(totalWidth + 10 * contentLayout->columnCount(), totalHeight + 10 *  contentLayout->rowCount() );
}









