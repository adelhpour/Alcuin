#include "negui_decorate_menu_buttons.h"
#include "negui_customized_common_widgets.h"

#include <QShortcut>

void decorateImportButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Open");
    button->setToolTip("Open");
    setIcon(button, iconsDirectory + "/folder.svg");
}

void decorateExportButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("SaveAs");
    button->setToolTip("SaveAs");
    setIcon(button, iconsDirectory + "/share.svg");
}

void decorateSaveButton(QToolButton* button, const QString& iconsDirectory) {
    QShortcut* shortcut = new QShortcut(QKeySequence::Save, button, SLOT(click()));
    button->setText("Save");
    button->setToolTip("Save");
    setIcon(button, iconsDirectory + "/save.svg");
}

void decorateAutoLayoutButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("AutoLayout");
    button->setToolTip("AutoLayout");
    setIcon(button, iconsDirectory + "/share-2.svg");
}

void decorateUndoActionButton(QToolButton* button, const QString& iconsDirectory) {
    button->setShortcut(QKeySequence::Undo);
    setIcon(button, iconsDirectory + "/rotate-ccw.svg");
}

void decorateRedoActionButton(QToolButton* button, const QString& iconsDirectory) {
    button->setShortcut(QKeySequence::Redo);
    setIcon(button, iconsDirectory + "/rotate-cw.svg");
}

void decorateResetSceneButton(QToolButton* button, const QString& iconsDirectory) {
    QShortcut* shortcut = new QShortcut(QKeySequence::New, button, SLOT(click()));
    button->setText("New");
    button->setToolTip("New");
    setIcon(button, iconsDirectory + "/file.svg");
}

void decorateNormalModeButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Normal Mode");
    button->setToolTip("Set the scene mode to the normal mode");
    setIcon(button, iconsDirectory + "/mouse-pointer.svg");
}

void decorateZoomInButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Zoom In");
    button->setToolTip("Zooming in");
    setIcon(button, iconsDirectory + "/zoom-in.svg");
}

void decorateZoomOutButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Zoom Out");
    button->setToolTip("Zooming out");
    setIcon(button, iconsDirectory + "/zoom-out.svg");
}

QToolButton* decorateAddModeButton(QList<QToolButton*> buttons, const QString& iconsDirectory) {
    QToolButton* button = new MyModeToolButton("Add");
    button->setText("Add");
    button->setToolTip("Add an item to the network");
    button->setMenu(createAddButtonMenu(buttons));
    decorateAddModeButton(button, iconsDirectory);

    return button;
}

void decorateAddModeButton(QToolButton* button, const QString& iconsDirectory) {
    QPixmap pixmap(iconsDirectory + "/plus-square.svg");
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
}

QMenu* createAddButtonMenu(QList<QToolButton*> buttons) {
    QMenu* addButtonMenu = new QMenu();
    for (QToolButton* button : qAsConst(buttons)) {
        addButtonMenu->addAction(createButtonWidgetAction(button, addButtonMenu));
        QObject::connect((MyToolButton*)button, &MyToolButton::menuItemIsChosen, addButtonMenu, [addButtonMenu] () { addButtonMenu->close(); });
    }

    return addButtonMenu;
}

QWidgetAction* createButtonWidgetAction(QToolButton* button, QWidget* parent) {
    QWidgetAction* widgetAction = new QWidgetAction(parent);
    widgetAction->setDefaultWidget(button);

    return widgetAction;
}

void setIcon(QToolButton* button, const QString& iconPath) {
    QPixmap pixmap(iconPath);
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(pixmap.rect().size());
}