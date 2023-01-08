#ifndef __NEGUI_ARROW_HEAD_STYLE_H
#define __NEGUI_ARROW_HEAD_STYLE_H

#include "negui_element_style_base.h"

class MyArrowHeadStyle : public MyElementStyleBase {
public:
    
    MyArrowHeadStyle(const QString& name);
    
    const QString type() const override;
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    QWidget* getAddRemoveShapeStylesButtons();
    
    void write(QJsonObject &json) override;
    
protected:
    QWidget* _addRemoveShapeStylesButtons;
};

class MyAddRemoveArrowHeadShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
