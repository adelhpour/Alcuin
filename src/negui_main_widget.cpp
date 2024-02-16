#include "negui_main_widget.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_mode_menu.h"
#include "negui_graphics_view.h"
#include "negui_secondary_graphics_view.h"
#include "negui_graphics_scene.h"
#include "negui_status_bar.h"
#include "negui_null_feature_menu.h"
#include "negui_call_api_function.h"

#include <QGridLayout>
#include <QSettings>
#include <QStandardPaths>

// MyMainWidget

MyMainWidget::MyMainWidget(QWidget *parent) :  QFrame(parent) {
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

MyMainWidget::~MyMainWidget() {
    close();
}

void MyMainWidget::setWidgets() {
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

void MyMainWidget::setInteractions() {
    /// main widget
    // api
    connect((MyInteractor*)interactor(), &MyInteractor::askForCallAPIFunction, this, [this] (const QString& functionName, const QJsonValue& inputs) {
        return callAPIFunction(this, functionName, inputs);
    });
    // menubar
    connect((MyInteractor*)interactor(), SIGNAL(canUndoChanged(const bool&)), this, SIGNAL(canUndoChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(canRedoChanged(const bool&)), this, SIGNAL(canRedoChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCuttableStatusChanged(const bool&)), this, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(elementsCopyableStatusChanged(const bool&)), this, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect((MyInteractor*)interactor(), SIGNAL(pasteElementsStatusChanged(const bool&)), this, SIGNAL(pasteElementsStatusChanged(const bool&)));

    /// feature menu
    // display feature menu
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayNullFeatureMenu()), this, SLOT(displayNullFeatureMenu()));
    connect((MyInteractor*)interactor(), &MyInteractor::askForDisplayFeatureMenu, this, [this] (QWidget* featureMenu) { displayFeatureMenu(featureMenu); });
    connect((MyInteractor*)interactor(), SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherFeatureMenuCanBeDisplayed()), this, SLOT(canDisplayFeatureMenu()) );
    connect((MyInteractor*)interactor(), &MyInteractor::askForEnableFeatureMenuDisplay, this, [this] () { enableDisplayFeatureMenu(true); } );
    connect((MyInteractor*)interactor(), &MyInteractor::askForCurrentlyBeingDisplayedFeatureMenu, this, [this] () { return featureMenu(); });

    /// mode menu
    // set mode
    connect((MyInteractor*)interactor(), SIGNAL(modeIsSet(const QString&)), modeMenu(), SLOT(setMode(const QString&)));
    connect((MyInteractor*)interactor(), SIGNAL(addElementModeIsEnabled(const QString &)), modeMenu(), SLOT(activateAddElementButton(const QString&)));

    /// graphics view
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

    // cursor position
    connect((MyInteractor*)interactor(), SIGNAL(askForCursorPosition()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(cursorPosition()));

    // activated shift modifier
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherShiftModifierIsPressed()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(isShiftModifierPressed()));

    // activated control modifier
    connect((MyInteractor*)interactor(), SIGNAL(askForWhetherControlModifierIsPressed()), ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SLOT(isControlModifierPressed()));

    // items bounding rect
    connect((MyInteractor*)interactor(), &MyInteractor::askForItemsBoundingRect, ((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), [this] () { return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->itemsBoundingRect(); });

    // select all
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForSelectAll, (MyInteractor*)interactor(), [this] () { ((MyInteractor*)this->interactor())->selectNetworkElements(true); });
    
    // add node
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseLeftButtonIsPressed, this, [this] (const QPointF& position) { addNode(position.x(), position.y()); });

    // display the element selection rectangle
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseLeftButtonIsPressed, this, [this] (const QPointF& position) { displaySelectionArea(position.x(), position.y()); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mousePressedLeftButtonIsMoved, this, [this] (const QPointF& position) { (displaySelectionArea(position.x(), position.y())); });
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
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForPasteCopiedNetworkElements, this, [this] () { ((MyInteractor*)interactor())->pasteCopiedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForDeleteSelectedNetworkElements, this, [this] () { ((MyInteractor*)interactor())->deleteSelectedNetworkElements(); });
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::askForAlignSelectedNetworkElements, this, [this] (const QString& type)  { ((MyInteractor*)interactor())->alignSelectedNetworkElements(type); });
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplaySceneContextMenu(const qreal&, const qreal&)), ((MyGraphicsView*)view())->scene(), SLOT(displayContextMenu(const qreal&, const qreal&)));

    // status bar
    connect(view(), SIGNAL(mouseLeft()), statusBar(), SLOT(resetMessage()));
    connect(interactor(), SIGNAL(currentFileNameIsUpdated(const QString&)), statusBar(), SLOT(setFileName(const QString&)));
    connect(view(), SIGNAL(scaleChanged(const qreal)), statusBar(), SLOT(setMagnificationFactor(const qreal)));
    connect(((MyGraphicsView*)view())->scene(), SIGNAL(mousePositionIsChanged(const QPointF&)), statusBar(), SLOT(setCoordinatesToMousePosition(const QPointF&)));
}

void MyMainWidget::arrangeWidgetLayers() {
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

QObject* MyMainWidget::interactor() {
    return _interactor;
}

QWidget* MyMainWidget::toolBar() {
    return _toolBar;
}

QWidget* MyMainWidget::modeMenu() {
    return _modeMenu;
}

QWidget* MyMainWidget::view() {
    return _view;
}

QWidget* MyMainWidget::secondaryView() {
    return _secondaryView;
}

QWidget* MyMainWidget::statusBar() {
    return _statusBar;
}

QWidget* MyMainWidget::featureMenu() {
    return _featureMenu;
}

const qreal& MyMainWidget::layoutMenuRow() {
    return _layoutMenuRow;
}

void MyMainWidget::displayNullFeatureMenu() {
    if (featureMenu())
        displayFeatureMenu(new MyNullFeatureMenu(((MyInteractor *) interactor())->iconsDirectoryPath()));
}

void MyMainWidget::displayFeatureMenu(QWidget* featureMenu) {
    deleteFeatureMenu();
    featureMenu->setFixedHeight(height() - 2 * toolBar()->height() - 2 * statusBar()->height());
    ((QGridLayout*)layout())->addWidget(featureMenu, layoutMenuRow(), 2, Qt::AlignTop | Qt::AlignRight);
    arrangeWidgetLayers();
    _featureMenu = featureMenu;
    connect(_featureMenu, SIGNAL(askForRemoveFeatureMenu()), this, SLOT(removeFeatureMenu()));
}

void MyMainWidget::removeFeatureMenu() {
    deleteFeatureMenu();
    _canDisplayFeatureMenu = false;
}

void MyMainWidget::deleteFeatureMenu() {
    if (featureMenu()) {
        layout()->removeWidget(featureMenu());
        featureMenu()->deleteLater();
        _featureMenu = NULL;
    }
}

void MyMainWidget::setReadyToLaunch() {
    ((MyInteractor*)interactor())->setNewNetworkCanvas();
}

void MyMainWidget::readSettings() {
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
        ((MyInteractor*)interactor())->setWorkingDirectoryPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Documents");
    else
        ((MyInteractor*)interactor())->setWorkingDirectoryPath(workingDirectory);

    settings.endGroup();
}

void MyMainWidget::writeSettings() {
    QSettings settings("MyCompany", "NetworkEditorGUI");
    settings.beginGroup("NetworkEditorWidget");

    // window size
    if (dynamic_cast<QWidget*>(parent()))
        settings.setValue("geometry", ((QWidget*)parent())->saveGeometry());
    else
        settings.setValue("geometry", saveGeometry());

    // working directory
    settings.setValue("working directory", ((MyInteractor*)interactor())->workingDirectoryPath());

    settings.endGroup();
}

void MyMainWidget::closeEvent(QCloseEvent *event) {
    writeSettings();
    QWidget::closeEvent(event);
}

void MyMainWidget::enableNormalMode() {
    ((MyInteractor*)interactor())->enableNormalMode();
}

void MyMainWidget::enableAddNodeMode(const QString& nodeStyleName) {
    ((MyInteractor*)interactor())->enableAddNodeMode(nodeStyleName);
}

void MyMainWidget::enableAddEdgeMode(const QString& edgeStyleName) {
    ((MyInteractor*)interactor())->enableAddEdgeMode(edgeStyleName);
}

void MyMainWidget::createNetwork(const QJsonObject &json) {
    ((MyInteractor*)interactor())->createNetwork(json);
}

void MyMainWidget::resetNetworkCanvas() {
    ((MyInteractor*)interactor())->resetNetworkCanvas();
}

void MyMainWidget::resetNetwork() {
    ((MyInteractor*)interactor())->resetNetwork();
}

void MyMainWidget::resetCanvas() {
    ((MyInteractor*)interactor())->resetCanvas();
}

void MyMainWidget::clearScene() {
    ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->clearScene();
}

void MyMainWidget::resetScale() {
    ((MyGraphicsView*)view())->resetScale();
}

void MyMainWidget::setToolTip(const QString& toolTip) {
    ((MyGraphicsView*)view())->setToolTip(toolTip);
}

void MyMainWidget::setNetworkBackgroundColor(const QString& backgroundColor) {
    ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->setBackgroundColor(backgroundColor);
}

const QString MyMainWidget::getNetworkBackgroundColor() {
    return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->backgroundColor();
}

void MyMainWidget::setNewNetworkCanvas() {
    ((MyInteractor*)interactor())->setNewNetworkCanvas();
}

QJsonArray MyMainWidget::listOfPluginItemNames(const QString& type) {
    return ((MyInteractor*)interactor())->listOfPluginItemNames(type);
}

QJsonArray MyMainWidget::listOfPluginItemCategories(const QString& type) {
    return ((MyInteractor*)interactor())->listOfPluginItemCategories(type);
}

void MyMainWidget::callPluginFunctions(const QString& pluginName) {
    ((MyInteractor*)interactor())->callPluginFunctions(pluginName);
}

void MyMainWidget::saveCurrentNetwork() {
    ((MyInteractor*)interactor())->saveCurrentNetwork();
}

void MyMainWidget::triggerUndoAction() {
    ((MyInteractor*)interactor())->triggerUndoAction();
}

void MyMainWidget::triggerRedoAction() {
    ((MyInteractor*)interactor())->triggerRedoAction();
}

void MyMainWidget::cutSelectedNetworkElements() {
    ((MyInteractor*)interactor())->cutSelectedNetworkElements();
}

void MyMainWidget::copySelectedNetworkElements() {
    ((MyInteractor*)interactor())->copySelectedNetworkElements();
}

void MyMainWidget::pasteCopiedNetworkElements() {
    ((MyInteractor*)interactor())->pasteCopiedNetworkElements();
}

void MyMainWidget::resetCopiedNetworkElements() {
    ((MyInteractor*)interactor())->resetCopiedNetworkElements();
}

void MyMainWidget::selectNetworkElements(const bool& selected) {
    ((MyInteractor*)interactor())->selectNetworkElements(selected);
}

void MyMainWidget::selectNetworkElementsOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectNetworkElementsOfCategory(selected, category);
}

void MyMainWidget::selectNodes(const bool& selected) {
    ((MyInteractor*)interactor())->selectNodes(selected);
}

void MyMainWidget::selectNodesOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectNodesOfCategory(selected, category);
}

void MyMainWidget::selectEdges(const bool& selected) {
    ((MyInteractor*)interactor())->selectEdges(selected);
}

void MyMainWidget::selectEdgesOfCategory(const bool& selected, const QString& category) {
    ((MyInteractor*)interactor())->selectEdgesOfCategory(selected, category);
}

void MyMainWidget::setNetworkElementSelected(const QString& elementName, const bool& selected) {
    ((MyInteractor*)interactor())->setNetworkElementSelected(elementName, selected);
}

const bool MyMainWidget::areSelectedElementsCopyable() {
    return ((MyInteractor*)interactor())->areSelectedElementsCopyable();
}

const bool MyMainWidget::areSelectedElementsCuttable() {
    return ((MyInteractor*)interactor())->areSelectedElementsCuttable();
}

const bool MyMainWidget::areSelectedElementsAlignable() {
    return ((MyInteractor*)interactor())->areSelectedElementsAlignable();
}

const bool MyMainWidget::areAnyElementsCopied() {
    return ((MyInteractor*)interactor())->areAnyElementsCopied();
}

const bool MyMainWidget::areAnyElementsSelected() {
    return ((MyInteractor*)interactor())->areAnyElementsSelected();
}

void MyMainWidget::deleteSelectedNetworkElements() {
    ((MyInteractor*)interactor())->deleteSelectedNetworkElements();
}

void MyMainWidget::alignSelectedNetworkElements(const QString& alignType) {
    ((MyInteractor*)interactor())->alignSelectedNetworkElements(alignType);
}

void MyMainWidget::updateFeatureMenu() {
    ((MyInteractor*)interactor())->updateFeatureMenu();
}

void MyMainWidget::displaySelectionArea(const qreal& x, const qreal& y) {
    ((MyInteractor*)interactor())->displaySelectionArea(x, y);
}

void MyMainWidget::clearSelectionArea() {
    ((MyInteractor*)interactor())->clearSelectionArea();
}

void MyMainWidget::addNode(const qreal& x, const qreal& y) {
    ((MyInteractor*)interactor())->addNode(x, y);
}

void MyMainWidget::deleteNode(const QString& nodeName) {
    ((MyInteractor*)interactor())->deleteNode(nodeName);
}

QJsonArray MyMainWidget::listOfNodes() {
    return ((MyInteractor*)interactor())->listOfNodes();
}

const qreal MyMainWidget::numberOfNodes() {
    return ((MyInteractor*)interactor())->numberOfNodes();
}

void MyMainWidget::addEdge(QList<QString> sourceNodes, QList<QString> targetNodes) {
    ((MyInteractor*)interactor())->addEdge(sourceNodes, targetNodes);
}

void MyMainWidget::deleteEdge(const QString& edgeName) {
    ((MyInteractor*)interactor())->deleteEdge(edgeName);
}

QJsonArray MyMainWidget::listOfEdges() {
    return ((MyInteractor*)interactor())->listOfEdges();
}

const qreal MyMainWidget::numberOfEdges() {
    return ((MyInteractor*)interactor())->numberOfEdges();
}

void MyMainWidget::adjustConnectedEdgesOfNodes() {
    ((MyInteractor*)interactor())->adjustConnectedEdgesOfNodes();
}

void MyMainWidget::adjustExtentsOfNodes() {
    ((MyInteractor*)interactor())->adjustExtentsOfNodes();
}

QJsonObject MyMainWidget::exportNetworkInfo() {
    return ((MyInteractor*)interactor())->exportNetworkInfo();
}

void MyMainWidget::createChangeStageCommand() {
    ((MyInteractor*)interactor())->createChangeStageCommand();
}

const QString MyMainWidget::applicationDirectoryPath() {
    return ((MyInteractor*)interactor())->applicationDirectoryPath();
}

const QString MyMainWidget::iconsDirectoryPath() {
    return ((MyInteractor*)interactor())->iconsDirectoryPath();
}

void MyMainWidget::zoomIn() {
    ((MyGraphicsView*)view())->zoomIn();
}

void MyMainWidget::zoomOut() {
    ((MyGraphicsView*)view())->zoomOut();
}

const QString MyMainWidget::getName() {
    return Name_Definition;
}

const QString MyMainWidget::getVersionNumber() {
    return Version_Definition;
}

const bool MyMainWidget::isControlModifierPressed() {
    return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->isControlModifierPressed();
}

const bool MyMainWidget::isShiftModifierPressed() {
    return ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->isShiftModifierPressed();
}

void MyMainWidget::displaySceneContextMenu(const qreal& x, const qreal& y) {
    ((MyGraphicsScene*)((MyGraphicsView*)view())->scene())->displayContextMenu(x, y);
}

void MyMainWidget::enableDisplayFeatureMenu(const bool& enable) {
    _canDisplayFeatureMenu = enable;
}

const bool MyMainWidget::canDisplayFeatureMenu() {
    return _canDisplayFeatureMenu;
}

void MyMainWidget::saveFigure(const QString& fileName) {
    ((MyGraphicsView*)view())->saveFigure(fileName);
}

const QString MyMainWidget::getOpenFileName(const QString& fileExtension) {
    return ((MyInteractor*)interactor())->getOpenFileName(fileExtension);
}

const QString MyMainWidget::getSaveFileName(const QString& defaultFileExtension) {
    return ((MyInteractor*)interactor())->getSaveFileName(defaultFileExtension);
}

const QString MyMainWidget::getSaveAsFileName(const QString& fileExtension) {
    return ((MyInteractor*)interactor())->getSaveAsFileName(fileExtension);
}

const QString MyMainWidget::getSaveFigureFileName(const QString& fileExtension) {
    return ((MyInteractor*)interactor())->getSaveFigureFileName(fileExtension);
}

const QJsonValue MyMainWidget::takeParameterFromUser(const QString& name, const QJsonValue defaultValue) {
    return ((MyInteractor*)interactor())->takeParameterFromUser(name, defaultValue);
}

void MyMainWidget::showParameterValue(const QString& name,const QJsonValue value) {
    return ((MyInteractor*)interactor())->showParameterValue(name, value);
}

const QString& MyMainWidget::workingDirectoryPath() {
    return ((MyInteractor*)interactor())->workingDirectoryPath();
}

void MyMainWidget::setWorkingDirectoryPath(const QString& workingDirectoryPath) {
    ((MyInteractor*)interactor())->setWorkingDirectoryPath(workingDirectoryPath);
}

void MyMainWidget::saveCurrentNetworkWithUserPermission() {
    ((MyInteractor*)interactor())->saveCurrentNetworkWithUserPermission();
}
