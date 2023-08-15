#include "negui_status_bar.h"

// MyStatusBar

MyStatusBar::MyStatusBar(QWidget* parent) : QStatusBar(parent) {
    setStyleSheet("QStatusBar {background-color : white; border: 1px solid lightgray;  border-radius: 5px;}");
    setContentsMargins(0, 0, 0, 0);
};