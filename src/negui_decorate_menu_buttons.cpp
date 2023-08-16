#include "negui_decorate_menu_buttons.h"
#include "negui_customized_common_widgets.h"

#include <QShortcut>

void decorateImportButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Open");
    button->setToolTip("Open");
    setIcon(button, iconsDirectory + "/open.png");
}

void decorateDataExportButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("SaveAs");
    button->setToolTip("SaveAs");
    setIcon(button, iconsDirectory + "/saveAs.png");
}

void decoratePrintExportButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Export");
    button->setToolTip("Export");
    setIcon(button, iconsDirectory + "/export.png");
}

void decorateSaveButton(QToolButton* button, const QString& iconsDirectory) {
    QShortcut* shortcut = new QShortcut(QKeySequence::Save, button, SLOT(click()));
    button->setText("Save");
    button->setToolTip("Save");
    setIcon(button, iconsDirectory + "/save.png");
}

void decorateAutoLayoutButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("AutoLayout");
    button->setToolTip("AutoLayout");
    setIcon(button, iconsDirectory + "/autoLayout.png");
}

void decorateUndoActionButton(QToolButton* button, const QString& iconsDirectory) {
    button->setShortcut(QKeySequence::Undo);
    setIcon(button, iconsDirectory + "/undo.png");
}

void decorateRedoActionButton(QToolButton* button, const QString& iconsDirectory) {
    button->setShortcut(QKeySequence::Redo);
    setIcon(button, iconsDirectory + "/redo.png");
}

void decorateResetSceneButton(QToolButton* button, const QString& iconsDirectory) {
    QShortcut* shortcut = new QShortcut(QKeySequence::New, button, SLOT(click()));
    button->setText("New");
    button->setToolTip("New");
    setIcon(button, iconsDirectory + "/new.png");
}

void decorateNormalModeButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Normal Mode");
    button->setToolTip("Set the scene mode to the normal mode");
    setIcon(button, iconsDirectory + "/normal.png");
}

void decorateZoomInButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Zoom In");
    button->setToolTip("Zooming in");
    setIcon(button, iconsDirectory + "/zoomIn.png");
}

void decorateZoomOutButton(QToolButton* button, const QString& iconsDirectory) {
    button->setText("Zoom Out");
    button->setToolTip("Zooming out");
    setIcon(button, iconsDirectory + "/zoomOut.png");
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
    button->setText("Add Element");
    button->setToolTip("Choose an element to add it ot the scene");
    setIcon(button, iconsDirectory + "/add.png");
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

void decorateCloseFeatureMenuButton(QPushButton* button, const QString& iconsDirectory) {
    button->setToolTip("Close Window");
    setIcon(button, iconsDirectory + "/close.png");

#if defined(Q_OS_WIN)
    button->setIconSize(QSize(20, 20));
#else
    button->setIconSize(QSize(15, 15));
#endif
    button->setStyleSheet("QPushButton {background-color: transparent; border-width: 0px; border-color: transparent; }");
}

void setIcon(QAbstractButton* button, const QString& iconPath) {
    QPixmap pixmap;
    pixmap.load(iconPath);
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
#if defined(Q_OS_WIN)
    button->setIconSize(QSize(60, 60));
#else
    button->setIconSize(pixmap.rect().size());
#endif
}