#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

class MyModeMenu : public MyFrame {
    Q_OBJECT

public:

    MyModeMenu(QWidget *parent = nullptr);

    void setNormalModeButton(QAbstractButton* button, const QString& iconsDirectoryPath);

    void setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath);

    void setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath);

    void setZoomInButton(QAbstractButton* button, const QString& iconsDirectoryPath);

    void setZoomOutButton(QAbstractButton* button, const QString& iconsDirectoryPath);

    QList<QAbstractButton*> getButtons();

public slots:

    void setMode(const QString& mode);

    void deactivateButtons();

protected:
    QList<QAbstractButton*> _buttons;
};

const bool isModeToolButton(QAbstractButton* button);

const bool isButtonOfTheMode(MyModeToolButton* button, const QString& mode);

#endif

