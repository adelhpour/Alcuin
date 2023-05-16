#include "negui_mode_menu.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setLayout(contentLayout);
}

void MyModeMenu::addButtons(QList<QToolButton*> buttons) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    for (QToolButton* button : qAsConst(buttons))
        contentLayout->addWidget(button, contentLayout->rowCount(), 0);
}