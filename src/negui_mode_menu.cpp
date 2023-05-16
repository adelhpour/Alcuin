#include "negui_mode_menu.h"

#include <QGridLayout>

// MyModeMenu

MyModeMenu::MyModeMenu(QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    contentLayout->addWidget(new MyLabel("Mode"), 0, 0);
    setLayout(contentLayout);
}