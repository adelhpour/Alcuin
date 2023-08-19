#include "negui_status_bar.h"

#include <QLabel>

// MyStatusBar

MyStatusBar::MyStatusBar(QWidget* parent) : QStatusBar(parent) {
    setStyleSheet("QStatusBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
    resetMessage();
}

void MyStatusBar::resetMessage() {
    showMessage(tr("Ready"));
}

void MyStatusBar::setMessageToMousePosition(const QPointF& mousePosition) {
    showMessage(QString::number(mousePosition.toPoint().x()) + ", " + QString::number(mousePosition.toPoint().y()));
}