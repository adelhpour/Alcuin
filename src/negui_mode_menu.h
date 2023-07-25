#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setNormalModeButton(QToolButton* button, const QString& iconsDirectoryPath);

    void setAddModeButtons(QList<QToolButton*> buttons, const QString& iconsDirectoryPath);

    void setZoomInButton(QToolButton* button, const QString& iconsDirectoryPath);

    void setZoomOutButton(QToolButton* button, const QString& iconsDirectoryPath);

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

