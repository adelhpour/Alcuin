#ifndef __NEGUI_NODE_STYLE_H
#define __NEGUI_NODE_STYLE_H

#include "negui_element_style_base.h"

class MyNodeStyle : public MyElementStyleBase {
public:
    
    MyNodeStyle(const QString& name);
    
    const QString type() const override;
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    const QString& convertibleParentCategory() const;
    
    bool isConvertibleToParentCategory(QList<QString> parentCategories);
    
    void convertToParentCategory();
    
    QList<QString> parentCategories();
    
    QWidget* getAddRemoveShapeStylesButtons();
    
    // read the element style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the element style info to the json object
    void write(QJsonObject &json) override;
    
protected:
    QString _convertibleParentCategory;
    QList<QString> _parentCategories;
    QWidget* _addRemoveShapeStylesButtons;
};

class MyAddRemoveNodeShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveNodeShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
