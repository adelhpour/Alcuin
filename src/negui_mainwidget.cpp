#include "negui_mainwidget.h"

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
    _view = new MyGraphicsView(this);
    _toolBar = new MyToolBar(this);
    _interactor = new MyInteractor(this);
    
    toolBar()->addButtons(interactor()->getMenuButtons());
}

void MyNetworkEditorWidget::setInteractions() {
    /// graphics view
    // export screen scene
    connect(interactor(), SIGNAL(askForExportFigure(const QString&, QPrinter::OutputFormat)), view(), SLOT(exportFigure(const QString&, QPrinter::OutputFormat)));
    
    // set tool tip
    connect(interactor(), SIGNAL(askForSetToolTip(const QString&)), view(), SLOT(setToolTip(const QString&)));
    
    // reset scale
    connect(interactor(), SIGNAL(askForClearScene()), view(), SLOT(resetScale()));
    
    // enter key pressed
    connect(view(), SIGNAL(enterKeyIsPressed()), interactor(), SIGNAL(enterKeyIsPressed()));
    
    /// graphcis scene
    // set scene rect
    connect(interactor(), SIGNAL(askForSetSceneRect(qreal, qreal, qreal, qreal)), view()->scene(), SLOT(setSceneRect(qreal, qreal, qreal, qreal)));
    
    // add graphics item
    connect(interactor(), SIGNAL(askForAddGraphicsItem(QGraphicsItem*)), view()->scene(), SLOT(addGraphicsItem(QGraphicsItem*)));
    
    // remove graphics item
    connect(interactor(), SIGNAL(askForRemoveGraphicsItem(QGraphicsItem*)), view()->scene(), SLOT(removeGraphicsItem(QGraphicsItem*)));
    
    // reset scene
    connect(interactor(), SIGNAL(askForClearScene()), view()->scene(), SLOT(clearScene()));
    
    // items at position
    connect(interactor(), SIGNAL(askForItemsAtPosition(const QPointF&)), view()->scene(), SLOT(itemsAtPosition(const QPointF&)));
    
    // add node
    connect(view()->scene(), SIGNAL(mouseLeftButtonIsPressed(const QPointF&)), interactor(), SLOT(addNewNode(const QPointF&)));
    
    // change mode
    connect(view()->scene(), &MyGraphicsScene::mouseRightButtonIsPressed, interactor(), &MyInteractor::enableNormalMode);
    connect(view()->scene(), &MyGraphicsScene::mouseRightButtonIsPressed, interactor(), &MyInteractor::disconnectPressedEnterKeyFromDataExportTools);
    connect(view()->scene(), &MyGraphicsScene::escapeKeyIsPressed, interactor(), &MyInteractor::enableNormalMode);
    connect(view()->scene(), &MyGraphicsScene::escapeKeyIsPressed, interactor(), &MyInteractor::disconnectPressedEnterKeyFromDataExportTools);
}

MyInteractor* MyNetworkEditorWidget::interactor() {
    return _interactor;
}

MyToolBar* MyNetworkEditorWidget::toolBar() {
    return _toolBar;
}

MyGraphicsView* MyNetworkEditorWidget::view() {
    return _view;
}
