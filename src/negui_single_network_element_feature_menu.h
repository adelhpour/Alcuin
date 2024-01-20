#ifndef __NEGUI_SINGLE_NETWORK_ELEMENT_FEATURE_MENU_H
#define __NEGUI_SINGLE_NETWORK_ELEMENT_FEATURE_MENU_H

#include "negui_feature_menu_base.h"
#include "negui_shape_style_base.h"

class MySingleNetworkElementFeatureMenu : public MyFeatureMenuBase {
    Q_OBJECT

public:

    MySingleNetworkElementFeatureMenu(QWidget* elementFeatureMenu, const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    FEATURE_MENU_TYPE type() override;

    QList<MyShapeStyleBase*> shapeStyles();

    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);

    void addShapeStyle(MyShapeStyleBase* shapeStyle);

    void addSingleShapeStyle(MyShapeStyleBase* shapeStyle);

    MyShapeStyleBase* beingModifiedShapeStyle();

    void setBeingModifiedShapeStyle(MyShapeStyleBase* shapeStyle);

    void clearShapeStyles();

signals:

    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);

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
