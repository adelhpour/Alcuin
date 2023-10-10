#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void addButtons(QList<QAbstractButton*> buttons);

    QList<QAbstractButton*> getButtons();

    qint32 buttonSize();

    qint32 buttonPadding();

public slots:

            void setMode(const QString& mode);

    void deactivateButtons();

    void activateAddElementButton(const QString& elementStyleName);

protected:
    QList<QAbstractButton*> _buttons;
};

const bool isModeToolButton(QAbstractButton* button);

#endif