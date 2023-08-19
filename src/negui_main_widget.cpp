#include "negui_main_widget.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_mode_menu.h"
#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"
#include "negui_status_bar.h"

#include <QGridLayout>
#include <QSettings>

// MyNetworkEditorWidget

MyNetworkEditorWidget::MyNetworkEditorWidget(QWidget *parent) :  QFrame(parent) {
    setObjectName("main_widget");
    setStyleSheet("QFrame {background-color : white}");
    
    setMinimumSize(120, 80);
    resize(1050, 700);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    readSettings();

    setWidgets();
    setInteractions();
    
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(title(), 0, 0, 1, 3, Qt::AlignCenter);
    layout->addWidget(toolBar(), 1, 0, 1, 3);
    layout->addWidget(modeMenu(), 2, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(view(), 2, 1, 1, 1);
    layout->addWidget(statusBar(), 3, 0, 1, 3);
    setLayout(layout);

    setReadyToLaunch();
}

MyNetworkEditorWidget::~MyNetworkEditorWidget() {
    close();
}

void MyNetworkEditorWidget::setWidgets() {
    _title = new QLabel(this);
    _toolBar = new MyToolBar(this);
    _modeMenu = new MyModeMenu(this);
    _view = new MyGraphicsView(this);
    _statusBar = new MyStatusBar(this);
    _interactor = new MyInteractor(this);
    _featureMenu = NULL;

    QString iconsDirectoryPath = ((MyInteractor*)interactor())->iconsDirectory().path();
    ((MyToolBar*)toolBar())->setButtons(((MyInteractor*)interactor())->getToolBarMenuButtons());
    ((MyModeMenu*)modeMenu())->setNormalModeButton(((MyInteractor*)interactor())->getNormalModeButton(), iconsDirectoryPath);
    ((MyModeMenu*)modeMenu())->setAddModeButtons(((MyInteractor*)interactor())->getAddModeButtons(), iconsDirectoryPath);
    ((MyModeMenu*)modeMenu())->setZoomInButton(((MyGraphicsView*)view())->getZoomInButton(), iconsDirectoryPath);
    ((MyModeMenu*)modeMenu())->setZoomOutButton(((MyGraphicsView*)view())->getZoomOutButton(), iconsDirectoryPath);
}

void MyNetworkEditorWidget::setInteractions() {
    /// main widget
    // set title
    connect((MyInteractor*)interactor(), &MyInteractor::currentFileNameIsUpdated, this, [this] (const QString& titleText) { ((QLabel*)title())->setText(titleText); });

    // menubar
    connect(this, SIGNAL(askForSetNewNetworkCanvas()), (MyInteractor*)interactor(), SLOT(setNewNetworkCanvas()));
    connect(this, SIGNAL(askForListOfPluginItemNames(const QString&)), (MyInteractor*)interactor(), SLOT(listOfPluginItemNames(const QString&)));
    connect(this, SIGNAL(askForListOfPluginItemCategories(const QString&)), (MyInteractor*)interactor(), SLOT(listOfPluginItemCategories(const QString&)));
    connect(this, SIGNAL(askForReadFromFile(const QString&)), (MyInteractor*)interactor(), SLOT(readFromFile(const QString&)));
    connect(this, SIGNAL(askForSaveCurrentNetwork()), (MyInteractor*)interactor(), SLOT(saveCurrentNetwork()));
    connect(this, SIGNAL(askForWriteDataToFile(const QString&)), (MyInteractor*)interactor(), SLOT(writeDataToFile(const QString&)));
    connect(this, SIGNAL(askForWriteFigureToFile(const QString&)), (MyInteractor*)interactor(), SLOT(writeFigureToFile(const QString&)));
    connect(this, SIGNAL(askForTriggerUndoAction()), ((MyInteractor*)interactor())->undoStack(), SLOT(undo()));
    connect(((MyInteractor*)interactor())->undoStack(), SIGNAL(canUndoChanged(const bool&)), this, SIGNAL(canUndoChanged(const bool&)));
    connect(this, SIGNAL(askForTriggerRedoAction()), ((MyInteractor*)interactor())->undoStack(), SLOT(redo()));
    connect(((MyInteractor*)interactor())->undoStack(), SIGNAL(canRedoChanged(const bool&)), this, SIGNAL(canRedoChanged(const bool&)));
    connect(this, SIGNAL(askForCutSelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(cutSelectedNetworkElements()));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCuttableStatusChanged(const bool&)), this, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect(this, SIGNAL(askForCopySelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(copySelectedNetworkElements()));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCopyableStatusChanged(const bool&)), this, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect(this, SIGNAL(askForPasteCopiedNetworkElements()), (MyInteractor*)interactor(), SLOT(pasteCopiedNetworkElements()));
    connect((MyInteractor*)interactor(), SIGNAL(pasteElementsStatusChanged(const bool&)), this, SIGNAL(pasteElementsStatusChanged(const bool&)));
    connect(this, QOverload<>::of(&MyNetworkEditorWidget::askForSelectAllElements), (MyInteractor*)interactor(), [this] () { ((MyInteractor*)this->interactor())->selectElements(true); });
    connect(this, QOverload<const QString&>::of(&MyNetworkEditorWidget::askForSelectAllElements), (MyInteractor*)interactor(), [this] (const QString& category) { ((MyInteractor*)this->interactor())->selectElements(true, category); });
    connect(this, SIGNAL(askForZoomIn()), (MyGraphicsView*)view(), SLOT(zoomIn()));
    connect(this, SIGNAL(askForZoomOut()), (MyGraphicsView*)view(), SLOT(zoomOut()));

    /// feature menu
    // display feature menu
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));

    /// mode menu
    // set mode
    connect((MyInteractor*)interactor(), SIGNAL(modeIsSet(const QString&)), modeMenu(), SLOT(setMode(const QString&)));

    /// graphics view
    // export screen scene
    connect((MyInteractor*)interactor(), SIGNAL(askForExportFigure(const QString&, const QString&)), (MyGraphicsView*)view(), SLOT(exportFigure(const QString&, const QString&)));
    
    // set tool tip
    connect((MyInteractor*)interactor(), SIGNAL(askForSetToolTip(const QString&)), (MyGraphicsView*)view(), SLOT(setToolTip(const QString&)));
    
    // reset scale
    connect((MyInteractor*)interactor(), SIGNAL(askForResetScale()), (MyGraphicsView*)view(), SLOT(resetScale()));

    // enter key pressed
    connect((MyGraphicsView*)view(), SIGNAL(enterKeyIsPressed()), (MyInteractor*)interactor(), SIGNAL(enterKeyIsPressed()));

    /// graphics scene
    // set scene mode
    connect((MyInteractor*)interactor(), &MyInteractor::modeIsSet, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] (const QString& mode) { ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->setSceneMode(mode); });

    // set scene rect
    connect((MyInteractor*)interactor(), SIGNAL(askForSetSceneRect(qreal, qreal, qreal, qreal)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(setSceneRect(qreal, qreal, qreal, qreal)));
    
    // add graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(addGraphicsItem(QGraphicsItem*)));
    
    // remove graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(removeGraphicsItem(QGraphicsItem*)));
    
    // reset scene
    connect((MyInteractor*)interactor(), SIGNAL(askForClearScene()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(clearScene()));
    
    // items at position
    connect((MyInteractor*)interactor(), SIGNAL(askForItemsAtPosition(const QPointF&)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(itemsAtPosition(const QPointF&)));

    // activated shift modifier
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherShiftModifierIsPressed()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(isShiftModifierPressed()));

    // activated shift modifier
    connect((MyInteractor*)interactor(), &MyInteractor::askForItemsBoundingRect, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] () { return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->itemsBoundingRect(); });

    // select all
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForSelectAll, (MyInteractor*)interactor(), [this] () { ((MyInteractor*)this->interactor())->selectElements(true); });
    
    // add node
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(addNewNode(const QPointF&)));

    // display the element selection rectangle
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mousePressedLeftButtonIsMoved(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsReleased()), (MyInteractor*)interactor(), SLOT(clearSelectionArea()));

    // change mode
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::escapeKeyIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForEnableNormalMode, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);

    // remove menu
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(removeFeatureMenu()));

    // context menu
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherSelectedElementsAreCopyable()), (MyInteractor*)interactor(), SLOT(areSelectedElementsCopyable()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherSelectedElementsAreCuttable()), (MyInteractor*)interactor(), SLOT(areSelectedElementsCuttable()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherAnyElementsAreCopied()), (MyInteractor*)interactor(), SLOT(areAnyElementsCopied()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherAnyElementsAreSelected()), (MyInteractor*)interactor(), SLOT(areAnyElementsSelected()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForCopySelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(copySelectedNetworkElements()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForCutSelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(cutSelectedNetworkElements()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForPasteCopiedNetworkElements(const QPointF &)), (MyInteractor*)interactor(), SLOT(pasteCopiedNetworkElements(const QPointF &)));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForDeleteSelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(deleteSelectedNetworkElements()));

    // status bar
    connect(view(), SIGNAL(mouseLeft()), statusBar(), SLOT(resetMessage()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(mousePositionIsChanged(const QPointF&)), statusBar(), SLOT(setMessageToMousePosition(const QPointF&)));
}

QObject* MyNetworkEditorWidget::interactor() {
    return _interactor;
}

QWidget* MyNetworkEditorWidget::title() {
    return _title;
}

QWidget* MyNetworkEditorWidget::toolBar() {
    return _toolBar;
}

QWidget* MyNetworkEditorWidget::modeMenu() {
    return _modeMenu;
}

QWidget* MyNetworkEditorWidget::view() {
    return _view;
}

QWidget* MyNetworkEditorWidget::statusBar() {
    return _statusBar;
}

void MyNetworkEditorWidget::displayFeatureMenu(QWidget* featureMenu) {
    removeFeatureMenu();
    ((QGridLayout*)layout())->addWidget(featureMenu, 2, 2, 1, 1, Qt::AlignTop);
    _featureMenu = featureMenu;
}

void MyNetworkEditorWidget::removeFeatureMenu() {
    if (_featureMenu) {
        layout()->removeWidget(_featureMenu);
        _featureMenu->deleteLater();
        _featureMenu = NULL;
    }
}

void MyNetworkEditorWidget::setReadyToLaunch() {
    ((MyInteractor*)interactor())->setNewNetworkCanvas();
}

void MyNetworkEditorWidget::readSettings() {
    QSettings settings("MyCompany", "NetworkEditorGUI");
    settings.beginGroup("NetworkEditorWidget");
    const auto geometry1 = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry1.isEmpty())
        setGeometry(200, 200, 1050, 700);
    else
        restoreGeometry(geometry1);
    settings.endGroup();
}

void MyNetworkEditorWidget::writeSettings() {
    QSettings settings("MyCompany", "NetworkEditorGUI");
    settings.beginGroup("NetworkEditorWidget");
    if (dynamic_cast<QWidget*>(parent()))
        settings.setValue("geometry", ((QWidget*)parent())->saveGeometry());
    else
        settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

void MyNetworkEditorWidget::closeEvent(QCloseEvent *event) {
    writeSettings();
    QWidget::closeEvent(event);
}