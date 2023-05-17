#include "negui_mode_menu.h"
#include "negui_customized_mode_menu_widgets.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenu::setAddButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(createAddButton(buttons), contentLayout->rowCount(), 0);
}

void MyModeMenu::setRemoveButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(button, contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::createAddButton(QList<QToolButton*> buttons) {
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