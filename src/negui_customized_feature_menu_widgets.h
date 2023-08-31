#ifndef __NEGUI_CUSTOMIZED_FEATURE_MENU_WIDGETS_H
#define __NEGUI_CUSTOMIZED_FEATURE_MENU_WIDGETS_H

#include "negui_customized_common_widgets.h"
#include "negui_shape_style_base.h"

#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QMenu>
#include <QToolButton>
#include <QPushButton>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItem>

class MyFeatureMenuItemFrame : public MyFrame {
    Q_OBJECT

signals:
    void askForAddShapeStyle(MyShapeStyleBase*);

    void askForRemoveShapeStyle(MyShapeStyleBase*);

    void askForChangeShapeStyle(MyShapeStyleBase*);

    void askForSetRemovingMenu(QList<MyShapeStyleBase*>);

    void isUpdated();

public:

    MyFeatureMenuItemFrame(QWidget* parent = nullptr);

    const QSize extents() const;
};

class MyRestrictedToNameConventionsLineEdit : public MyLineEdit {
    Q_OBJECT

public:

    MyRestrictedToNameConventionsLineEdit(const QString &contents = "", QWidget* parent = nullptr);

    void setText(const QString& newText);
};

class MyColorPickerButton : public QToolButton {
    Q_OBJECT

public:
    MyColorPickerButton(QWidget *parent = nullptr);

    const QString& currentColor() const;

    void setBackgroundColor(const QString& color);

public slots:

            void setCurrentColor(const QString& color);

protected:
    QMenu* _colorPickerMenu;
    QString _currentColor;
};

class MyColorPickerMenu : public QMenu {
    Q_OBJECT

public:
    MyColorPickerMenu();

    signals:
            void colorChosen(const QString&);

private slots:
            void colorTileButtonPicked(QPushButton* colorTileButton);
};

class MyColorTileButton : public QPushButton {

public:
    MyColorTileButton(const QString& color, const QString& value, QWidget* parent = 0);

    const QString& color() const;

    const QString& value() const;

protected:
    QString _color;
    QString _value;
};

class MyFontPickerButton : public QPushButton {
    Q_OBJECT

public:

    MyFontPickerButton(QWidget *parent = nullptr);

    void setCurrentFont(const QFont& font);

    void setCurrentFont();

    const QFont& currentFont() const;

signals:

    void fontIsChanged();

protected:
    QFont _currentFont;
};

class MyStandardItem : public QStandardItem {

public:

    MyStandardItem(const QString& text = "", const qreal& fontsize = qreal(12.0), const bool& isBold = false, const QColor& color = QColor(Qt::black));
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

    void expandBranch(MyShapeStyleBase* shapeStyle);

    void expandBranch(const qint32& branchIndex);

    void expandFirstBranch();

    void expandLastBranch();

    const QSize collapsedSize() const;

    const QString getExpandedBranchTitle();

signals:

    void extentsAreUpdated(const QSize& expandableWidgetSize);

protected:

    branchVec _branches;
};

#endif
