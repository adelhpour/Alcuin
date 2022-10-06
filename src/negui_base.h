#ifndef __NEGUI_BASE_H
#define __NEGUI_BASE_H

#if defined MAKE_NEGUI_LIB
    #define NEGUI_LIB_EXPORT Q_DECL_EXPORT
#else
    #define NEGUI_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QApplication>
#include <QWidget>
#include <QWidgetAction>
#include <QGridLayout>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QStandardItem>
#include <QTreeView>
#include <QProxyStyle>
#include <QPainter>
#include <QUndoStack>
#include <QtMath>
#include <QPrinter>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class MyBase {
    
public:
    
    MyBase(const QString& name);
    
    const QString& name() const;
    
    // import/export info
    virtual void read(const QJsonObject &json) = 0;
    virtual void write(QJsonObject &json) = 0;

protected:
    
    QString _name;
};

class MyPluginItemBase : public MyBase {
    
public:
    
    MyPluginItemBase(const QString& name);
    
    // icon
    virtual const QIcon icon() = 0;
    const QSize& iconSize() const;
    
protected:
    
    QSize _iconSize;
};

class MySpacerItem : public QSpacerItem {
    
public:
    
    MySpacerItem(int w, int h);
};

class MyLabel : public QLabel {
    
public:
    
    MyLabel(const QString& text = "", QWidget* parent = nullptr);
};

class MyTitleLabel : public MyLabel {
    
public:
    
    MyTitleLabel(const QString& text = "", QWidget* parent = nullptr);
};

class MyLineEdit : public QLineEdit {
    
public:
    
    MyLineEdit(const QString &contents = "", QWidget* parent = nullptr);
    
    void setText(const QString &contents);
};

class MyReadOnlyLineEdit : public MyLineEdit {
    
public:
    
    MyReadOnlyLineEdit(const QString &contents = "", QWidget* parent = nullptr);
};

class MySpinBox : public QSpinBox {
    
public:
    
    MySpinBox(QWidget* parent = nullptr);
};

class MyDoubleSpinBox : public QDoubleSpinBox {
    
public:
    
    MyDoubleSpinBox(QWidget* parent = nullptr);
};

class MyComboBox : public QComboBox {
    
public:
    
    MyComboBox(QWidget* parent = nullptr);
};

class MyGroupBox : public QGroupBox {
    Q_OBJECT
    
public:
    
    MyGroupBox(QWidget* parent = nullptr);
};

class MyMenuItemGroupBox : public MyGroupBox {
    Q_OBJECT
    
public:
    
    MyMenuItemGroupBox(QWidget* parent = nullptr);
    
    const QSize extents() const;
};

class MyDialog : public QDialog {
    Q_OBJECT

public:
    
    MyDialog(QWidget *parent = nullptr);
};

class MyStandardItem : public QStandardItem {
    
public:
    
    MyStandardItem(const QString& text = "", const qreal& fontsize = qreal(12.0), const bool& isbold = false, const QColor& color = QColor(Qt::black));
};

class MyTreeView : public QTreeView {
    Q_OBJECT
    
public:
    
    MyTreeView(QWidget* parent = nullptr);
    
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
    
    void addBranchWidget(QWidget* branchWidget = NULL, const QString& branchTitle = "", const QString& rootTitle= "");
    
    void removeBranches(const QString& rootTitle= "", const unsigned int& staticbranches = 0);
    
    const QSize collapsedSize() const;
    
signals:
    
    void extentsAreUpdated(const QSize& expandableWidgetSize);
    
protected:
    
    QStandardItemModel* treeModel;
    branchVec _branches;
};

class MyProxyStyle : public QProxyStyle {
    
public:
    
    MyProxyStyle(QStyle *style = nullptr);
    
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
};

class MyToolButton : public QToolButton {
    
public:
    
    MyToolButton(QWidget* parent = nullptr);
};

class MyToolButtonMenu : public QMenu {
    
public:
    
    MyToolButtonMenu(QWidget* parent = nullptr);
};

class MyWidgetAction : public QWidgetAction {
    Q_OBJECT
    
public:
    
    MyWidgetAction(QObject* parent = nullptr);
    
    void setItems(QList<MyPluginItemBase*> items);
    
signals:
    void itemIsChosen(MyPluginItemBase*);
    
protected:
    QWidget* createItemPreviewWidget(QList<MyPluginItemBase*> items);
};

class MyItemPreviewButton : public QPushButton {
    
public:
    
    MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent = nullptr);
};

class MyUndoStack : public QUndoStack {
    
public:
    
    MyUndoStack(QObject *parent = nullptr);
    
    void addCommand(QUndoCommand* command);
    
    void clear();
};

#endif
