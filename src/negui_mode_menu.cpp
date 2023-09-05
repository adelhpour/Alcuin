#include "negui_mode_menu.h"
#include "negui_decorate_menu_buttons.h"
#include "negui_customized_mode_menu_widgets.h"

#include <QGridLayout>

// MyModeMenuBase

MyModeMenuBase::MyModeMenuBase(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFixedSize(QSize(0, 0));
}

void MyModeMenuBase::setNormalModeButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateNormalModeButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
    button->setFixedSize(QSize(buttonSize(), buttonSize()));
    setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
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
    button->setFixedSize(QSize(buttonSize(), buttonSize()));
    setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
}

void MyModeMenuBase::setZoomOutButton(QAbstractButton* button, const QString& iconsDirectoryPath) {
    QGridLayout *contentLayout = (QGridLayout *) layout();
    decorateZoomOutButton(button, iconsDirectoryPath);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
    button->setFixedSize(QSize(buttonSize(), buttonSize()));
    setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
}

void MyModeMenuBase::setMode(const QString& mode) {
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

qint32 MyModeMenuBase::buttonSize() {
    return 45.0;
}

qint32 MyModeMenuBase::buttonPadding() {
    return buttonSize() / 3;
}

// MyFrequentlyUsedButtonsModeMenu

MyFrequentlyUsedButtonsModeMenu::MyFrequentlyUsedButtonsModeMenu(QWidget *parent) : MyModeMenuBase(parent) {

}

void MyFrequentlyUsedButtonsModeMenu::setFrequentlyUsedButtons(QList<QAbstractButton*> buttons, const QString& iconsDirectoryPath) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    for (QAbstractButton* button : buttons) {
        contentLayout->addWidget(button, contentLayout->rowCount(), 0);
        _buttons.push_back(button);
        button->setFixedSize(QSize(buttonSize(), buttonSize()));
        setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
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
    setFixedSize(QSize(qMax(width(), button->width() + buttonPadding()), height() + button->height() + buttonPadding()));
}

const bool isModeToolButton(QAbstractButton* button) {
    if (dynamic_cast<MyModeMenuModeButton*>(button))
        return true;

    return false;
}