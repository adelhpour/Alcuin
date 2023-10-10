#include "negui_main_widget.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_mode_menu.h"
#include "negui_graphics_view.h"
#include "negui_secondary_graphics_view.h"
#include "negui_graphics_scene.h"
#include "negui_status_bar.h"
#include "negui_null_feature_menu.h"

#include <QGridLayout>
#include <QSettings>
#include <QStandardPaths>

// MyNetworkEditorWidget

MyNetworkEditorWidget::MyNetworkEditorWidget(QWidget *parent) :  QFrame(parent) {
    setObjectName("main_widget");
    setStyleSheet("QFrame {background-color : white}");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setWidgets();
    setInteractions();
    
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(toolBar(), layout->rowCount(), 0, 1, 3);
    _layoutMenuRow = layout->rowCount();
    layout->addWidget(modeMenu(), layout->rowCount(), 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(view(), layout->rowCount() - 1, 1);
    layout->addWidget(secondaryView(), layout->rowCount() - 1, 1, Qt::AlignBottom | Qt::AlignRight);
    layout->addWidget(statusBar(), layout->rowCount(), 0, 1, 3);
    setLayout(layout);
    arrangeWidgetLayers();

    readSettings();
    setReadyToLaunch();
}

MyNetworkEditorWidget::~MyNetworkEditorWidget() {
    close();
}

void MyNetworkEditorWidget::setWidgets() {
    _toolBar = new MyToolBar(this);
    _statusBar = new MyStatusBar(this);
    _modeMenu = new MyModeMenu(this);
    _view = new MyGraphicsView(this);
    _secondaryView = new MySecondaryGraphicsView(((MyGraphicsView*)view())->scene());
    _interactor = new MyInteractor(this);
    _featureMenu = NULL;

    ((MyToolBar*)toolBar())->addButtons(((MyInteractor*)interactor())->getToolbarMenuButtons() + ((MyGraphicsView*)view())->getToolbarMenuButtons(((MyInteractor*)interactor())->iconsDirectory().path()));
    ((MyModeMenu*)modeMenu())->addButtons(((MyInteractor*)interactor())->getModeMenuButtons());
}

void MyNetworkEditorWidget::setInteractions() {
    /// main widget
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
    connect(this, QOverload<>::of(&MyNetworkEditorWidget::askForSelectAllElements), (MyInteractor*)interactor(), [this] () {
        ((MyInteractor*)this->interactor())->selectElements(true);
        ((MyInteractor*)this->interactor())->displayFeatureMenu();
    });
    connect(this, QOverload<const QString&>::of(&MyNetworkEditorWidget::askForSelectAllElements), (MyInteractor*)interactor(), [this] (const QString& category) {
        ((MyInteractor*)this->interactor())->selectElements(true, category);
        ((MyInteractor*)this->interactor())->displayFeatureMenu();
    });
    connect(this, SIGNAL(askForZoomIn()), (MyGraphicsView*)view(), SLOT(zoomIn()));
    connect(this, SIGNAL(askForZoomOut()), (MyGraphicsView*)view(), SLOT(zoomOut()));

    /// feature menu
    // display feature menu
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayFeatureMenu()), this, SLOT(displayFeatureMenu()));
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
    connect((MyInteractor*)interactor(), &MyInteractor::askForCurrentlyBeingDisplayedNetworkElementFeatureMenu, this, [this] () { return featureMenu(); } );

    /// mode menu
    // set mode
    connect((MyInteractor*)interactor(), SIGNAL(modeIsSet(const QString&)), modeMenu(), SLOT(setMode(const QString&)));
    connect((MyInteractor*)interactor(), SIGNAL(addElementModeIsEnabled(const QString &)), modeMenu(), SLOT(activateAddElementButton(const QString&)));

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
    // change stage command
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(askForCreateChangeStageCommand()), (MyInteractor*)interactor(), SLOT(createChangeStageCommand()));

    // set scene mode
    connect((MyInteractor*)interactor(), &MyInteractor::modeIsSet, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] (const QString& mode) { ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->setSceneMode(mode); });
    
    // add graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(addGraphicsItem(QGraphicsItem*)));
    
    // remove graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(removeGraphicsItem(QGraphicsItem*)));

    // network background color
    connect((MyInteractor*)interactor(), SIGNAL(askForNetworkBackgroundColor()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(backgroundColor()));
    connect((MyInteractor*)interactor(), SIGNAL(askForSetNetworkBackgroundColor(const QString&)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(setBackgroundColor(const QString&)));

    // network extents
    connect((MyInteractor*)interactor(), SIGNAL(askForNetworkExtents()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(networkExtents()));

    // reset scene
    connect((MyInteractor*)interactor(), SIGNAL(askForClearScene()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(clearScene()));
    connect((MyInteractor*)interactor(), SIGNAL(askForClearScene()), this, SLOT(removeFeatureMenu()));
    
    // items at position
    connect((MyInteractor*)interactor(), SIGNAL(askForItemsAtPosition(const QPointF&)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(itemsAtPosition(const QPointF&)));

    // activated shift modifier
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherShiftModifierIsPressed()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(isShiftModifierPressed()));

    // activated control modifier
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherControlModifierIsPressed()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(isControlModifierPressed()));

    // items bounding rect
    connect((MyInteractor*)interactor(), &MyInteractor::askForItemsBoundingRect, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] () { return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->itemsBoundingRect(); });

    // select all
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForSelectAll, (MyInteractor*)interactor(), [this] () { ((MyInteractor*)this->interactor())->selectElements(true); });
    
    // add node
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(addNewNode(const QPointF&)));

    // display the element selection rectangle
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mousePressedLeftButtonIsMoved(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsReleased()), (MyInteractor*)interactor(), SLOT(clearSelectionArea()));

    // display null feature menu
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(escapeKeyIsPressed()), this, SLOT(displayFeatureMenu()));

    // change mode
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::escapeKeyIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForEnableNormalMode, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);

    // context menu
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherSelectedElementsAreCopyable()), (MyInteractor*)interactor(), SLOT(areSelectedElementsCopyable()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherSelectedElementsAreCuttable()), (MyInteractor*)interactor(), SLOT(areSelectedElementsCuttable()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherAnyElementsAreCopied()), (MyInteractor*)interactor(), SLOT(areAnyElementsCopied()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherAnyElementsAreSelected()), (MyInteractor*)interactor(), SLOT(areAnyElementsSelected()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForWhetherAnyElementsAreAlignable()), (MyInteractor*)interactor(), SLOT(areSelectedElementsAlignable()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForCopySelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(copySelectedNetworkElements()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForCutSelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(cutSelectedNetworkElements()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForPasteCopiedNetworkElements(const QPointF &)), (MyInteractor*)interactor(), SLOT(pasteCopiedNetworkElements(const QPointF &)));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForDeleteSelectedNetworkElements()), (MyInteractor*)interactor(), SLOT(deleteSelectedNetworkElements()));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(askForAlignSelectedNetworkElements(const QString&)), (MyInteractor*)interactor(), SLOT(alignSelectedNetworkElements(const QString&)));
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), ((MyGraphicsView*)view())->scene(), SLOT(displayContextMenu(const QPointF&)));

    // status bar
    connect(view(), SIGNAL(mouseLeft()), statusBar(), SLOT(resetMessage()));
    connect(interactor(), SIGNAL(currentFileNameIsUpdated(const QString&)), statusBar(), SLOT(setFileName(const QString&)));
    connect(view(), SIGNAL(scaleChanged(const qreal)), statusBar(), SLOT(setMagnificationFactor(const qreal)));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(mousePositionIsChanged(const QPointF&)), statusBar(), SLOT(setCoordinatesToMousePosition(const QPointF&)));
}

void MyNetworkEditorWidget::arrangeWidgetLayers() {
    modeMenu()->stackUnder(toolBar());
    modeMenu()->stackUnder(statusBar());
    if (featureMenu()) {
        featureMenu()->stackUnder(toolBar());
        featureMenu()->stackUnder(statusBar());
        view()->stackUnder(featureMenu());
    }
    view()->stackUnder(toolBar());
    view()->stackUnder(statusBar());
    view()->stackUnder(modeMenu());
}

QObject* MyNetworkEditorWidget::interactor() {
    return _interactor;
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

QWidget* MyNetworkEditorWidget::secondaryView() {
    return _secondaryView;
}

QWidget* MyNetworkEditorWidget::statusBar() {
    return _statusBar;
}

QWidget* MyNetworkEditorWidget::featureMenu() {
    return _featureMenu;
}

const qreal& MyNetworkEditorWidget::layoutMenuRow() {
    return _layoutMenuRow;
}

void MyNetworkEditorWidget::displayFeatureMenu() {
    if (featureMenu())
        displayFeatureMenu(new MyNullFeatureMenu(((MyInteractor *) interactor())->iconsDirectory().path()));
}

void MyNetworkEditorWidget::displayFeatureMenu(QWidget* featureMenu) {
    removeFeatureMenu();
    featureMenu->setFixedHeight(height() - 2 * toolBar()->height() - 2 * statusBar()->height());
    ((QGridLayout*)layout())->addWidget(featureMenu, layoutMenuRow(), 2, Qt::AlignTop | Qt::AlignRight);
    arrangeWidgetLayers();
    _featureMenu = featureMenu;
    connect(_featureMenu, SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
}

void MyNetworkEditorWidget::removeFeatureMenu() {
    if (featureMenu()) {
        layout()->removeWidget(featureMenu());
        featureMenu()->deleteLater();
        _featureMenu = NULL;
    }
}

void MyNetworkEditorWidget::setReadyToLaunch() {
    ((MyInteractor*)interactor())->setNewNetworkCanvas();
}

void MyNetworkEditorWidget::readSettings() {
    QSettings settings("MyCompany", "NetworkEditorGUI");
    settings.beginGroup("NetworkEditorWidget");

    // window size
    const auto geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
        setGeometry(200, 200, 1050, 700);
    else
        restoreGeometry(geometry);

    // working directory
    const auto workingDirectory = settings.value("working directory", QByteArray()).toByteArray();
    if (workingDirectory.isEmpty())
        ((MyInteractor*)interactor())->askForSettingWorkingDirectoryPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Documents");
    else
        ((MyInteractor*)interactor())->askForSettingWorkingDirectoryPath(workingDirectory);

    settings.endGroup();
}

void MyNetworkEditorWidget::writeSettings() {
    QSettings settings("MyCompany", "NetworkEditorGUI");
    settings.beginGroup("NetworkEditorWidget");

    // window size
    if (dynamic_cast<QWidget*>(parent()))
        settings.setValue("geometry", ((QWidget*)parent())->saveGeometry());
    else
        settings.setValue("geometry", saveGeometry());

    // working directory
    settings.setValue("working directory", ((MyInteractor*)interactor())->askForWorkingDirectoryPath());

    settings.endGroup();
}

void MyNetworkEditorWidget::closeEvent(QCloseEvent *event) {
    writeSettings();
    QWidget::closeEvent(event);
}