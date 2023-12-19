#ifndef __NEGUI_MAINWIDGET_H
#define __NEGUI_MAINWIDGET_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class MyMainWidget : public QFrame {
    Q_OBJECT
    
public:
    
    explicit MyMainWidget(QWidget *parent = nullptr);
    ~MyMainWidget();

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
    void setNewNetworkCanvas();
    void resetNetworkCanvas();
    void resetNetwork();
    void resetCanvas();
    void clearScene();
    void resetScale();
    void setToolTip(const QString& toolTip);
    void createNetwork(const QJsonObject &json);
    void setNetworkBackgroundColor(const QString& backgroundColor);
    const QString getNetworkBackgroundColor();
    void saveCurrentNetwork();
    void saveCurrentNetworkWithUserPermission();
    void saveFigure(const QString& fileName);
    QJsonObject exportNetworkInfo();
    void triggerUndoAction();
    void triggerRedoAction();
    void createChangeStageCommand();
    void cutSelectedNetworkElements();
    void copySelectedNetworkElements();
    void pasteCopiedNetworkElements();
    void pasteCopiedNetworkElements(const qreal& x, const qreal& y);
    void resetCopiedNetworkElements();
    const bool areSelectedElementsCopyable();
    const bool areSelectedElementsCuttable();
    const bool areAnyElementsCopied();
    void selectNetworkElements(const bool& selected);
    void selectNetworkElementsOfCategory(const bool& selected, const QString&);
    void selectNodes(const bool& selected);
    void selectNodesOfCategory(const bool& selected, const QString& category);
    void selectEdges(const bool& selected);
    void selectEdgesOfCategory(const bool& selected, const QString& category);
    void setNetworkElementSelected(const QString& networkElementName, const bool& selected);
    const bool areAnyElementsSelected();
    void displaySelectionArea(const qreal& x, const qreal& y);
    void clearSelectionArea();
    const bool areSelectedElementsAlignable();
    void alignSelectedNetworkElements(const QString& alignType);
    void deleteSelectedNetworkElements();
    void updateFeatureMenu();
    void addNode(const qreal& x, const qreal& y);
    void adjustConnectedEdgesOfNodes();
    void adjustExtentsOfNodes();
    const QString applicationDirectoryPath();
    const QString iconsDirectoryPath();
    const QString& workingDirectoryPath();
    void setWorkingDirectoryPath(const QString& workingDirectoryPath);
    const QString getOpenFileName(const QString& fileExtension);
    const QString getSaveFileName(const QString& defaultFileExtension);
    const QString getSaveAsFileName(const QString& fileExtension);
    const QString getSaveFigureFileName(const QString& fileExtension);
    const QJsonValue takeParameterFromUser(const QString& name, const QJsonValue defaultValue);

    const bool isControlModifierPressed();
    const bool isShiftModifierPressed();
    void displaySceneContextMenu(const qreal& x, const qreal& y);
    void enableDisplayFeatureMenu(const bool& enable);
    const bool canDisplayFeatureMenu();
    void zoomIn();
    void zoomOut();
    void displayNullFeatureMenu();
    void removeFeatureMenu();
    const QString getName();
    const QString getVersionNumber();
    QJsonArray listOfPluginItemNames(const QString& type);
    QJsonArray listOfPluginItemCategories(const QString& type);


    void callPluginFunctions(const QString& pluginName);
    void clearNodesInfo();
    void addEdges(const QJsonObject &json);
    void addEdge(const QJsonObject& json);
    void clearEdgesInfo();
    void addDefaultNetworkElementStyles();

protected:

    void setWidgets();
    void setInteractions();
    void arrangeWidgetLayers();
    const qreal& layoutMenuRow();
    void setReadyToLaunch();
    void closeEvent(QCloseEvent *event) override;
    void deleteFeatureMenu();
    void displayFeatureMenu(QWidget* featureMenu);

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
