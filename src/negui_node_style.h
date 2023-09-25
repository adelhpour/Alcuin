#ifndef __NEGUI_NODE_STYLE_H
#define __NEGUI_NODE_STYLE_H

#include "negui_network_element_style_base.h"

class MyNodeStyleBase : public MyNetworkElementStyleBase {
public:

    typedef enum {
        SIMPLE_CLASSIC_NODE_STYLE,
        COMPLEX_CLASSIC_NODE_STYLE,
        CENTROID_NODE_STYLE,
    } NODE_STYLE_TYPE;

    MyNodeStyleBase(const QString& name);
    
    const QString type() const override;

    virtual NODE_STYLE_TYPE nodeStyleType() = 0;

    QObject* createIconBuilder() override;

    const QString toolTipText() override;

    const QString& parentTitle();
    
    QList<QString> parentCategories();
    
    // read the element style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the element style info to the json object
    void write(QJsonObject &json) override;
    
protected:
    QString _parentTitle;
    QList<QString> _parentCategories;
};

class MyClassicNodeStyleBase : public MyNodeStyleBase {
public:

    MyClassicNodeStyleBase(const QString& name);

    void addDefaultShapeStyle() override;

    const QString& convertibleParentCategory() const;

    bool isConvertibleToParentCategory(QList<QString> parentCategories);

    void convertToParentCategory();

    // read the element style info from the json object
    void read(const QJsonObject &json) override;

    // write the element style info to the json object
    void write(QJsonObject &json) override;

protected:
    QString _convertibleParentCategory;
};

class MySimpleClassicNodeStyle : public MyClassicNodeStyleBase {
    Q_OBJECT

public:

    MySimpleClassicNodeStyle(const QString& name);

    NODE_STYLE_TYPE nodeStyleType() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;

    void updateSimpleTextStyleExtentsWithOtherShapeStyleExtents();

    QWidget* shapeStylesButtons() override;

    void replaceShapeStyle(MyShapeStyleBase* shapeStyle) override;

    const bool whetherAnotherGeometricShapeAlreadyExists();

    const bool whetherAnotherTextShapeAlreadyExists();

public slots:

    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) override;
};

class MyComplexClassicNodeStyle : public MyClassicNodeStyleBase {
public:

    MyComplexClassicNodeStyle(const QString& name);

    NODE_STYLE_TYPE nodeStyleType() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;

    QWidget* shapeStylesButtons() override;
};

class MyCentroidNodeStyle : public MyNodeStyleBase {
public:

    MyCentroidNodeStyle(const QString& name);

    NODE_STYLE_TYPE nodeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

class MyChangeNodeShapeStylesButton : public MyChangeShapeStylesButtonsBase {
    Q_OBJECT

public:

    MyChangeNodeShapeStylesButton(QWidget* parent = nullptr);

    virtual void setMenu() override;
};

class MyAddRemoveNodeShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveNodeShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
