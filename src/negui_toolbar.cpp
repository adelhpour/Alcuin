#include "negui_toolbar.h"

// MyToolBar

MyToolBar::MyToolBar(QWidget* parent) : QToolBar(parent) {
    setStyleSheet("QToolBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
#if defined(Q_OS_WIN)
    setIconSize(QSize(60, 60));
#endif
};

void MyToolBar::setButtons(QList<QAbstractButton*> buttons) {
    for (QAbstractButton* button : qAsConst(buttons))
        addWidget(button);
}
