#ifndef __NEGUI_MODE_MENU_H
#define __NEGUI_MODE_MENU_H

#include "negui_customized_common_widgets.h"

class MyModeMenuBase : public MyFrame {
    Q_OBJECT

public:

    MyModeMenuBase(QWidget *parent = nullptr);

    void setNormalModeButton(QAbstractButton* button, const QString& iconsDirectoryPath);

    virtual void setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath);

    virtual void setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath);

    void setZoomInButton(QAbstractButton* button, const QString& iconsDirectoryPath);

    void setZoomOutButton(QAbstractButton* button, const QString& iconsDirectoryPath);

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

class MyFrequentlyUsedButtonsModeMenu : public MyModeMenuBase {
    Q_OBJECT

public:

    MyFrequentlyUsedButtonsModeMenu(QWidget *parent = nullptr);

    void setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) override;
};

class MyAddModeButtonsModeMenu : public MyModeMenuBase {
    Q_OBJECT

public:

    MyAddModeButtonsModeMenu(QWidget *parent = nullptr);

    void setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) override;
};

const bool isModeToolButton(QAbstractButton* button);

#endif

