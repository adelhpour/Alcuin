#ifndef __NEGUI_FEATURE_MENU_H
#define __NEGUI_FEATURE_MENU_H

#include "negui_base.h"
#include "negui_shape_style_base.h"
#include "negui_ellipse_style.h"
#include "negui_rect_style.h"
#include "negui_polygon_style.h"
#include "negui_line_style.h"
#include "negui_text_style.h"

class MyAddRemoveShapeStylesButtons;
class MyShapeStyleTreeView;

class MyFeatureMenu : public MyDialog {
    Q_OBJECT

public:
    
    MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent = nullptr);
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
    void setShapeStyles();
    
    QList<MyShapeStyleBase*> getShapeStyles();
    
    QList<MyShapeStyleBase*> getTemporaryShapeStyles();
    
private slots:
    
    void setExpandableWidgetSize(const QSize& expandableWidgetSize);
    
    void addShapeStyle(MyShapeStyleBase* shapeStyle);
    
    void removeShapeStyle(MyShapeStyleBase* shapeStyle);
    
protected:
    
    void updateExtents();
    
    QWidget* _elementFeatureMenu;
    MyAddRemoveShapeStylesButtons* _addRemoveShapeStyleButtons;
    MyShapeStyleTreeView* _shapeStylesTreeView;
    QList<MyShapeStyleBase*> _shapeStyles;
    QList<MyShapeStyleBase*> _temporaryAddedShapeStyles;
    QList<MyShapeStyleBase*> _temporaryRemovedShapeStyles;
    QSize _expandableWidgetSize;
};

class MyAddRemoveShapeStylesButtons : public QDialogButtonBox {
    Q_OBJECT
    
public:
    
    MyAddRemoveShapeStylesButtons(QWidget* parent = nullptr);
    
    void setRemovingMenu(QList<MyShapeStyleBase*> shapeStyles);
    
signals:
    void askForAddShapeStyle(MyShapeStyleBase* shapeStyle);
    void askForRemoveShapeStyle(MyShapeStyleBase* shapeStyle);
    
protected:
    QPushButton* _addPushButton;
    QPushButton* _removePushButton;
    QMenu* _addingMenu;
    QMenu* _removingMenu;
};

class MyShapeStyleTreeView : public MyTreeView {
    Q_OBJECT
    
public:
    
    MyShapeStyleTreeView(QWidget* parent = nullptr);
    
    void setBranches(QList<MyShapeStyleBase*> shapeStyles);
    
    void exapndLastBranch();
};

#endif
