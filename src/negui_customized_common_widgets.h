#ifndef __NEGUI_CUSTOMIZED_COMMON_WIDGETS_H
#define __NEGUI_CUSTOMIZED_COMMON_WIDGETS_H

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
#include <QEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>

class MyFrame : public QFrame {
    Q_OBJECT

public:

    MyFrame(QWidget* parent = nullptr);
};

class MySpacerItem : public QSpacerItem {

public:

    MySpacerItem(int w, int h);
};

class MyLabel : public QLabel {

public:

    MyLabel(const QString& text = "", const QString& toolTip = "", QWidget* parent = nullptr);
};

class MyTitleLabel : public MyLabel {

public:

    MyTitleLabel(const QString& text = "", const QString& toolTip = "", QWidget* parent = nullptr);
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

class MyToolButton : public QToolButton {
    Q_OBJECT

public:

    MyToolButton(QWidget* parent = nullptr);

    void setStyleToSelectedMode();

    void setStyleToHoveredMode();

    void setStyleToUnselectedMode();
    
signals:

    void menuItemIsChosen();

protected:

    bool event(QEvent* event) override;
};

class MyMenu : public QMenu {
    Q_OBJECT

public:

    MyMenu(QWidget* parent = nullptr);
};

class MyDialog : public QDialog {
    Q_OBJECT

public:

    MyDialog(QWidget *parent = nullptr);

    void setButtons();
};

class MyAutoSaveMessageBox : public QMessageBox {
    Q_OBJECT

public:

    MyAutoSaveMessageBox(const QString& networkName, QWidget *parent = nullptr);
};

#endif
