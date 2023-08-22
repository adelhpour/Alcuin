#include "negui_mode_menu.h"
#include "negui_decorate_menu_buttons.h"

#include <QGridLayout>

// MyModeMenuBase

MyModeMenuBase::MyModeMenuBase(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenuBase::setNormalModeButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateNormalModeButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenuBase::setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {

}

void MyModeMenuBase::setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {

}

void MyModeMenuBase::setZoomInButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    decorateZoomInButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenuBase::setZoomOutButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateZoomOutButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);

}

void MyModeMenuBase::setMode(const QString& mode) {
    deactivateButtons();
    for (QAbstractButton* button : getButtons()) {
        if (isModeToolButton(button)) {
            if (isButtonOfTheMode((MyModeMenuModeButton*)button, mode.split("_").at(0))) {
                ((MyModeMenuModeButton*)button)->setActive(true);
                return;
            }
        }
    }
}

QList<QAbstractButton*> MyModeMenuBase::getButtons() {
    return _buttons;
}

void MyModeMenuBase::activateAddElementButton(const QString& elementStyleName) {
    deactivateButtons();
    for (QAbstractButton* button : getButtons()) {
        if (button->toolTip() == elementStyleName)
            ((MyModeMenuModeButton*)button)->setActive(true);
    }
}

void MyModeMenuBase::deactivateButtons() {
    for (QAbstractButton* button : getButtons()) {
        if (dynamic_cast<MyModeMenuToolButton*>(button))
            ((MyModeMenuToolButton*)button)->setActive(false);
    }
}

// MyFrequentlyUsedButtonsModeMenu

MyFrequentlyUsedButtonsModeMenu::MyFrequentlyUsedButtonsModeMenu(QWidget *parent) : MyModeMenuBase(parent) {

}

void MyFrequentlyUsedButtonsModeMenu::setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    for (QAbstractButton* button : buttons) {
        contentLayout->addWidget(button, contentLayout->rowCount(), 0);
        _buttons.push_back(button);
    }
}

// MyAddModeButtonsModeMenu

MyAddModeButtonsModeMenu::MyAddModeButtonsModeMenu(QWidget *parent) : MyModeMenuBase(parent) {

}

void MyAddModeButtonsModeMenu::setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QAbstractButton* button = decorateAddModeButton(buttons, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

const bool isModeToolButton(QAbstractButton* button) {
    if (dynamic_cast<MyModeMenuModeButton*>(button))
        return true;

    return false;
}

const bool isButtonOfTheMode(MyModeMenuModeButton* button, const QString& mode) {
    if (button->mode() == mode)
        return true;

    return false;
}