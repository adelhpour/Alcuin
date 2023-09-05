#include "negui_customized_mode_menu_widgets.h"

// MyModeMenuToolButton

MyModeMenuToolButton::MyModeMenuToolButton(QWidget* parent) : MyToolButton(parent) {

}

void MyModeMenuToolButton::setActive(const bool& active) {
    if (active)
        setStyleToActiveForm();
    else
        setStyleToInactiveForm();
}

void MyModeMenuToolButton::setStyleToActiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : darkgray; } QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton:hover { background-color: lightgray} QToolButton::menu-indicator {width : 0}");
}

void MyModeMenuToolButton::setStyleToInactiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : white; } QToolButton:pressed {background-color : white; border-radius : 5px} QToolButton:hover { background-color: lightgray} QToolButton::menu-indicator {width : 0}");
}

// MyModeMenuModeButton

MyModeMenuModeButton::MyModeMenuModeButton(const QString& mode, const QString& alternativeSimilarMode, QWidget* parent) : MyModeMenuToolButton(parent) {
    setSceneMode(mode);
    setAlternativeSimilarSceneMode(alternativeSimilarMode);
}
