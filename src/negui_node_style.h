#ifndef __NEGUI_NODE_STYLE_H
#define __NEGUI_NODE_STYLE_H

#include "negui_element_style_base.h"

class MyNodeStyleBase : public MyElementStyleBase {
public:

    MyNodeStyleBase(const QString& name);
    
    const QString type() const override;

    QObject* createIconBuilder() override;

    const QString toolTipText() override;
    
    QList<QString> parentCategories();
    
    // read the element style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the element style info to the json object
    void write(QJsonObject &json) override;
    
protected:
    QList<QString> _parentCategories;
};

class MyClassicNodeStyle : public MyNodeStyleBase {
public:

    MyClassicNodeStyle(const QString& name);

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;

    const QString& convertibleParentCategory() const;

    bool isConvertibleToParentCategory(QList<QString> parentCategories);

    void convertToParentCategory();

    QWidget* getAddRemoveShapeStylesButtons();

    // read the element style info from the json object
    void read(const QJsonObject &json) override;

    // write the element style info to the json object
    void write(QJsonObject &json) override;

protected:
    QString _convertibleParentCategory;
    QWidget* _addRemoveShapeStylesButtons;
};

class MyCentroidNodeStyle : public MyNodeStyleBase {
public:

    MyCentroidNodeStyle(const QString& name);

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

class MyAddRemoveNodeShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveNodeShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
