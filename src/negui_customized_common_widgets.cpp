#include "negui_customized_common_widgets.h"

#include <QGridLayout>

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

MyLabel::MyLabel(const QString& text, const QString& toolTip, QWidget* parent) : QLabel(parent) {
    setContentsMargins(0, 0, 0, 0);
    QFont _font;
    _font.setBold(true);
    _font.setCapitalization(QFont::Capitalize);
    qreal fontSize;
#if defined(Q_OS_WIN)
    fontSize = 8;
#else
    fontSize = 12;
#endif
    _font.setPointSize(fontSize/ qMax(QFontMetrics(font()).width(text) / 110.0, 1.000));
    setFont(_font);

    if (!text.isEmpty())
        setText(text);
    if (!toolTip.isEmpty())
        setToolTip(toolTip);

    setFixedSize(120, 20);
}

// MyTitleLabel

MyTitleLabel::MyTitleLabel(const QString& text, const QString& toolTip, QWidget* parent) : MyLabel(text, toolTip, parent) {
    QFont _font = font();
#if defined(Q_OS_WIN)
    _font.setPointSize(12);
#else
    _font.setPointSize(18);
#endif
    setFont(_font);
    setAlignment(Qt::AlignCenter);
    setFixedSize(160, 25);
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
    setStyleToUnselectedMode();
}

void MyToolButton::setStyleToSelectedMode() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : darkgray; } QToolButton:pressed {background-color : darkgrey; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

void MyToolButton::setStyleToHoveredMode() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : lightgray; } QToolButton:pressed {background-color : darkgrey; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

void MyToolButton::setStyleToUnselectedMode() {
    setStyleSheet("QToolButton {border: 0px; border-radius: 5px; background-color : white; } QToolButton:pressed {background-color : darkgrey; border-radius : 5px} QToolButton::menu-indicator {width : 0}");
}

bool MyToolButton::event(QEvent* event) {
    if (event->type() == QEvent::Enter)
        setStyleToHoveredMode();
    else if (event->type() == QEvent::Leave || event->type() == QEvent::Hide || event->type() == QEvent::WindowUnblocked)
        setStyleToUnselectedMode();

    return QToolButton::event(event);
}

// MyMenu

MyMenu::MyMenu(QWidget* parent) : QMenu(parent) {
    setStyleSheet("QMenu { background-color: white; border-radius: 10px;} ");
}


// MyDialog

MyDialog::MyDialog(QWidget *parent) : QDialog(parent) {
    setStyleSheet("QDialog {background-color: white;}");
    resize(350, 150);
    setLayout(new QGridLayout());
}

void MyDialog::setButtons() {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QDialogButtonBox* buttons = new QDialogButtonBox(Qt::Horizontal, this);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    QObject::connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    contentLayout->addWidget(buttons, contentLayout->rowCount(), 0, 1, 2);

}

// MyAutoSaveMessageBox

MyAutoSaveMessageBox::MyAutoSaveMessageBox(const QString& networkName, QWidget *parent) {
    setWindowTitle("Save Action");
    setText("Do you want to save the changes made to the network \"" + networkName + "\"?");
    setInformativeText("Your changes will be lost if you don’t save them.");
    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    setDefaultButton(QMessageBox::Yes);
}
