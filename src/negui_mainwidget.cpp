#include "negui_mainwidget.h"
#include "negui_interactor.h"
#include "negui_toolbar.h"
#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"

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
    layout->addWidget(toolBar(), 0, 0);
    layout->addWidget(view(), 2, 0);
    setLayout(layout);
}

MyNetworkEditorWidget::~MyNetworkEditorWidget() {
    
}

void MyNetworkEditorWidget::setWidgets() {
    _toolBar = new MyToolBar(this);
    _view = new MyGraphicsView(this);
    _interactor = new MyInteractor(this);
    
    ((MyToolBar*)toolBar())->addButtons(((MyInteractor*)interactor())->getMenuButtons());
}

void MyNetworkEditorWidget::setInteractions() {
    /// graphics view
    // export screen scene
    connect((MyInteractor*)interactor(), SIGNAL(askForExportFigure(const QString&, QPrinter::OutputFormat)), (MyGraphicsView*)view(), SLOT(exportFigure(const QString&, QPrinter::OutputFormat)));
    
    // set tool tip
    connect((MyInteractor*)interactor(), SIGNAL(askForSetToolTip(const QString&)), (MyGraphicsView*)view(), SLOT(setToolTip(const QString&)));
    
    // reset scale
    connect((MyInteractor*)interactor(), SIGNAL(askForResetScale()), (MyGraphicsView*)view(), SLOT(resetScale()));
    
    // enter key pressed
    connect((MyGraphicsView*)view(), SIGNAL(enterKeyIsPressed()), (MyInteractor*)interactor(), SIGNAL(enterKeyIsPressed()));
    
    /// graphcis scene
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
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mousePressedLeftButtonIsMoved(const QPointF&)), (MyInteractor*)interactor(), SLOT(displaySelectionArea(const QPointF&)));
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), SIGNAL(mouseLeftButtonIsReleased()), (MyInteractor*)interactor(), SLOT(clearSelectionArea()));

    // change mode
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::mouseRightButtonIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
    connect(((MyGraphicsScene*)((MyGraphicsView*)view())->scene()), &MyGraphicsScene::escapeKeyIsPressed, (MyInteractor*)interactor(), &MyInteractor::enableNormalMode);
}

QObject* MyNetworkEditorWidget::interactor() {
    return _interactor;
}

QWidget* MyNetworkEditorWidget::toolBar() {
    return _toolBar;
}

QWidget* MyNetworkEditorWidget::view() {
    return _view;
}
