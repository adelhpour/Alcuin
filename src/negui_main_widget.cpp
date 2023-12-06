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
    _canDisplayFeatureMenu = false;

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

    ((MyToolBar*)toolBar())->addButtons(((MyInteractor*)interactor())->getToolbarMenuButtons() + ((MyGraphicsView*)view())->getToolbarMenuButtons(((MyInteractor*)interactor())->iconsDirectoryPath()));
    ((MyModeMenu*)modeMenu())->addButtons(((MyInteractor*)interactor())->getModeMenuButtons());
}

void MyNetworkEditorWidget::setInteractions() {
    /// main widget
    // menubar
    connect((MyInteractor*)interactor(), SIGNAL(canUndoChanged(const bool&)), this, SIGNAL(canUndoChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(canRedoChanged(const bool&)), this, SIGNAL(canRedoChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCuttableStatusChanged(const bool&)), this, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCopyableStatusChanged(const bool&)), this, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(pasteElementsStatusChanged(const bool&)), this, SIGNAL(pasteElementsStatusChanged(const bool&)));

    /// feature menu
    // display feature menu
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayNullFeatureMenu()), this, SLOT(displayNullFeatureMenu()));
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
    connect((MyInteractor*)interactor(), &MyInteractor::askForWhetherFeatureMenuCanBeDisplayed, this, [this] () { return _canDisplayFeatureMenu; } );
    connect((MyInteractor*)interactor(), &MyInteractor::askForEnableFeatureMenuDisplay, this, [this] () { _canDisplayFeatureMenu = true; } );
    connect((MyInteractor*)interactor(), &MyInteractor::askForCurrentlyBeingDisplayedFeatureMenu, this, [this] () { return featureMenu(); });

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

    /// graphics scene
    // change stage command
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForCreateChangeStageCommand, this, [this] () { ((MyInteractor*)interactor())->createChangeStageCommand(); });

    // set scene mode
    connect((MyInteractor*)interactor(), &MyInteractor::modeIsSet, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] (const QString& mode) { ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->setSceneMode(mode); });
    
    // add graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(addGraphicsItem(QGraphicsItem*)));
    
    // remove graphics item
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(removeGraphicsItem(QGraphicsItem*)));

    // network background color
    connect((MyInteractor*)interactor(), SIGNAL(askForNetworkBackgroundColor()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(backgroundColor()));
    connect((MyInteractor*)interactor(), SIGNAL(askForSetNetworkBackgroundColor(const QString&)), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(setBackgroundColor(const QString&)));

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
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseLeftButtonIsPressed, this, [this] (const QPointF& position) { ((MyInteractor*)interactor())->addNode(position); });

    // display the element selection rectangle
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseLeftButtonIsPressed, this, [this] (const QPointF& position) { ((MyInteractor*)interactor())->displaySelectionArea(position); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mousePressedLeftButtonIsMoved, this, [this] (const QPointF& position) { ((MyInteractor*)interactor())->displaySelectionArea(position); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseLeftButtonIsReleased, this, [this] () { ((MyInteractor*)interactor())->clearSelectionArea(); });

    // change mode
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::escapeKeyIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForEnableNormalMode, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);

    // context menu
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForWhetherSelectedElementsAreCopyable, this, [this] () { return ((MyInteractor*)interactor())->areSelectedElementsCopyable(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForWhetherSelectedElementsAreCuttable, this, [this] () { return ((MyInteractor*)interactor())->areSelectedElementsCuttable(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForWhetherAnyElementsAreCopied, this, [this] () { return ((MyInteractor*)interactor())->areAnyElementsCopied(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForWhetherAnyElementsAreSelected, this, [this] () { return ((MyInteractor*)interactor())->areAnyElementsSelected(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForWhetherAnyElementsAreAlignable, this, [this] () { return ((MyInteractor*)interactor())->areSelectedElementsAlignable(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForCopySelectedNetworkElements, this, [this] () { ((MyInteractor*)interactor())->copySelectedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForCutSelectedNetworkElements, this, [this] () { ((MyInteractor*)interactor())->cutSelectedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForPasteCopiedNetworkElements, this, [this] (const QPointF & position) { ((MyInteractor*)interactor())->pasteCopiedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForDeleteSelectedNetworkElements, this, [this] () { ((MyInteractor*)interactor())->deleteSelectedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForAlignSelectedNetworkElements, this, [this] (const QString& type)  { ((MyInteractor*)interactor())->alignSelectedNetworkElements(type); });
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

void MyNetworkEditorWidget::displayNullFeatureMenu() {
    if (featureMenu())
        displayFeatureMenu(new MyNullFeatureMenu(((MyInteractor *) interactor())->iconsDirectoryPath()));
}

void MyNetworkEditorWidget::displayFeatureMenu(QWidget* featureMenu) {
    deleteFeatureMenu();
    featureMenu->setFixedHeight(height() - 2 * toolBar()->height() - 2 * statusBar()->height());
    ((QGridLayout*)layout())->addWidget(featureMenu, layoutMenuRow(), 2, Qt::AlignTop | Qt::AlignRight);
    arrangeWidgetLayers();
    _featureMenu = featureMenu;
    connect(_featureMenu, SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
}

void MyNetworkEditorWidget::removeFeatureMenu() {
    deleteFeatureMenu();
    _canDisplayFeatureMenu = false;
}

void MyNetworkEditorWidget::deleteFeatureMenu() {
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

void MyNetworkEditorWidget::enableNormalMode() {
    ((MyInteractor*)interactor())->enableNormalMode();
}

void MyNetworkEditorWidget::enableAddNodeMode(const QString& nodeStyleName) {
    ((MyInteractor*)interactor())->enableAddNodeMode(nodeStyleName);
}

void MyNetworkEditorWidget::enableAddEdgeMode(const QString& edgeStyleName) {
    ((MyInteractor*)interactor())->enableAddEdgeMode(edgeStyleName);
}

void MyNetworkEditorWidget::enableSelectMode(const QString& elementCategory) {
    ((MyInteractor*)interactor())->enableSelectMode(elementCategory);
}

void MyNetworkEditorWidget::enableSelectNodeMode(const QString& nodeCategory) {
    ((MyInteractor*)interactor())->enableSelectNodeMode(nodeCategory);
}

void MyNetworkEditorWidget::enableSelectEdgeMode(const QString& edgeCategory) {
    ((MyInteractor*)interactor())->enableSelectEdgeMode(edgeCategory);
}

void MyNetworkEditorWidget::createNetwork(const QJsonObject &json) {
    ((MyInteractor*)interactor())->createNetwork(json);
}

void MyNetworkEditorWidget::resetNetworkCanvas() {
    ((MyInteractor*)interactor())->resetNetworkCanvas();
}

void MyNetworkEditorWidget::resetNetwork() {
    ((MyInteractor*)interactor())->resetNetwork();
}

void MyNetworkEditorWidget::resetCanvas() {
    ((MyInteractor*)interactor())->resetCanvas();
}

void MyNetworkEditorWidget::setBackground(const QJsonObject &json) {
    ((MyInteractor*)interactor())->setBackground(json);
}

void MyNetworkEditorWidget::setNewNetworkCanvas() {
    ((MyInteractor*)interactor())->setNewNetworkCanvas();
}

QStringList MyNetworkEditorWidget::listOfPluginItemNames(const QString& type) {
    return ((MyInteractor*)interactor())->listOfPluginItemNames(type);
}

QStringList MyNetworkEditorWidget::listOfPluginItemCategories(const QString& type) {
    return ((MyInteractor*)interactor())->listOfPluginItemCategories(type);
}

void MyNetworkEditorWidget::defaultPluginAction(const QString& defaultPluginItemName) {
    ((MyInteractor*)interactor())->defaultPluginAction(defaultPluginItemName);
}

void MyNetworkEditorWidget::saveCurrentNetwork() {
    ((MyInteractor*)interactor())->saveCurrentNetwork();
}

void MyNetworkEditorWidget::writeDataToFile(const QString& exportToolName) {
    ((MyInteractor*)interactor())->writeDataToFile(exportToolName);
}

void MyNetworkEditorWidget::writeFigureToFile(const QString& exportToolName) {
    ((MyInteractor*)interactor())->writeFigureToFile(exportToolName);
}

void MyNetworkEditorWidget::triggerUndoAction() {
    ((MyInteractor*)interactor())->triggerUndoAction();
}

void MyNetworkEditorWidget::triggerRedoAction() {
    ((MyInteractor*)interactor())->triggerRedoAction();
}

void MyNetworkEditorWidget::cutSelectedNetworkElements() {
    ((MyInteractor*)interactor())->cutSelectedNetworkElements();
}

void MyNetworkEditorWidget::copySelectedNetworkElements() {
    ((MyInteractor*)interactor())->copySelectedNetworkElements();
}

void MyNetworkEditorWidget::pasteCopiedNetworkElements() {
    ((MyInteractor*)interactor())->pasteCopiedNetworkElements();
}

void MyNetworkEditorWidget::pasteCopiedNetworkElements(const QPointF& position) {
    ((MyInteractor*)interactor())->pasteCopiedNetworkElements(position);
}

void MyNetworkEditorWidget::resetCopiedNetworkElements() {
    ((MyInteractor*)interactor())->resetCopiedNetworkElements();
}

void MyNetworkEditorWidget::selectAllElements() {
    ((MyInteractor*)interactor())->selectAllElements();
}

void MyNetworkEditorWidget::selectAllElements(const QString& category) {
    ((MyInteractor*)interactor())->selectAllElements(category);
}

void MyNetworkEditorWidget::selectElements(const bool& selected) {
    ((MyInteractor*)interactor())->selectElements(selected);
}

void MyNetworkEditorWidget::selectElementsOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectElementsOfCategory(selected, category);
}

void MyNetworkEditorWidget::selectNodes(const bool& selected) {
    ((MyInteractor*)interactor())->selectNodes(selected);
}

void MyNetworkEditorWidget::selectNodesOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectNodesOfCategory(selected, category);
}

void MyNetworkEditorWidget::selectEdges(const bool& selected) {
    ((MyInteractor*)interactor())->selectEdges(selected);
}

void MyNetworkEditorWidget::selectEdgesOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectEdgesOfCategory(selected, category);
}

void MyNetworkEditorWidget::setElementSelected(const QString& elementName) {
    ((MyInteractor*)interactor())->setElementSelected(elementName);
}

const bool MyNetworkEditorWidget::areSelectedElementsCopyable() {
    return ((MyInteractor*)interactor())->areSelectedElementsCopyable();
}

const bool MyNetworkEditorWidget::areSelectedElementsCuttable() {
    return ((MyInteractor*)interactor())->areSelectedElementsCuttable();
}

const bool MyNetworkEditorWidget::areSelectedElementsAlignable() {
    return ((MyInteractor*)interactor())->areSelectedElementsAlignable();
}

const bool MyNetworkEditorWidget::areAnyElementsCopied() {
    return ((MyInteractor*)interactor())->areAnyElementsCopied();
}

const bool MyNetworkEditorWidget::areAnyElementsSelected() {
    return ((MyInteractor*)interactor())->areAnyElementsSelected();
}

void MyNetworkEditorWidget::deleteSelectedNetworkElements() {
    ((MyInteractor*)interactor())->deleteSelectedNetworkElements();
}

void MyNetworkEditorWidget::alignSelectedNetworkElements(const QString& alignType) {
    ((MyInteractor*)interactor())->alignSelectedNetworkElements(alignType);
}

void MyNetworkEditorWidget::updateFeatureMenu() {
    ((MyInteractor*)interactor())->updateFeatureMenu();
}

void MyNetworkEditorWidget::displaySelectionArea(const QPointF& position) {
    ((MyInteractor*)interactor())->displaySelectionArea(position);
}

void MyNetworkEditorWidget::clearSelectionArea() {
    ((MyInteractor*)interactor())->clearSelectionArea();
}

void MyNetworkEditorWidget::addNodes(const QJsonObject &json) {
    ((MyInteractor*)interactor())->addNodes(json);
}

void MyNetworkEditorWidget::addNode(const QJsonObject& json) {
    ((MyInteractor*)interactor())->addNode(json);
}

void MyNetworkEditorWidget::addNode(const QPointF& position) {
    ((MyInteractor*)interactor())->addNode(position);
}

void MyNetworkEditorWidget::clearNodesInfo() {
    ((MyInteractor*)interactor())->clearNodesInfo();
}

void MyNetworkEditorWidget::addEdges(const QJsonObject &json) {
    ((MyInteractor*)interactor())->addEdges(json);
}

void MyNetworkEditorWidget::addEdge(const QJsonObject& json) {
    ((MyInteractor*)interactor())->addEdge(json);
}

void MyNetworkEditorWidget::clearEdgesInfo() {
    ((MyInteractor*)interactor())->clearEdgesInfo();
}

QJsonObject MyNetworkEditorWidget::exportNetworkInfo() {
    return ((MyInteractor*)interactor())->exportNetworkInfo();
}

void MyNetworkEditorWidget::addDefaultNetworkElementStyles() {
    ((MyInteractor*)interactor())->addDefaultNetworkElementStyles();
}

void MyNetworkEditorWidget::createChangeStageCommand() {
    ((MyInteractor*)interactor())->createChangeStageCommand();
}

const QString MyNetworkEditorWidget::applicationDirectoryPath() {
    return ((MyInteractor*)interactor())->applicationDirectoryPath();
}

const QString MyNetworkEditorWidget::iconsDirectoryPath() {
    return ((MyInteractor*)interactor())->iconsDirectoryPath();
}

void MyNetworkEditorWidget::zoomIn() {
    ((MyGraphicsView*)view())->zoomIn();
}

void MyNetworkEditorWidget::zoomOut() {
    ((MyGraphicsView*)view())->zoomOut();
}

const QString MyNetworkEditorWidget::getName() {
    return Name_Definition;
}

const QString MyNetworkEditorWidget::getVersionNumber() {
    return Version_Definition;
}
