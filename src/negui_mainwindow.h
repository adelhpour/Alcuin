#ifndef __NEGUI_MAINWINDOW_H
#define __NEGUI_MAINWINDOW_H

#if defined MAKE_NEGUI_LIB
    #define NEGUI_LIB_EXPORT Q_DECL_EXPORT
#else
    #define NEGUI_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QApplication>
#include <QMainWindow>
#include <QPluginLoader>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QWidgetAction>
#include <QFileDialog>
#include <QPrinter>
#include <QMenu>
#include <QMenuBar>
#include <QWheelEvent>
#include <QTimeLine>
#include <QScrollBar>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QUndoStack>
#include "negui_interfaces.h"


class MyGraphicsView;
class MyGraphicsScene;
class MyNode;
class MyEdge;
class MyUndoStack;

typedef enum {
    NORMAL_MODE,
    ADD_NODE_MODE,
    ADD_EDGE_MODE,
    REMOVE_MODE,
} SceneMode;

class NEGUI_LIB_EXPORT MyMainWindow : public QMainWindow {
    Q_OBJECT
public:
    /// Constructor
    explicit MyMainWindow(QWidget *parent = nullptr);
    
    /// Destructor
    ~MyMainWindow();
    
    /// Functinos
    // set view
    void setView(MyGraphicsView* view);
    
    // get view
    MyGraphicsView* view();
    
    // show whether the view is set
    const bool isSetView() const { return _isSetView; }
    
    // set scene extends
    void setSceneRect(QRectF rect);

private slots:
    
    void screenshot();
    void exit();
    
private:
    bool loadPlugins();
    
    void createMenus();
    
    MyGraphicsView* _view;
    MyUndoStack* _undoStack;
    bool _isSetView;
    NodeInterface* _nodeInterface;
};

class NEGUI_LIB_EXPORT MyGraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    
    MyGraphicsView(QWidget* parent = nullptr);
    
    // return the scene of this view
    MyGraphicsScene* scene();
    
    // get the current scale of the view
    const qreal currentScale() const;
    
    // set the maximum allowed scale of the view
    void setMaxScale(const qreal& maxScale);
    
    // get the maximum allowed scale of the view
    const qreal maxScale() const;
    
    // set the minimum allowed scale of the view
    void setMinScale(const qreal& minScale);
    
    // get the minimum allowed scale of the view
    const qreal minScale() const;
    
private slots:
    void scalingTime(qreal x);
    void animFinished();
    
protected:
    int _numScheduledScalings;
    qreal _maxScale;
    qreal _minScale;
    bool _pan;
    int _panStartX;
    int _panStartY;
    
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

class NEGUI_LIB_EXPORT MyGraphicsScene : public QGraphicsScene {
    Q_OBJECT
    
public:
    
    MyGraphicsScene(QWidget* parent = nullptr);
    
    // set the mode of the scene
    void setMode(SceneMode mode);
    
    // get the mode of the scene
    SceneMode mode();
    
    // get nodes
    QList<MyNode*>& nodes();
    
    // get edges
    QList<MyEdge*>& edges();
    
    // enable the normal mode of the view
    void enableNormalMode();
    
    // enable the add node mode of the view
    void enableAddNodeMode();
    
    // enable the add edge mode of the view
    void enableAddEdgeMode();
    
    // enable the remove mode of the view
    void enableRemoveMode();
    
    // add a node to the scene where the mouse is clicked
    void addNode(const QPointF& position);
    
    // add a node to the scene
    void addNode(MyNode* node);
    
    // remove a node from the scene
    void removeNode(MyNode* node);
    
    // add an edge to the scene between the nodes selected
    void addEdge(const QPointF& position);
    
    // add an edge to the scene
    void addEdge(MyEdge* edge);
    
    // remove an edge from the scene
    void removeEdge(MyEdge* edge);
    
    // remove the items at this position from the scene
    void removeItems(const QPointF& position);
    
    // set selected edge start node
    void setSelectedEdgeStartNode(MyNode* n);
    
    // unset selected edge start node
    void unSetSelectedEdgeStartNode();
    
    // get selected edge start node
    MyNode* selectedEdgeStartNode();
    
    // show whether the selected edge start node is set
    const bool isSetSelectedEdgeStartNode() const { return _isSetSelectedEdgeStartNode; }
    
    // check whether an edge with the n1 and n2 node endings already exists
    bool edgeExists(MyNode* n1, MyNode* n2);
    
signals:
    void commandCreated(QUndoCommand* command);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
    SceneMode _mode;
    MyNode* _selectedEdgeStartNode;
    bool _isSetSelectedEdgeStartNode;
    QList<MyNode*> _nodes;
    QList<MyEdge*> _edges;
};

class NEGUI_LIB_EXPORT MyNode : public QGraphicsEllipseItem {
    
public:
    
    MyNode(const qreal& x, const qreal& y, QGraphicsItem *parent = nullptr);
    
    // add to edges
    void addEdge(MyEdge* e);
    
    // remove from edges
    void removeEdge(MyEdge* e);
    
    // get edges
    QList<MyEdge*>& edges();
    
    // set the center point of the node
    void setCenter(const QPointF& center);
    
    // get the center point of the node
    const QPointF center() const;
    
    // get the radius of the node
    const qreal radius() const;
    
    // determine whether the node is visible on the scene
    bool setActive(const bool& active);
    
    // return true if the node is visible on the scene
    const bool isActive() const { return _isActive; }
    
    // determine whether the node is selected as the start node of an edge
    void setSelected(const bool& selected);
    
    // determine whether the node is movable
    void enableMovable(const bool& movable);
    
protected:
    qreal _radius;
    QPointF _initialCenter;
    QPointF _center;
    QPen _pen;
    QBrush _brush;
    QBrush _selectedBrush;
    QList<MyEdge*> _edges;
    bool _isActive;
    
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

class NEGUI_LIB_EXPORT MyEdge : public QGraphicsLineItem {
    
public:
    
    MyEdge(MyNode* startNode, MyNode* endNode, QGraphicsItem *parent = nullptr);
    
    // get the start node of the edge
    MyNode* startNode();
    
    // get the end node of the edge
    MyNode* endNode();
    
    // determine whether the edge is visible on the scene
    bool setActive(const bool& active);
    
    // return true if the edge is visible on the scene
    const bool isActive() const { return _isActive; }
    
    // determine whether the edge is connected to its start and end nodes
    bool connectToNodes(const bool& connect);
    
    // return true if the edge is connected to its start and end nodes
    const bool isConnectedToNodes() const { return _isConnectedToNodes; }
    
    // update the ending points of the edge using the start and end node centers
    void updatePoints();

protected:
    QPen _pen;
    MyNode* _startNode;
    MyNode* _endNode;
    bool _isActive;
    bool _isConnectedToNodes;
};

class NEGUI_LIB_EXPORT MyUndoStack : public QUndoStack {
    Q_OBJECT
    
public:
    
    MyUndoStack(QObject *parent = nullptr);
    
private slots:
    
    void addCommand(QUndoCommand* command);
};

class NEGUI_LIB_EXPORT MyAddNodeCommand : public QUndoCommand {
public:
    
    MyAddNodeCommand(MyGraphicsScene *scene, MyNode* node, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MyNode* _node;
    MyGraphicsScene* _scene;
};

class NEGUI_LIB_EXPORT MyRemoveNodeCommand : public QUndoCommand {
public:
    
    MyRemoveNodeCommand(MyGraphicsScene *scene, MyNode* node, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MyNode* _node;
    MyGraphicsScene* _scene;
};

class NEGUI_LIB_EXPORT MyAddEdgeCommand : public QUndoCommand {
public:
    
    MyAddEdgeCommand(MyGraphicsScene *scene, MyEdge* edge, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MyEdge* _edge;
    MyGraphicsScene* _scene;
};

class NEGUI_LIB_EXPORT MyRemoveEdgeCommand : public QUndoCommand {
public:
    
    MyRemoveEdgeCommand(MyGraphicsScene *scene, MyEdge* edge, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    MyEdge* _edge;
    MyGraphicsScene* _scene;
};

#endif
