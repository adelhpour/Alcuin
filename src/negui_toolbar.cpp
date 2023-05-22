#include "negui_toolbar.h"

// MyToolBar

MyToolBar::MyToolBar(QWidget* parent) : QToolBar(parent) {
    setStyleSheet("QToolBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
};

void MyToolBar::setButtons(QList<QToolButton*> buttons) {
    for (QToolButton* button : qAsConst(buttons))
        addWidget(button);
}
