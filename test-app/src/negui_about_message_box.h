#ifndef __NEGUI_ABOUT_MESSAGE_BOX_H
#define __NEGUI_ABOUT_MESSAGE_BOX_H

#include <QMessageBox>

class MyAboutMessageBox : public QMessageBox {
    Q_OBJECT

public:

    MyAboutMessageBox(const QString& name, const QString& version, QWidget *parent = nullptr);
};

#endif
