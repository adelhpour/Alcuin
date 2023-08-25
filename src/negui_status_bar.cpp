#include "negui_status_bar.h"

// MyStatusBar

MyStatusBar::MyStatusBar(QWidget* parent) : QStatusBar(parent) {
    setStyleSheet("QStatusBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setFixedHeight(25);
    _fileNameLabel = new QLabel();
    _coordinatesLabel = new QLabel();
    addPermanentWidget(_fileNameLabel, 97);
    addPermanentWidget(_coordinatesLabel, 3);

    resetMessage();
}

void MyStatusBar::resetMessage() {
    _coordinatesLabel->setText("");
}

void MyStatusBar::setFileName(const QString& fileName) {
    _fileNameLabel->setText(fileName);
}

void MyStatusBar::setCoordinatesToMousePosition(const QPointF& mousePosition) {
    _coordinatesLabel->setText(QString::number(mousePosition.toPoint().x()) + ", " + QString::number(mousePosition.toPoint().y()));
}