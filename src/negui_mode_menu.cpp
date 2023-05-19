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
    QPixmap pixmap("/Users/home/Downloads/feather/mouse-pointer.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());

    return button;
}

void MyModeMenu::setSelectModeButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateSelectButton(button), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateSelectButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Downloads/feather/maximize.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());

    return button;
}

void MyModeMenu::setAddModeButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateAddButton(buttons), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateAddButton(QList<QToolButton*> buttons) {
    QToolButton* button = new MyModeToolButton();
    button->setMenu(createAddButtonMenu(buttons));

    QPixmap pixmap("/Users/home/Downloads/feather/plus-square.svg");
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
}

QToolButton* MyModeMenu::decorateRemoveButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Downloads/feather/minus-square.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}

void MyModeMenu::setZoomInButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateZoomInButton(button), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateZoomInButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Downloads/feather/zoom-in.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}

void MyModeMenu::setZoomOutButton(QToolButton* button) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(decorateZoomOutButton(button), contentLayout->rowCount(), 0);
}

QToolButton* MyModeMenu::decorateZoomOutButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Downloads/feather/zoom-out.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
    return button;
}