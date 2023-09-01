#ifndef __NEGUI_PARAMETERS_H
#define __NEGUI_PARAMETERS_H

#include "negui_base.h"
#include <QObject>
#include <QWidget>

class MyParameterBase : public QObject, public MyBase {
    Q_OBJECT

public:

    MyParameterBase(const QString& name, const QString& hint);

    typedef enum {
        DOUBLE_PARAMETER_TYPE,
        INTEGER_PARAMETER_TYPE,
        BOOLEAN_PARAMETER_TYPE,
        STRING_PARAMETER_TYPE,
        NOMINAL_PARAMETER_TYPE,
        POINT_PARAMETER_TYPE,
        COLOR_PARAMETER_TYPE,
        FONT_PARAMETER_TYPE,
    } PARAMETER_TYPE;
    
    virtual ~MyParameterBase() {
        delete _inputWidget;
    };
    
    // get type of parameter
    virtual PARAMETER_TYPE type() = 0;

    virtual const QString hint();

    // set the default value using the input widget value
    virtual void setDefaultValue() = 0;

    // get the user input widget
    QWidget* inputWidget();

    virtual QWidget* createInputWidget() = 0;

    virtual void updateInputWidget() = 0;

    virtual void connectInputWidget() = 0;

    // reset the values of the parameter
    virtual void reset() = 0;

signals:

    void isUpdated();

protected:
    QWidget* _inputWidget;
    QString _hint;
};

class MyDoubleParameter : public MyParameterBase {
public:

    MyDoubleParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const qreal& value);

    // get default value
    const qreal& defaultValue() const;

    // set the default value using the input widget value
    void setDefaultValue() override;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    // set min
    void setMin(const qreal& min);

    // get min
    const qreal& min() const;

    // show whether the min is set
    const bool isSetMin() const { return _isSetMin; }

    // set max
    void setMax(const qreal& max);

    // get max
    const qreal& max() const;

    // show whether the max is set
    const bool isSetMax() const { return _isSetMax; }

    // set step
    void setStep(const qreal& step);

    // get step
    const qreal& step() const;

    // show whether the step is set
    const bool isSetStep() const { return _isSetStep; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    qreal _defaultValue;
    qreal _min;
    qreal _max;
    qreal _step;
    bool _isSetDefaultValue;
    bool _isSetMin;
    bool _isSetMax;
    bool _isSetStep;
};

class MyIntegerParameter : public MyParameterBase {
public:

    MyIntegerParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const qint32& value);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const qint32& defaultValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    // set min
    void setMin(const qint32& min);

    // get min
    const qint32& min() const;

    // show whether the min is set
    const bool isSetMin() const { return _isSetMin; }

    // set max
    void setMax(const qint32& max);

    // get max
    const qint32& max() const;

    // show whether the max is set
    const bool isSetMax() const { return _isSetMax; }

    // set step
    void setStep(const qint32& step);

    // get step
    const qint32& step() const;

    // show whether the step is set
    const bool isSetStep() const { return _isSetStep; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    qint32 _defaultValue;
    qint32 _min;
    qint32 _max;
    qint32 _step;
    bool _isSetDefaultValue;
    bool _isSetMin;
    bool _isSetMax;
    bool _isSetStep;
};

class MyBooleanParameter : public MyParameterBase {
public:

    MyBooleanParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const bool& value);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const bool& defaultValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    bool _defaultValue;
    bool _isSetDefaultValue;
};

class MyStringParameter : public MyParameterBase {
public:

    MyStringParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const QString& value);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const QString& defaultValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    QString _defaultValue;
    bool _isSetDefaultValue;
};

class MyNominalParameter : public MyParameterBase {
public:

    MyNominalParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const QString& value);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const QString& defaultValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // get the items of the nominal parameter
    const QStringList& items() const;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    QString _defaultValue;
    QStringList _items;
    bool _isSetDefaultValue;
};

class MyPointParameterBase : public MyParameterBase {
public:

    MyPointParameterBase(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value of x
    void setDefaultValueX(const qreal& x);

    // get the default value of x
    const qreal& defaultValueX() const;

    // set default value of y
    void setDefaultValueY(const qreal& y);

    // get the default value of y
    const qreal& defaultValueY() const;

    // set the default value using the input widget value
    void setDefaultValue() override;

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    MyParameterBase* _x;
    MyParameterBase* _y;
};

class MyAbsolutePointParameter : public MyPointParameterBase {
public:

    MyAbsolutePointParameter(const QString& name, const QString& hint = "");
};

class MyRelativePointParameter : public MyPointParameterBase {
public:

    MyRelativePointParameter(const QString& name, const QString& hint = "");
};

class MyControlPointParameter : public MyPointParameterBase {
public:

    MyControlPointParameter(const QString& name, const QString& hint = "");
};

class MyColorParameter : public MyParameterBase {
public:

    MyColorParameter(const QString& name, const QString& hint = "");

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const QString& value);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const QString& defaultValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    QString _defaultValue;
    bool _isSetDefaultValue;
};

class MyBorderWidthParameter : public MyIntegerParameter {
public:

    MyBorderWidthParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyCentroidBorderWidthParameter : public MyBorderWidthParameter {
public:

    MyCentroidBorderWidthParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyPositionalParameter : public MyDoubleParameter {
public:

    MyPositionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeRectanglePositionalParameter : public MyPositionalParameter {
public:

    MyNodeRectanglePositionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeEllipsePositionalParameter : public MyPositionalParameter {
public:

    MyNodeEllipsePositionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeTextPositionalParameter : public MyPositionalParameter {
public:

    MyNodeTextPositionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadRectangleXParameter : public MyPositionalParameter {
public:

    MyArrowHeadRectangleXParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadRectangleYParameter : public MyPositionalParameter {
public:

    MyArrowHeadRectangleYParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadEllipseCxParameter : public MyPositionalParameter {
public:

    MyArrowHeadEllipseCxParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadEllipseCyParameter : public MyPositionalParameter {
public:

    MyArrowHeadEllipseCyParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyRelativePositionalParameter : public MyPositionalParameter {
public:

    MyRelativePositionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyDimensionalParameter : public MyDoubleParameter {
public:

    MyDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeRectangleDimensionalParameter : public MyDimensionalParameter {
public:

    MyNodeRectangleDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeEllipseDimensionalParameter : public MyDimensionalParameter {
public:

    MyNodeEllipseDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeTextDimensionalParameter : public MyDimensionalParameter {
public:

    MyNodeTextDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadRectangleDimensionalParameter : public MyDimensionalParameter {
public:

    MyArrowHeadRectangleDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyArrowHeadEllipseDimensionalParameter : public MyDimensionalParameter {
public:

    MyArrowHeadEllipseDimensionalParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyCornerCurvatureParameter : public MyDoubleParameter {
public:

    MyCornerCurvatureParameter(const QString& name, const QString& hint = "");

    // reset the values of the parameter
    void reset() override;
};

class MyNodeCentroidRadiusParameter : public  MyDimensionalParameter {
public:

    MyNodeCentroidRadiusParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyBorderColorParameter : public MyColorParameter {
public:

    MyBorderColorParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyCentroidBorderColorParameter : public MyBorderColorParameter {
public:

    MyCentroidBorderColorParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyFillColorParameter : public MyColorParameter {
public:

    MyFillColorParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyCentroidFillColorParameter : public MyFillColorParameter {
public:

    MyCentroidFillColorParameter();

    // reset the values of the parameter
    void reset() override;
};

class MyTextPlainTextParameter : public MyStringParameter {

public:

    MyTextPlainTextParameter();

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;
};

class MyFontParameter : public MyParameterBase {
public:

    MyFontParameter();

    // get type of parameter
    PARAMETER_TYPE type() override;

    // set default value
    void setDefaultValue(const QFont& value);

    // set default value
    void setDefaultFontFamilyValue(const QString& fontFamily);

    // set default value
    void setDefaultFontSizeValue(const qreal& fontSize);

    // set default value
    void setDefaultFontWeightValue(const QString& fontWeight);

    // set default value
    void setDefaultFontStyleValue(const QString& fontStyle);

    // set the default value using the input widget value
    void setDefaultValue() override;

    // get default value
    const QFont& defaultValue() const;

    // get default value
    const QString defaultFontFamilyValue() const;

    // get default value
    const qreal defaultFontSizeValue() const;

    // get default value
    const QString defaultFontWeightValue() const;

    // get default value
    const QString defaultFontStyleValue() const;

    // show whether the default value is set
    const bool isSetDefaultValue() const { return _isSetDefaultValue; }

    QWidget* createInputWidget() override;

    void updateInputWidget() override;

    void connectInputWidget() override;

    // reset the values of the parameter
    void reset() override;

    // read the parameter info from the json object
    void read(const QJsonObject &json) override;

    // write the parameter info to the json object
    void write(QJsonObject &json) override;

protected:
    QFont _defaultValue;
    bool _isSetDefaultValue;
};

class MyHorizontalAlignmentParameter : public MyNominalParameter {
public:

    MyHorizontalAlignmentParameter();

    // get the value of default alignment
    const Qt::Alignment defaultAlignment() const;

    // reset the values of the parameter
    void reset() override;
};

class MyVerticalAlignmentParameter : public MyNominalParameter {
public:

    MyVerticalAlignmentParameter();

    // get the value of default alignment
    const Qt::Alignment defaultAlignment() const;

    // reset the values of the parameter
    void reset() override;
};

class MyNodeDistributionNumberOfGridColumnsParameter : public MyIntegerParameter {
public:

    MyNodeDistributionNumberOfGridColumnsParameter(const qint32 numberOfSelectedNodes);

    // reset the values of the parameter
    void reset() override;

protected:
    qint32 _numberOfSelectedNodes;
};

#endif
