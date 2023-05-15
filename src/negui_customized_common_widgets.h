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

#endif
