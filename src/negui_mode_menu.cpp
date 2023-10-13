#include "negui_mode_menu.h"
#include "negui_customized_menu_button_widgets.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFixedSize(QSize(0, 0));
}

void MyModeMenu::addButtons(QList<QAbstractButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    for (QAbstractButton* button : buttons) {
        contentLayout->addWidget(button, contentLayout->rowCount(), 0);
        _buttons.push_back(button);
        button->setFixedSize(QSize(buttonSize(), buttonSize()));
        setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
    }
}

void MyModeMenu::setMode(const QString& mode) {
    deactivateButtons();
    for (QAbstractButton* button : getButtons()) {
        if (isModeToolButton(button)) {
            if (((MyModeMenuModeButton*)button)->getSceneModeAsString() == mode || ((MyModeMenuModeButton*)button)->getAlternativeSimilarSceneModeAsString() == mode) {
                ((MyModeMenuModeButton*)button)->setActive(true);
                return;
            }
        }
    }
}

QList<QAbstractButton*> MyModeMenu::getButtons() {
    return _buttons;
}

void MyModeMenu::activateAddElementButton(const QString& elementStyleName) {
    for (QAbstractButton* button : getButtons()) {
        if (button->toolTip() == elementStyleName) {
            deactivateButtons();
            ((MyModeMenuModeButton*)button)->setActive(true);
        }
    }
}

void MyModeMenu::deactivateButtons() {
    for (QAbstractButton* button : getButtons()) {
        if (dynamic_cast<MyModeMenuButton*>(button))
            ((MyModeMenuButton*)button)->setActive(false);
    }
}

qint32 MyModeMenu::buttonSize() {
    return 45.0;
}

qint32 MyModeMenu::buttonPadding() {
    return buttonSize() / 3;
}

const bool isModeToolButton(QAbstractButton* button) {
    if (dynamic_cast<MyModeMenuModeButton*>(button))
        return true;

    return false;
}