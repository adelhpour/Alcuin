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
}

QToolButton* MyModeMenu::decorateNormalButton(QToolButton* button) {
    return button;
}

void MyModeMenu::setSelectModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateSelectButton(button), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateSelectButton(QToolButton* button) {
    return button;
}

void MyModeMenu::setAddModeButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateAddButton(buttons), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateAddButton(QList<QToolButton*> buttons) {
    QToolButton* addButton = new MyToolButton();
    addButton->setMenu(createAddButtonMenu(buttons));

    return addButton;
}

QMenu* MyModeMenu::createAddButtonMenu(QList<QToolButton*> buttons) {
    QMenu* addButtonMenu = new QMenu();
    for (QToolButton* button : qAsConst(buttons))
        addButtonMenu->addAction(createButtonWidgetAction(button));

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
}

QToolButton* MyModeMenu::decorateRemoveButton(QToolButton* button) {
    return button;
}