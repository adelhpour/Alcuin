#ifndef __NEGUI_FEATURE_MENU_H
#define __NEGUI_FEATURE_MENU_H

#include "negui_shape_style_base.h"
#include "negui_customized_feature_menu_widgets.h"

class MyFeatureMenu : public MyFrame {
    Q_OBJECT

public:
    
    MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent = nullptr);

    QList<MyShapeStyleBase*> shapeStyles();

    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);

    void addShapeStyle(MyShapeStyleBase* shapeStyle);

    void clearShapeStyles();

signals:
    
    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);

    void isUpdated(QList<MyShapeStyleBase*>);
    
private slots:

    void addNewShapeStyle(MyShapeStyleBase* shapeStyle);

    void removeShapeStyle(MyShapeStyleBase* shapeStyle);

    void setExpandableWidgetSize(const QSize& expandableWidgetSize);
    
protected:
    
    void updateExtents();
    
    QWidget* _elementFeatureMenu;
    QTreeView* _shapeStylesTreeView;
    QList<MyShapeStyleBase*> _shapeStyles;
    QSize _expandableWidgetSize;
};

#endif
