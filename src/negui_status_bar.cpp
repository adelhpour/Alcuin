#include "negui_status_bar.h"

// MyStatusBar

MyStatusBar::MyStatusBar(QWidget* parent) : QStatusBar(parent) {
    setStyleSheet("QStatusBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(25);
    _fileNameLabel = new QLabel();
    _coordinatesLabel = new QLabel();
    _coordinatesLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    addPermanentWidget(_coordinatesLabel, 40);
    _magnificationLabel = new QLabel("Magnification: 1");
    _magnificationLabel->setAlignment(Qt::AlignCenter);
    addPermanentWidget(_magnificationLabel, 20);
    _fileNameLabel = new QLabel();
    _fileNameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    addPermanentWidget(_fileNameLabel, 40);

    resetMessage();
}

void MyStatusBar::resetMessage() {
    _coordinatesLabel->setText("");
}

void MyStatusBar::setFileName(const QString& fileName) {
    _fileNameLabel->setText(fileName);
}

void MyStatusBar::setMagnificationFactor(const qreal magnificationFactor) {
    _magnificationLabel->setText("Magnification: " + QString::number(magnificationFactor));
}

void MyStatusBar::setCoordinatesToMousePosition(const QPointF& mousePosition) {
    _coordinatesLabel->setText(QString::number(mousePosition.toPoint().x()) + ", " + QString::number(mousePosition.toPoint().y()));
}