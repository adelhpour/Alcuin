#include "negui_mainwidget.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_mode_menu.h"
#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"

#include <QGridLayout>

// MyNetworkEditorWidget

MyNetworkEditorWidget::MyNetworkEditorWidget(QWidget *parent) :  QFrame(parent) {
    setObjectName("main_widget");
    setStyleSheet("QFrame {background-color : white}");
    
    setMinimumSize(120, 80);
    resize(1050, 700);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setWidgets();
    setInteractions();
    
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(toolBar(), 0, 0, 1, 3);
    layout->addWidget(modeMenu(), 1, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(view(), 1, 1, 1, 1);
    setLayout(layout);
}

MyNetworkEditorWidget::~MyNetworkEditorWidget() {
    
}

void MyNetworkEditorWidget::setWidgets() {
    _toolBar = new MyToolBar(this);
    _modeMenu = new MyModeMenu(this);
    _view = new MyGraphicsView(this);
    _interactor = new MyInteractor(this);
    _featureMenu = NULL;
    
    ((MyToolBar*)toolBar())->addButtons(((MyInteractor*)interactor())->getToolBarMenuButtons());
    ((MyModeMenu*)modeMenu())->setNormalModeButton(((MyInteractor*)interactor())->getNormalModeButton());
    ((MyModeMenu*)modeMenu())->setSelectModeButton(((MyInteractor*)interactor())->getSelectModeButton());
    ((MyModeMenu*)modeMenu())->setAddModeButtons(((MyInteractor*)interactor())->getAddModeButtons());
    ((MyModeMenu*)modeMenu())->setRemoveModeButton(((MyInteractor*)interactor())->getRemoveModeButton());
}

void MyNetworkEditorWidget::setInteractions() {
    /// network editor widget
    // feature menu
    connect((MyInteractor*)interactor(), SIGNAL(askForDisplayFeatureMenu(QWidget*)), this, SLOT(displayFeatureMenu(QWidget*)));

    /// graphics view
    // export screen scene
    connect((MyInteractor*)interactor(), SIGNAL(askForExportFigure(const QString&, QPrinter::OutputFormat)), (MyGraphicsView*)view(), SLOT(exportFigure(const QString&, QPrinter::OutputFormat)));
    
    // set tool tip
    connect((MyInteractor*)interactor(), SIGNAL(askForSetToolTip(const QString&)), (MyGraphicsView*)view(), SLOT(setToolTip(const QString&)));
    
    // reset scale
    connect((MyInteractor*)interactor(), SIGNAL(askForResetScale()), (MyGraphicsView*)view(), SLOT(resetScale()));

    // enter key pressed
    connect((MyGraphicsView*)view(), SIGNAL(enterKeyIsPressed()), (MyInteractor*)interactor(), SIGNAL(enterKeyIsPressed()));

    /// graphics scene
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
    
    // add node
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(addNewNode(const QPointF&)));

    // display the element selection rectangle
    //connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    //connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mousePressedLeftButtonIsMoved(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    //connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsReleased()), (MyInteractor*)interactor(), SLOT(setSelectionAreaCoveredNodesSelected()));
    //connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsReleased()), (MyInteractor*)interactor(), SLOT(clearSelectionArea()));

    // change mode
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseRightButtonIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::escapeKeyIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);

    // remove menu
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(removeFeatureMenu()));
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

void MyNetworkEditorWidget::displayFeatureMenu(QWidget* featureMenu) {
    removeFeatureMenu();
    ((QGridLayout*)layout())->addWidget(featureMenu, 1, 2, 1, 1, Qt::AlignTop);
    _featureMenu = featureMenu;
}

void MyNetworkEditorWidget::removeFeatureMenu() {
    if (_featureMenu) {
        layout()->removeWidget(_featureMenu);
        _featureMenu->deleteLater();
        _featureMenu = NULL;
    }
}
