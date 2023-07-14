#include "negui_mode_menu.h"
#include "negui_decorate_menu_buttons.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenu::setNormalModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    decorateNormalModeButton(button);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenu::setAddModeButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QToolButton* button = decorateAddModeButton(buttons);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenu::setZoomInButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    decorateZoomInButton(button);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

void MyModeMenu::setZoomOutButton(QToolButton* button) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateZoomOutButton(button);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);

}

void MyModeMenu::setMode(const QString& mode) {
    deactivateButtons();
    for (QToolButton* button : getButtons()) {
        if (isModeToolButton(button)) {
            if (isButtonOfTheMode((MyModeToolButton*)button, mode.split("_").at(0))) {
                ((MyModeToolButton*)button)->setActive(true);
                return;
            }
        }
    }
}

QList<QToolButton*> MyModeMenu::getButtons() {
    return _buttons;
}

void MyModeMenu::deactivateButtons() {
    for (QToolButton* button : getButtons()) {
        if (isModeToolButton(button))
            ((MyModeToolButton*)button)->setActive(false);
    }
}

const bool isModeToolButton(QToolButton* button) {
    if (dynamic_cast<MyModeToolButton*>(button))
        return true;

    return false;
}

const bool isButtonOfTheMode(MyModeToolButton* button, const QString& mode) {
    if (button->mode() == mode)
        return true;

    return false;
}