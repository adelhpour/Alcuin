#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setNormalModeButton(QToolButton* button);

    void setAddModeButtons(QList<QToolButton*> buttons);

    void setZoomInButton(QToolButton* button);

    void setZoomOutButton(QToolButton* button);

    QList<QToolButton*> getButtons();

public slots:

    void setMode(const QString& mode);

    void deactivateButtons();

protected:
    QList<QToolButton*> _buttons;
};

const bool isModeToolButton(QToolButton* button);

const bool isButtonOfTheMode(MyModeToolButton* button, const QString& mode);

#endif

