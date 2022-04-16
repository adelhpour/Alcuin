#include "negui_mainwindow.h"
#include <iostream>

MyMainWindow::MyMainWindow(QWidget *parent) :  QMainWindow(parent) {
    _isSetView = false;
    setObjectName("main_window");
    
    //ui->setupUi(this);
    
    QWidget* topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    QWidget* bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    setWindowTitle(tr("Network Editor"));
    setMinimumSize(120, 80);
    resize(1050, 700);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setView(new MyGraphicsView(this));
    _undoStack = new MyUndoStack(this);
    if (view()->scene())
        connect(view()->scene(), SIGNAL(commandCreated(QUndoCommand*)), _undoStack, SLOT(addCommand(QUndoCommand*)));
    loadPlugins();
    createMenus();

    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller, 0, 0);
    layout->addWidget(view(), 1, 0);
    layout->addWidget(bottomFiller, 2, 0);
    
    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MyMainWindow::~MyMainWindow() {
    
}

bool MyMainWindow::loadPlugins() {
    QDir pluginsDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS")
        pluginsDir.cdUp();
#endif
    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = pluginLoader.instance();
        if (plugin) {
            _nodeInterface = qobject_cast<NodeInterface *>(plugin);
            if (_nodeInterface) {
                _nodeInterface->loadScript(pluginsDir.path());
                return true;
            }
            
            pluginLoader.unload();
        }
    }

    return false;
}

void MyMainWindow::createMenus() {
    QMenu* _menuBar = NULL;
    QMenu* _subMenu = NULL;
    QAction* _action = NULL;
    
    /// file menu
    _menuBar = menuBar()->addMenu(tr("&File"));
    
    // exit action
    _action = _menuBar->addAction(tr("&Exit"));
    _action->setShortcuts(QKeySequence::Quit);
    _action->setStatusTip(tr("Exit the application"));
    connect(_action, &QAction::triggered, this, &MyMainWindow::exit);
    
    // exit action
    _action = _menuBar->addAction(tr("Export as &PDF"));
    _action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    _action->setStatusTip(tr("Export a drawing of the model as a PDF file"));
    if (isSetView())
        connect(_action, &QAction::triggered, this, &MyMainWindow::screenshot);
    
    /// edit menu
    _menuBar = menuBar()->addMenu(tr("&Edit"));
    _subMenu = _menuBar->addMenu(tr("&Add Item"));
    
    // add node action
    _action = _subMenu->addAction(tr("&Node"));
    _action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    _action->setStatusTip(tr("Add a node to the network"));
    if (isSetView() && view()->scene())
        connect(_action, &QAction::triggered, view()->scene(), &MyGraphicsScene::enableAddNodeMode);
    
    // add node action
    _action = _subMenu->addAction(tr("&Edge"));
    _action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    _action->setStatusTip(tr("Add an edge between two nodes"));
    if (isSetView() && view()->scene())
        connect(_action, &QAction::triggered, view()->scene(), &MyGraphicsScene::enableAddEdgeMode);
    
    // remove action
    _action = _menuBar->addAction(tr("&Remove"));
    _action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    _action->setStatusTip(tr("Remove an item from the network"));
    if (isSetView() && view()->scene())
        connect(_action, &QAction::triggered, view()->scene(), &MyGraphicsScene::enableRemoveMode);
    
    _menuBar->addSeparator();
    
    // undo action
    _action = _undoStack->createUndoAction(this, tr("&Undo"));
    _action->setShortcuts(QKeySequence::Undo);
    _menuBar->addAction(_action);
    
    // redo action
    _action = _undoStack->createRedoAction(this, tr("&Redo"));
    _action->setShortcuts(QKeySequence::Redo);
    _menuBar->addAction(_action);
}

void MyMainWindow::setView(MyGraphicsView* view) {
    _view = view;
    _isSetView = true;
}
   
MyGraphicsView* MyMainWindow::view() {
    return _view;
}

void MyMainWindow::screenshot() {
    // get the image file name by showing a file dialog to the user and asking them to enter their desired palce to save and the name of the file
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF File", "drawing", "(*.pdf)");
    
    // if file name is set successfully
    if (!fileName.isEmpty()) {
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setPageSize(QPageSize(QSize(view()->scene()->sceneRect().width(), view()->scene()->sceneRect().height()), QPageSize::Point));
        printer.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        
        QPainter painter(&printer);
        view()->scene()->render(&painter);
    }
}

void MyMainWindow::exit() {
    close();
}

// MyGraphicsView

MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    _maxScale = 3.0;
    _minScale = 1.0 / 3.0;
    _numScheduledScalings = 0;
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setGeometry(75, 50, 900, 600);
    setScene(new MyGraphicsScene());
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    show();
};

MyGraphicsScene* MyGraphicsView::scene() {
    return ((MyGraphicsScene*)(QGraphicsView::scene()));
}

void MyGraphicsView::wheelEvent (QWheelEvent * event) {
    int numDegrees = event->delta();
    int numSteps = numDegrees;
    _numScheduledScalings += numSteps;
    
    if (_numScheduledScalings * numSteps < 0)
        _numScheduledScalings = numSteps;
    
    QTimeLine* anim = new QTimeLine(20, this);
    anim->setUpdateInterval(1);
    
    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();
}

const qreal MyGraphicsView::currentScale() const {
    return transform().m11();
}

void MyGraphicsView::setMaxScale(const qreal& maxScale) {
    _maxScale = maxScale;
}

const qreal MyGraphicsView::maxScale() const {
    return _maxScale;
}

void MyGraphicsView::setMinScale(const qreal& minScale) {
    _minScale = minScale;
}

const qreal MyGraphicsView::minScale() const {
    return _minScale;
}

void MyGraphicsView::scalingTime(qreal x) {
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 10000.0;
    if ((factor  > 1.00000 && (currentScale() < _maxScale)) || (factor  < 1.00000 && (currentScale() > _minScale)))
        scale(factor, factor);
}

void MyGraphicsView::animFinished() {
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->~QObject();
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    if (_pan) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

// MyGraphicsScene

MyGraphicsScene::MyGraphicsScene(QWidget* parent) : QGraphicsScene(parent) {
    _selectedEdgeStartNode = NULL;
    _isSetSelectedEdgeStartNode = false;
    setSceneRect(30.0, 20.0, 840.0, 560.0);
    setMode(NORMAL_MODE);
}

void MyGraphicsScene::setMode(SceneMode mode) {
    _mode = mode;
    unSetSelectedEdgeStartNode();
}

SceneMode MyGraphicsScene::mode() {
    return _mode;
}

QList<MyNode*>& MyGraphicsScene::nodes() {
    return _nodes;
}

QList<MyEdge*>& MyGraphicsScene::edges() {
    return _edges;
}

void MyGraphicsScene::enableNormalMode() {
    setMode(NORMAL_MODE);
    for (MyNode *node : qAsConst(_nodes)) {
        node->setCursor(Qt::PointingHandCursor);
        node->enableMovable(true);
    }
    for (MyEdge *edge : qAsConst(_edges))
        edge->setCursor(Qt::ArrowCursor);
}

void MyGraphicsScene::enableAddNodeMode() {
    setMode(ADD_NODE_MODE);
    for (MyNode *node : qAsConst(_nodes)) {
        node->setCursor(Qt::ArrowCursor);
        node->enableMovable(false);
    }
    for (MyEdge *edge : qAsConst(_edges))
        edge->setCursor(Qt::ArrowCursor);
}

void MyGraphicsScene::enableAddEdgeMode() {
    setMode(ADD_EDGE_MODE);
    for (MyNode *node : qAsConst(_nodes)) {
        node->setCursor(Qt::PointingHandCursor);
        node->enableMovable(false);
    }
    for (MyEdge *edge : qAsConst(_edges))
        edge->setCursor(Qt::ArrowCursor);
}

void MyGraphicsScene::enableRemoveMode() {
    setMode(REMOVE_MODE);
    for (MyNode *node : qAsConst(_nodes)) {
        node->setCursor(Qt::PointingHandCursor);
        node->enableMovable(false);
    }
    for (MyEdge *edge : qAsConst(_edges))
        edge->setCursor(Qt::PointingHandCursor);
}

void MyGraphicsScene::addNode(const QPointF& position) {
    if (mode() == ADD_NODE_MODE) {
        MyNode* _node = new MyNode(position.x(), position.y());
        _node->setZValue(1);
        QUndoCommand *addNodeCommand = new MyAddNodeCommand(this, _node);
        emit commandCreated(addNodeCommand);
    }
}

void MyGraphicsScene::addNode(MyNode* n) {
    if (!n->isActive()) {
        addItem(n);
        _nodes.push_back(n);
        n->setActive(true);
    }
}

void MyGraphicsScene::removeNode(MyNode* n) {
    if (n->isActive()) {
        removeItem(n);
        _nodes.removeOne(n);
        n->setActive(false);
    }
}

void MyGraphicsScene::addEdge(const QPointF& position) {
    if (mode() == ADD_EDGE_MODE) {
        QList<QGraphicsItem *> itemsList = items(position);
        if (itemsList.size() == 1 && itemsList.first()->type() == 4) {
            if (!isSetSelectedEdgeStartNode())
                setSelectedEdgeStartNode((MyNode*)itemsList.first());
            else if (selectedEdgeStartNode() != (MyNode*)itemsList.first() && !edgeExists(selectedEdgeStartNode(), (MyNode*)itemsList.first())) {
                MyEdge* _edge = new MyEdge(selectedEdgeStartNode(), (MyNode*)itemsList.first());
                _edge->setZValue(0);
                QUndoCommand *addEdgeCommand = new MyAddEdgeCommand(this, _edge);
                emit commandCreated(addEdgeCommand);
                unSetSelectedEdgeStartNode();
            }
        }
        else
            enableNormalMode();
    }
}

void MyGraphicsScene::addEdge(MyEdge* e) {
    if (!edgeExists(e->startNode(), e->endNode()) && e->setActive(true)) {
        addItem(e);
        _edges.push_back(e);
    }
}

void MyGraphicsScene::removeEdge(MyEdge* e) {
    if (e->isActive()) {
        e->setActive(false);
        removeItem(e);
        _edges.removeOne(e);
    }
}

void MyGraphicsScene::removeItems(const QPointF& position) {
    if (mode() == REMOVE_MODE) {
        QList<QGraphicsItem *> itemsList = items(position);
        if (!itemsList.isEmpty()) {
            for (QGraphicsItem *item : qAsConst(itemsList)) {
                QUndoCommand *removeCommand = NULL;
                if(item->type() == 4)
                    removeCommand = new MyRemoveNodeCommand(this, (MyNode*)item);
                else if (item->type() == 6)
                    removeCommand = new MyRemoveEdgeCommand(this, (MyEdge*)item);
                emit commandCreated(removeCommand);
            }
        }
        else
            enableNormalMode();
    }
}

void MyGraphicsScene::setSelectedEdgeStartNode(MyNode* n) {
    if (n) {
        _selectedEdgeStartNode = n;
        _isSetSelectedEdgeStartNode = true;
        _selectedEdgeStartNode->setSelected(true);
    }
}

void MyGraphicsScene::unSetSelectedEdgeStartNode() {
    if (_selectedEdgeStartNode)
        _selectedEdgeStartNode->setSelected(false);
    _selectedEdgeStartNode = NULL;
    _isSetSelectedEdgeStartNode = false;
}

MyNode* MyGraphicsScene::selectedEdgeStartNode() {
    return _selectedEdgeStartNode;
}

bool MyGraphicsScene::edgeExists(MyNode* n1, MyNode* n2) {
    for (MyEdge *edge : qAsConst(_edges)) {
        if ((edge->startNode() == n1 && edge->endNode() == n2) || (edge->startNode() == n2 && edge->endNode() == n1)) {
            unSetSelectedEdgeStartNode();
            return true;
        }
    }
    
    return false;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        switch (_mode) {
            case ADD_NODE_MODE:
                addNode(event->scenePos());
                break;
                
            case ADD_EDGE_MODE:
                addEdge(event->scenePos());
                break;
                
            case REMOVE_MODE:
                removeItems(event->scenePos());
                break;
                
            default:
                break;
        }
        
        event->accept();
        return;
    }
    else if (event->button() == Qt::RightButton) {
        enableNormalMode();
        event->accept();
        return;
    }
    event->ignore();
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
    {
        enableNormalMode();
        event->accept();
        return;
    }
    event->ignore();
}

// MyNode

MyNode::MyNode(const qreal& x, const qreal& y, QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    _isActive = false;
    _radius = 25.0;
    _initialCenter = QPointF(x, y);
    setCenter(_initialCenter);
    
    // pen
    _pen.setWidth(2);
    _pen.setColor(Qt::lightGray);
    setPen(_pen);
    
    // brush
    _selectedBrush.setStyle(Qt::SolidPattern);
    _selectedBrush.setColor(QColor("#4169e1"));
    _brush.setStyle(Qt::SolidPattern);
    _brush.setColor(Qt::darkCyan);
    setBrush(_brush);
    
    // bounding rect
    setRect(x - _radius, y - _radius, 2 * _radius, 2 * _radius);
    
    // make it send position changes
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    
    // set cursor when it is over the node
    setCursor(Qt::PointingHandCursor);
}

void MyNode::addEdge(MyEdge* e) {
    _edges.push_back(e);
}

void MyNode::removeEdge(MyEdge* e) {
    _edges.removeOne(e);
}

QList<MyEdge*>& MyNode::edges() {
    return _edges;
}

void MyNode::setCenter(const QPointF& center) {
    _center = center;
    for (MyEdge *edge : qAsConst(_edges))
        edge->updatePoints();
}

const QPointF MyNode::center() const {
    return _center;
}

const qreal MyNode::radius() const {
    return _radius;
}

bool MyNode::setActive(const bool& active) {
    return _isActive = active;
}

void MyNode::setSelected(const bool& selected) {
    if (selected)
        setBrush(_selectedBrush);
    else
        setBrush(_brush);
    QGraphicsItem::setSelected(selected);
}

void MyNode::enableMovable(const bool& movable) {
    setFlag(QGraphicsItem::ItemIsMovable, movable);
}

QVariant MyNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        setCenter(value.toPointF() + _initialCenter);
    
    return QGraphicsItem::itemChange(change, value);
}

// MyEdge

MyEdge::MyEdge(MyNode* startNode, MyNode* endNode, QGraphicsItem *parent) : QGraphicsLineItem(parent) {
    _isActive = false;
    _isConnectedToNodes = false;
    
    if (startNode && endNode) {
        _startNode = startNode;
        _startNode->addEdge(this);
        _endNode = endNode;
        _endNode->addEdge(this);
    }
    
    // pen
    _pen.setWidth(4);
    _pen.setColor(Qt::lightGray);
    setPen(_pen);
    
    // set cursor when it is over the node
    setCursor(Qt::ArrowCursor);
}

MyNode* MyEdge::startNode() {
    return _startNode;
}

MyNode* MyEdge::endNode() {
    return _endNode;
}

bool MyEdge::setActive(const bool& active) {
    if (active && startNode() && startNode()->isActive() && endNode() && endNode()->isActive()) {
        updatePoints();
        return _isActive = true;
    }
    
    return _isActive = false;
}

bool MyEdge::connectToNodes(const bool& connect) {
    return _isConnectedToNodes = connect;
}

void MyEdge::updatePoints() {
    if (startNode() && endNode()) {
        qreal slope = qAtan2(endNode()->center().y() - startNode()->center().y(), endNode()->center().x() - startNode()->center().x());
        setLine(startNode()->center().x() + startNode()->radius() * qCos(slope), startNode()->center().y() + startNode()->radius() * qSin(slope), endNode()->center().x() - endNode()->radius() * qCos(slope), endNode()->center().y() - endNode()->radius() * qSin(slope));
    }
}

// MyUndoStack

MyUndoStack::MyUndoStack(QObject *parent) : QUndoStack(parent) {
    
}

void MyUndoStack::addCommand(QUndoCommand* command) {
    push(command);
}

// MyAddNodeCommand

MyAddNodeCommand::MyAddNodeCommand(MyGraphicsScene* scene, MyNode* node, QUndoCommand* parent) : QUndoCommand(parent) {
    _scene = scene;
    _node = node;
}

void MyAddNodeCommand::undo() {
    _scene->removeNode(_node);
    _scene->enableNormalMode();
}

void MyAddNodeCommand::redo() {
    _scene->addNode(_node);
}

// MyRemoveNodeCommand

MyRemoveNodeCommand::MyRemoveNodeCommand(MyGraphicsScene* scene, MyNode* node, QUndoCommand* parent) : QUndoCommand(parent) {
    _scene = scene;
    _node = node;
}

void MyRemoveNodeCommand::undo() {
    _scene->addNode(_node);
    for (MyEdge *edge : qAsConst(_node->edges())) {
        if (edge->isConnectedToNodes())
            _scene->addEdge(edge);
    }
    _scene->enableNormalMode();
}

void MyRemoveNodeCommand::redo() {
    _scene->removeNode(_node);
    for (MyEdge *edge : qAsConst(_node->edges()))
        _scene->removeEdge(edge);
}

// MyAddEdgeCommand

MyAddEdgeCommand::MyAddEdgeCommand(MyGraphicsScene* scene, MyEdge* edge, QUndoCommand* parent) : QUndoCommand(parent) {
    _scene = scene;
    _edge = edge;
}

void MyAddEdgeCommand::undo() {
    _edge->connectToNodes(false);
    _scene->removeEdge(_edge);
    _scene->enableNormalMode();
}

void MyAddEdgeCommand::redo() {
    _edge->connectToNodes(true);
    _scene->addEdge(_edge);
}

// MyRemoveEdgeCommand

MyRemoveEdgeCommand::MyRemoveEdgeCommand(MyGraphicsScene* scene, MyEdge* edge, QUndoCommand* parent) : QUndoCommand(parent) {
    _scene = scene;
    _edge = edge;
}

void MyRemoveEdgeCommand::undo() {
    _edge->connectToNodes(true);
    _scene->addEdge(_edge);
    _scene->enableNormalMode();
}

void MyRemoveEdgeCommand::redo() {
    _edge->connectToNodes(false);
    _scene->removeEdge(_edge);
}
