#ifndef __NEGUI_FEATURE_MENU_H
#define __NEGUI_FEATURE_MENU_H

#include "negui_shape_style_base.h"
#include "negui_customized_feature_menu_widgets.h"

class MyFeatureMenuBase : public MyFrame {
    Q_OBJECT

public:

    MyFeatureMenuBase(QWidget* elementFeatureMenu, const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    QList<MyShapeStyleBase*> shapeStyles();

    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);

    void addShapeStyle(MyShapeStyleBase* shapeStyle);

    void addSingleShapeStyle(MyShapeStyleBase* shapeStyle);

    MyShapeStyleBase* beingModifiedShapeStyle();

    void setBeingModifiedShapeStyle(MyShapeStyleBase* shapeStyle);

    void clearShapeStyles();

signals:
    
    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);

    void askForRemoveFeatureMenu();

    void isUpdated(QList<MyShapeStyleBase*>);
    
private slots:

    void addNewShapeStyle(MyShapeStyleBase* shapeStyle);

    void removeShapeStyle(MyShapeStyleBase* shapeStyle);

    void changeShapeStyle(MyShapeStyleBase* shapeStyle);

    void setExpandableWidgetSize(const QSize& expandableWidgetSize);
    
protected:
    
    void updateExtents();
    
    QWidget* _elementFeatureMenu;
    QTreeView* _shapeStylesTreeView;
    QList<MyShapeStyleBase*> _shapeStyles;
    QSize _expandableWidgetSize;
};

#endif
