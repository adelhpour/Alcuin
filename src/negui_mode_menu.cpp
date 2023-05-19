#include "negui_mode_menu.h"
#include "negui_customized_mode_menu_widgets.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenu::setNormalModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateNormalButton(button), contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateNormalButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/mouse-pointer.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());

    return button;
}

void MyModeMenu::setSelectModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateSelectButton(button), contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateSelectButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/maximize.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());

    return button;
}

void MyModeMenu::setAddModeButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QToolButton* button = decorateAddButton(buttons);
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateAddButton(QList<QToolButton*> buttons) {
    QToolButton* button = new MyModeToolButton("Add");
    button->setMenu(createAddButtonMenu(buttons));

    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/plus-square.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());

    return button;
}

QMenu* MyModeMenu::createAddButtonMenu(QList<QToolButton*> buttons) {
    QMenu* addButtonMenu = new QMenu();
    for (QToolButton* button : qAsConst(buttons)) {
        addButtonMenu->addAction(createButtonWidgetAction(button));
        connect((MyToolButton*)button, &MyToolButton::menuItemIsChosen, addButtonMenu, [addButtonMenu] () { addButtonMenu->close(); });
    }

    return addButtonMenu;
}

QWidgetAction* MyModeMenu::createButtonWidgetAction(QToolButton* button) {
    QWidgetAction* widgetAction = new QWidgetAction(this);
    widgetAction->setDefaultWidget(button);

    return widgetAction;
}

void MyModeMenu::setRemoveModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateRemoveButton(button), contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateRemoveButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/minus-square.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}

void MyModeMenu::setZoomInButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateZoomInButton(button), contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateZoomInButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/zoom-in.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}

void MyModeMenu::setZoomOutButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateZoomOutButton(button), contentLayout->rowCount(), 0);
    _buttons.push_back(button);
}

QToolButton* MyModeMenu::decorateZoomOutButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/zoom-out.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
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