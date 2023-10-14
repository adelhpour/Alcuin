#include "negui_toolbar.h"

// MyToolBar

MyToolBar::MyToolBar(QWidget* parent) : QToolBar(parent) {
    setStyleSheet("QToolBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setIconSize(QSize(45, 45));
    setFixedHeight(45);
};

void MyToolBar::addButtons(QList<QAbstractButton*> buttons) {
    for (QAbstractButton* button : qAsConst(buttons))
        addWidget(button);
}
