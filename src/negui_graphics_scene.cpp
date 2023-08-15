#include "negui_graphics_scene.h"
#include "negui_context_menu.h"

// MyGraphicsScene

MyGraphicsScene::MyGraphicsScene(QWidget* parent) : QGraphicsScene(parent) {
    setSceneRect(30.0, 20.0, 840.0, 560.0);
    _isLeftButtonPressed = false;
    _isShiftModifierPressed = false;
}

void MyGraphicsScene::setSceneRect(qreal x, qreal y, qreal width, qreal height) {
    QGraphicsScene::setSceneRect(x, y, width, height);
}

void MyGraphicsScene::addGraphicsItem(QGraphicsItem* item) {
    addItem(item);
}

void MyGraphicsScene::removeGraphicsItem(QGraphicsItem* item) {
    removeItem(item);
}

void MyGraphicsScene::clearScene() {
    clear();
}

QList<QGraphicsItem *> MyGraphicsScene::itemsAtPosition(const QPointF& position) {
    return QGraphicsScene::items(position);
}

const bool MyGraphicsScene::isShiftModifierPressed() {
    return _isShiftModifierPressed;
}

QMenu* MyGraphicsScene::createContextMenu() {
    QMenu* contextMenu = new MyGraphicsSceneContextMenu();
    connectContextMenu(contextMenu);
    ((MyContextMenuBase*)contextMenu)->initializeActionsStatus();
    return contextMenu;
}

void MyGraphicsScene::connectContextMenu(QMenu* contextMenu) {
    connect(contextMenu, SIGNAL(askForCopySelectedNetworkElements()), this, SIGNAL(askForCopySelectedNetworkElements()));
    connect(contextMenu, SIGNAL(askForCutSelectedNetworkElements()), this, SIGNAL(askForCutSelectedNetworkElements()));
    connect((MyGraphicsSceneContextMenu*)contextMenu, &MyGraphicsSceneContextMenu::askForPasteCopiedNetworkElements, this, [this] () { emit askForPasteCopiedNetworkElements(_cursorPosition); });
    connect(contextMenu, SIGNAL(askForDeleteSelectedNetworkElements()), this, SIGNAL(askForDeleteSelectedNetworkElements()));
    connect(contextMenu, SIGNAL(askForWhetherSelectedElementsAreCopyable()), this, SIGNAL(askForWhetherSelectedElementsAreCopyable()));
    connect(contextMenu, SIGNAL(askForWhetherSelectedElementsAreCuttable()), this, SIGNAL(askForWhetherSelectedElementsAreCuttable()));
    connect(contextMenu, SIGNAL(askForWhetherAnyElementsAreCopied()), this, SIGNAL(askForWhetherAnyElementsAreCopied()));
    connect(contextMenu, SIGNAL(askForWhetherAnyElementsAreSelected()), this, SIGNAL(askForWhetherAnyElementsAreSelected()));
}

void MyGraphicsScene::displayContextMenu(const QPointF& position) {
    if (getSceneMode() == NORMAL_MODE) {
        QMenu* contextMenu = createContextMenu();
        contextMenu->exec(QPoint(position.x(), position.y()));
    }
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (!event->isAccepted()) {
        if (event->button() == Qt::LeftButton) {
            qint32 numberOfItems = items().size();
            emit mouseLeftButtonIsPressed(event->scenePos());
            
            // if a node is added
            if (items().size() != numberOfItems)
                event->accept();
            _isLeftButtonPressed = true;
        }
    }
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (_isLeftButtonPressed)
        emit mousePressedLeftButtonIsMoved(event->scenePos());
    emit mousePositionIsChanged(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
    _cursorPosition = event->scenePos();
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        _isLeftButtonPressed = false;
        emit mouseLeftButtonIsReleased();
    }
    else if (event->button() == Qt::RightButton) {
        if (getSceneMode() != NORMAL_MODE)
            emit askForEnableNormalMode();
    }
}

void MyGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    emit mouseLeftButtonIsDoubleClicked();
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event) {
    QGraphicsScene::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Escape) {
            emit escapeKeyIsPressed();
            event->accept();
        }
        else if (event->modifiers() == Qt::ShiftModifier) {
            _isShiftModifierPressed = true;
            event->accept();
        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A)
            emit askForSelectAll();
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C)
            emit askForCopySelectedNetworkElements();
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_X)
            emit askForCutSelectedNetworkElements();
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V)
            emit askForPasteCopiedNetworkElements(_cursorPosition);
        else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
            emit askForDeleteSelectedNetworkElements();
    }
}

void MyGraphicsScene::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsScene::keyReleaseEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            _isShiftModifierPressed = false;
            event->accept();
        }
    }
}
