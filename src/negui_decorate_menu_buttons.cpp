#include "negui_decorate_menu_buttons.h"
#include "negui_customized_common_widgets.h"

#include <QShortcut>

void decorateImportButton(QToolButton* button) {
    button->setText("Import");
    button->setToolTip("Import from a file");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/folder.svg");
}

void decorateExportButton(QToolButton* button) {
    button->setText("Export");
    button->setToolTip("Export to a file");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/share.svg");
}

void decorateSaveButton(QToolButton* button) {
    QShortcut* shortcut = new QShortcut(QKeySequence::Save, button, SLOT(click()));
    button->setText("Save");
    button->setToolTip("Save to a file");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/save.svg");
}

void decorateAutoLayoutButton(QToolButton* button) {
    button->setText("AutoLayout");
    button->setToolTip("Automatically set the position of the nodes and edges");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/share-2.svg");
}

void decorateUndoActionButton(QToolButton* button) {
    button->setShortcut(QKeySequence::Undo);
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/rotate-ccw.svg");
}

void decorateRedoActionButton(QToolButton* button) {
    button->setShortcut(QKeySequence::Redo);
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/rotate-cw.svg");
}

void decorateResetSceneButton(QToolButton* button) {
    QShortcut* shortcut = new QShortcut(QKeySequence::New, button, SLOT(click()));
    button->setText("Reset");
    button->setToolTip("Remove all network elements from the scene and create a new canvas");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/file.svg");
}

void decorateNormalModeButton(QToolButton* button) {
    button->setText("Normal Mode");
    button->setToolTip("Set the scene mode to the normal mode");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/mouse-pointer.svg");
}

void decorateZoomInButton(QToolButton* button) {
    button->setText("Zoom In");
    button->setToolTip("Zooming in");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/zoom-in.svg");
}

void decorateZoomOutButton(QToolButton* button) {
    button->setText("Zoom Out");
    button->setToolTip("Zooming out");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/zoom-out.svg");
}

QToolButton* decorateAddModeButton(QList<QToolButton*> buttons) {
    QToolButton* button = new MyModeToolButton("Add");
    button->setText("Add");
    button->setToolTip("Add an item to the network");
    button->setMenu(createAddButtonMenu(buttons));
    decorateAddModeButton(button);

    return button;
}

void decorateAddModeButton(QToolButton* button) {
    QPixmap pixmap("/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/plus-square.svg");
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