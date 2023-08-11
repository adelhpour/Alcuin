#include "negui_about_message_box.h"

MyAboutMessageBox::MyAboutMessageBox(const QString& name, const QString& version, QWidget *parent) {
    setWindowTitle("App Info");
    setText(name.toUpper() + " Version " + version);
}