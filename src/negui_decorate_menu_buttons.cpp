#include "negui_decorate_menu_buttons.h"
#include "negui_customized_common_widgets.h"

void decorateImportButton(QToolButton* button) {
    button->setText("Import");
    button->setToolTip("Import from a file");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/file-plus.svg");
}

void decorateExportButton(QToolButton* button) {
    button->setText("Export");
    button->setToolTip("Export to a file");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/save.svg");
}

void decorateAutoLayoutButton(QToolButton* button) {
    button->setText("AutoLayout");
    button->setToolTip("Automatically set the position of the nodes and edges");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/share-2.svg");
}

void decorateUndoActionButton(QToolButton* button) {
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/rotate-ccw.svg");
}

void decorateRedoActionButton(QToolButton* button) {
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/rotate-cw.svg");
}

void decorateResetSceneButton(QToolButton* button) {
    button->setText("Reset");
    button->setToolTip("Remove all network elements from the scene");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/trash-2.svg");
}

void decorateNormalModeButton(QToolButton* button) {
    button->setText("Normal Mode");
    button->setToolTip("Set the scene mode to the normal mode");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/mouse-pointer.svg");
}

void decorateSelectModeButton(QToolButton* button) {
    button->setText("Select Mode");
    button->setToolTip("Set the scene mode to the select mode");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/maximize.svg");
}

void decorateRemoveModeButton(QToolButton* button) {
    button->setText("Remove");
    button->setToolTip("Remove an item from the network");
    setIcon(button, "/Users/home/Documents/Education/SystemsBiology/Projects/Made/NetworkEditorGUI/NetworkEditorGUI/NetworkEditorGUI/icons/minus-square.svg");
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