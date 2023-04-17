#ifndef __NEGUI_FEATURE_MENU_H
#define __NEGUI_FEATURE_MENU_H

#include "negui_shape_style_base.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QStandardItem>
#include <QTreeView>
#include <QGridLayout>

class MyFeatureMenu : public MyGroupBox {
    Q_OBJECT

public:
    
    MyFeatureMenu(QWidget* elementFeatureMenu, QWidget *parent = nullptr);
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
    void setShapeStyles();
    
    QList<MyShapeStyleBase*> getShapeStyles();
    
    QList<MyShapeStyleBase*> getTemporaryShapeStyles();
    
signals:
    
    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);

    void isUpdated(QList<MyShapeStyleBase*>);
    
private slots:
    
    void setExpandableWidgetSize(const QSize& expandableWidgetSize);
    
    void addShapeStyle(MyShapeStyleBase* shapeStyle);
    
    void removeShapeStyle(MyShapeStyleBase* shapeStyle);
    
protected:
    
    void updateDialogBoxExtents();
    
    QWidget* _elementFeatureMenu;
    QTreeView* _shapeStylesTreeView;
    QList<MyShapeStyleBase*> _shapeStyles;
    QList<MyShapeStyleBase*> _temporaryAddedShapeStyles;
    QList<MyShapeStyleBase*> _temporaryRemovedShapeStyles;
    QSize _expandableWidgetSize;
};

class MyMenuItemGroupBox : public MyGroupBox {
    Q_OBJECT
    
signals:
    void askForAddShapeStyle(MyShapeStyleBase*);
    
    void askForRemoveShapeStyle(MyShapeStyleBase*);
    
    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);
    
public:
    
    MyMenuItemGroupBox(QWidget* parent = nullptr);
    
    const QSize extents() const;
};

class MyStandardItem : public QStandardItem {
    
public:
    
    MyStandardItem(const QString& text = "", const qreal& fontsize = qreal(12.0), const bool& isbold = false, const QColor& color = QColor(Qt::black));
};

class MyShapeStyleTreeView : public QTreeView {
    Q_OBJECT
    
public:
    
    MyShapeStyleTreeView(QWidget* parent = nullptr);
    
    void clearModel();
    
    /// Containers
    // branches
    typedef std::vector<std::pair<MyStandardItem*, MyStandardItem*>> branchVec;
    
    /// Iterators
    // branches
    typedef branchVec::iterator branchIt;
    typedef branchVec::const_iterator constBranchIt;
    
    /// Functions related to beginning and end of containers
    // branches
    const constBranchIt BranchesBegin() const { return _branches.begin(); }
    const constBranchIt BranchesEnd() const { return _branches.end(); }
    
    void setBranches(QList<MyShapeStyleBase*> shapeStyles);
    
    void addBranchWidget(QWidget* branchWidget = NULL, const QString& branchTitle = "", const QString& rootTitle= "");
    
    void removeBranches(const QString& rootTitle= "", const unsigned int& staticbranches = 0);
    
    void exapndLastBranch();
    
    const QSize collapsedSize() const;
    
signals:
    
    void extentsAreUpdated(const QSize& expandableWidgetSize);
    
protected:
    
    QStandardItemModel* treeModel;
    branchVec _branches;
};

#endif
