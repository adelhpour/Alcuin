#ifndef __NEGUI_SHAPE_STYLE_BASE_H
#define __NEGUI_SHAPE_STYLE_BASE_H

#include "negui_base.h"
#include "negui_parameters.h"

class MyShapeStyleBase : public QObject, public MyBase {
    Q_OBJECT

public:
    
    typedef enum {
        ELLIPSE_SHAPE_STYLE,
        RECTANGLE_SHAPE_STYLE,
        POLYGON_SHAPE_STYLE,
        LINE_SHAPE_STYLE,
        SIMPLE_TEXT_SHAPE_STYLE,
        WITH_PLAIN_TEXT_TEXT_SHAPE_STYLE,
        CENTROID_SHAPE_STYLE,
    } SHAPE_STYLE;
    
    MyShapeStyleBase(const QString& name);
    
    virtual ~MyShapeStyleBase();
    
    virtual SHAPE_STYLE type() = 0;
    
    // get parameters
    const QList<MyParameterBase*>& parameters() const;

    const QList<MyParameterBase*>& outsourcingParameters() const;

    const QList<MyParameterBase*>& hiddenParameters() const;

    void addParameter(MyParameterBase* parameter);
    
    void addParameterToTheBeginningOfTheList(MyParameterBase* parameter);

    void addOutsourcingParameter(MyParameterBase* parameter);

    void addHiddenParameter(MyParameterBase* parameter);
    
    // find a parameter among the list of parameters using its name
    MyParameterBase* findParameter(const QString& name) const;

    virtual const QRectF getShapeExtents() = 0;
    
    // populate the menu layout with the style features
    virtual void populateFeaturesMenu(QLayout* featureMenuLayout);
    
    // set the default values of each parameter
    virtual void updateFeatures();

    virtual void update(MyShapeStyleBase* shapeStyle) = 0;
    
    // reset all the values
    virtual void reset();
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;

signals:

    void isUpdated();

protected:
    QList<MyParameterBase*> _parameters;
    QList<MyParameterBase*> _outsourcingParameters;
    QList<MyParameterBase*> _hiddenParameters;
};

#endif
