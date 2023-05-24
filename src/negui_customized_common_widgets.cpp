#include "negui_customized_common_widgets.h"

// MyFrame

MyFrame::MyFrame(QWidget* parent) : QFrame(parent) {
    setStyleSheet("QFrame { background-color: white; border: no-border;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

// MySpacerItem

MySpacerItem::MySpacerItem(int w, int h) : QSpacerItem(w, h, QSizePolicy::Fixed, QSizePolicy::Fixed) {

}

// MyLabel

MyLabel::MyLabel(const QString& text, QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont _font;
    _font.setBold(true);
    _font.setCapitalization(QFont::Capitalize);
    _font.setPointSize(12);
    setFont(_font);

    if (!text.isEmpty())
        setText(text);

    setFixedSize(120, 20);
}

// MyTitleLabel

MyTitleLabel::MyTitleLabel(const QString& text, QWidget* parent) : MyLabel(text, parent) {
    QFont _font = font();
    _font.setPointSize(18);
    setFont(_font);
    setFixedSize(160, 25);
    setAlignment(Qt::AlignCenter);
}

// MyLineEdit

MyLineEdit::MyLineEdit(const QString &contents, QWidget* parent) : QLineEdit(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QLineEdit {border: 1px solid LightSlateGray; border-radius: 10px; padding: 0 8px; background: GhostWhite; selection-background-color: LightGray;} QLineEdit::read-only {color: gray}");
    setText(contents);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

void MyLineEdit::setText(const QString &contents) {
    QLineEdit::setText(contents);
    setToolTip(contents);
    setCursorPosition(0);
}

// MyReadOnlyLineEdit

MyReadOnlyLineEdit::MyReadOnlyLineEdit(const QString &contents, QWidget* parent) : MyLineEdit(contents, parent) {
    setReadOnly(true);
}

// MySpinBox

MySpinBox::MySpinBox(QWidget* parent) : QSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QSpinBox::up-button { background-color: transparent; }" "QSpinBox::down-button { background-color: transparent;}" "QSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyDoubleSpinBox

MyDoubleSpinBox::MyDoubleSpinBox(QWidget* parent) : QDoubleSpinBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    setStyleSheet("QDoubleSpinBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; margin-right: 15px }" "QDoubleSpinBox::up-button { background-color: transparent; }" "QDoubleSpinBox::down-button { background-color: transparent;}" "QDoubleSpinBox::up-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-bottom: 5px solid black; width: 0px; height: 0px; }" "QDoubleSpinBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; }");
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyComboBox

MyComboBox::MyComboBox(QWidget* parent) : QComboBox(parent) {
    setContentsMargins(0, 0, 0, 0);
    setStyleSheet("QComboBox { border: 1px solid LightSlateGray; border-radius: 5px; background-color: GhostWhite; color: black; padding: 1px 18px 1px 3px; }" "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left-width: 1px; border-left-color: LightSlateGray;border-left-style: solid; border-top-right-radius: 5px; border-bottom-right-radius: 5px; }" "QComboBox::down-arrow { border-left: 4px solid none;" "border-right: 4px solid none; border-top: 5px solid black; width: 0px; height: 0px; } QComboBox::disabled { color: gray;} QComboBox QAbstractItemView { border: 1px solid LightSlateGray; background-color: white; min-width: 100px; }");

    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setFixedSize(120, 20);
}

// MyToolButton

MyToolButton::MyToolButton(QWidget* parent) : QToolButton(parent) {
    setPopupMode(QToolButton::InstantPopup);
    setStyleSheet("QToolButton {border: 0px; background-color : white; } QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

// MyModeToolButton

MyModeToolButton::MyModeToolButton(const QString& mode, QWidget* parent) : MyToolButton(parent) {
    _mode = mode;
}

void MyModeToolButton::setActive(const bool& active) {
    if (active)
        setStyleToActiveForm();
    else
        setStyleToInactiveForm();
}

void MyModeToolButton::setStyleToActiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : darkgray; } QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

void MyModeToolButton::setStyleToInactiveForm() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : white; } QToolButton:pressed {background-color : darkgray; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

const QString& MyModeToolButton::mode() {
    return _mode;
}

// MyToolButtonMenu

MyToolButtonMenu::MyToolButtonMenu(QWidget* parent) : QMenu(parent) {
    setStyleSheet("QMenu { background-color: white; border-radius: 10px;} ");
    connect(this, &MyToolButtonMenu::menuItemIsChosen, this, [this] () { close(); });
}

// MyToolButtonCategoryMenu

MyToolButtonCategoryMenu::MyToolButtonCategoryMenu(QWidget* parent) : MyToolButtonMenu(parent) {
    _horizontalPadding = 80;
}

bool MyToolButtonCategoryMenu::event(QEvent *event) {
    if (event->type() == QEvent::Show)
        move(pos() + QPoint(_horizontalPadding, 0));

    return QMenu::event(event);
}