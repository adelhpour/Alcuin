#include "negui_mode_menu.h"
#include "negui_decorate_menu_buttons.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenu::setNormalModeButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    decorateNormalModeButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenu::setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    for (QAbstractButton* button : buttons) {
        contentLayout->addWidget(button, contentLayout->rowCount(), 0);
        _buttons.push_back(button);
    }
}

void MyModeMenu::setAddModeButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {
    //QGridLayout* contentLayout = (QGridLayout*)layout();
    //QAbstractButton* button = decorateAddModeButton(buttons, iconsDirectoryPath);
    //contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    //_buttons.push_back(button);
    //TODO for now, non-frequently-used elments styles are not shown
}

void MyModeMenu::setZoomInButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    decorateZoomInButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenu::setZoomOutButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateZoomOutButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);

}

void MyModeMenu::setMode(const QString& mode) {
    deactivateButtons();
    for (QAbstractButton* button : getButtons()) {
        if (isModeToolButton(button)) {
            if (isButtonOfTheMode((MyModeToolButton*)button, mode.split("_").at(0))) {
                ((MyModeToolButton*)button)->setActive(true);
                return;
            }
        }
    }
}

QList<QAbstractButton*> MyModeMenu::getButtons() {
    return _buttons;
}

void MyModeMenu::deactivateButtons() {
    for (QAbstractButton* button : getButtons()) {
        if (isModeToolButton(button))
            ((MyModeToolButton*)button)->setActive(false);
    }
}

const bool isModeToolButton(QAbstractButton* button) {
    if (dynamic_cast<MyModeToolButton*>(button))
        return true;

    return false;
}

const bool isButtonOfTheMode(MyModeToolButton* button, const QString& mode) {
    if (button->mode() == mode)
        return true;

    return false;
}