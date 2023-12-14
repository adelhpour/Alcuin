#ifndef __NEGUI_MAINWIDGET_H
#define __NEGUI_MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class MyNetworkEditorWidget : public QFrame {
    Q_OBJECT
    
public:
    
    explicit MyNetworkEditorWidget(QWidget *parent = nullptr);
    ~MyNetworkEditorWidget();

    QObject* interactor();
    QWidget* toolBar();
    QWidget* modeMenu();
    QWidget* view();
    QWidget* secondaryView();
    QWidget* statusBar();
    QWidget* featureMenu();
    void readSettings();
    void writeSettings();

signals:

    void canUndoChanged(const bool&);
    void canRedoChanged(const bool&);
    void elementsCuttableStatusChanged(const bool&);
    void elementsCopyableStatusChanged(const bool&);
    void pasteElementsStatusChanged(const bool&);

public slots:

    // interactor
    void enableNormalMode();
    void enableAddNodeMode(const QString& nodeStyleName);
    void enableAddEdgeMode(const QString& edgeStyleName);
    void enableSelectMode(const QString& elementCategory = "");
    void enableSelectNodeMode(const QString& nodeCategory = "");
    void enableSelectEdgeMode(const QString& edgeCategory = "");
    void createNetwork(const QJsonObject &json);
    void resetNetworkCanvas();
    void resetNetwork();
    void resetCanvas();
    void setBackground(const QJsonObject &json);
    void setNewNetworkCanvas();
    QStringList listOfPluginItemNames(const QString& type);
    QStringList listOfPluginItemCategories(const QString& type);
    void callPluginFunctions(const QString& pluginName);
    void saveCurrentNetwork();
    void triggerUndoAction();
    void triggerRedoAction();
    void cutSelectedNetworkElements();
    void copySelectedNetworkElements();
    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const QPointF& position);
    void resetCopiedNetworkElements();
    void selectAllElements();
    void selectAllElements(const QString&);
    void selectElements(const bool& selected);
    void selectElementsOfCategory(const bool& selected, const QString& category);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setElementSelected(const QString& elementName);
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areSelectedElementsAlignable();
    const bool areAnyElementsCopied();
    const bool areAnyElementsSelected();
    void deleteSelectedNetworkElements();
    void alignSelectedNetworkElements(const QString& alignType);
    void updateFeatureMenu();
    void displaySelectionArea(const QPointF& position);
    void clearSelectionArea();
    void addNodes(const QJsonObject &json);
    void addNode(const QJsonObject& json);
    void addNode(const QPointF& position);
    void clearNodesInfo();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void clearEdgesInfo();
    QJsonObject exportNetworkInfo();
    void addDefaultNetworkElementStyles();
    void createChangeStageCommand();
    const QString applicationDirectoryPath();
    const QString iconsDirectoryPath();

    // graphics view
    void zoomIn();
    void zoomOut();

    // feature menu
    void displayNullFeatureMenu();
    void removeFeatureMenu();

    // app
    const QString getName();
    const QString getVersionNumber();

private slots:

    void displayFeatureMenu(QWidget* featureMenu);
    void deleteFeatureMenu();

protected:

    void setWidgets();
    void setInteractions();
    void arrangeWidgetLayers();
    const qreal& layoutMenuRow();
    void setReadyToLaunch();
    void closeEvent(QCloseEvent *event) override;

    QObject* _interactor;
    QWidget* _toolBar;
    QWidget* _view;
    QWidget* _secondaryView;
    QWidget* _modeMenu;
    QWidget* _featureMenu;
    QWidget* _statusBar;
    qreal _layoutMenuRow;
    bool _canDisplayFeatureMenu;
};

#endif
