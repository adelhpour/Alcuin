#ifndef __NEGUI_ELEMENT_STYLE_BASE_H
#define __NEGUI_ELEMENT_STYLE_BASE_H

#include "negui_plugin_item_base.h"
#include "negui_shape_style_base.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QMenu>

class MyNetworkElementStyleBase : public QObject, public MyPluginItemBase {
    Q_OBJECT
    
public:
    
    MyNetworkElementStyleBase(const QString& name);
    
    QList<MyShapeStyleBase*>& shapeStyles();
    
    virtual void addDefaultShapeStyle() = 0;
    
    virtual MyShapeStyleBase* createShapeStyle(const QString& shape) = 0;
    
    virtual void clearShapeStyles();

    MyParameterBase* getParameter(MyShapeStyleBase::SHAPE_STYLE shape, const QString& parameterName);
    
    virtual const QRectF getShapesExtents(QRectF defaultExtents = QRectF(0.0, 0.0, 0.0, 0.0));
    
    const QIcon icon() override;

    const QString& nameTitle();

    const bool isNameEditable();
    
    virtual QObject* createIconBuilder() = 0;
    
    virtual const QString toolTipText() = 0;
    
    // read the element style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the element style info to the json object
    void write(QJsonObject &json) override;
    
public slots:
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
protected:
    QList<MyShapeStyleBase*> _shapeStyles;
    QString _nameTitle;
    bool _isNameEditable;
};

class MyChangeShapeStylesButtonsBase : public QDialogButtonBox {
    Q_OBJECT

public:

    MyChangeShapeStylesButtonsBase(QWidget* parent = nullptr);

    virtual void setMenu() = 0;

signals:

    void askForChangeShapeStyle(MyShapeStyleBase* shapeStyle);

protected:
    QPushButton* _changePushButton;
    QMenu* _menu;
};

class MyAddRemoveShapeStylesButtonsBase : public QDialogButtonBox {
    Q_OBJECT
    
public:
    
    MyAddRemoveShapeStylesButtonsBase(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() = 0;
    
signals:
    
    void askForAddShapeStyle(MyShapeStyleBase* shapeStyle);
    
    void askForRemoveShapeStyle(MyShapeStyleBase* shapeStyle);
    
public slots:
    
    void setRemovingMenu(QList<MyShapeStyleBase*> shapeStyles);
    
protected:
    QPushButton* _addPushButton;
    QPushButton* _removePushButton;
    QMenu* _addingMenu;
    QMenu* _removingMenu;
};

#endif
