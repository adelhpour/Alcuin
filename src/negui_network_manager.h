#ifndef __NEGUI_NETWORK_MANAGER_H
#define __NEGUI_NETWORK_MANAGER_H

#include "negui_network_element_base.h"
#include "negui_network_element_style_base.h"

class MyNetworkManager : public QObject, public MySceneModeElementBase {
    Q_OBJECT

public:

    MyNetworkManager();

    void enableNormalMode() override;

    void enableAddNodeMode(MyPluginItemBase* style);

    void enableAddEdgeMode(MyPluginItemBase* style);

    void enableSelectMode() override;

    void enableSelectNodeMode() override;

    void enableSelectEdgeMode() override;

    QList<MyNetworkElementBase*>& nodes();

    QList<MyNetworkElementBase*>& edges();

    void clearNodesInfo();

    void clearEdgesInfo();

    MyNetworkElementStyleBase* nodeStyle();

    void setNodeStyle(MyNetworkElementStyleBase* style);

    MyNetworkElementStyleBase* copiedNodeStyle();

    void setCopiedNodeStyle(MyNetworkElementStyleBase* style);

    bool isSetCopiedNodeStyle();

    void pasteCopiedNodeStyle(MyNetworkElementBase* element);

    MyNetworkElementStyleBase* edgeStyle();

    void setEdgeStyle(MyNetworkElementStyleBase* style);

    MyNetworkElementStyleBase* copiedEdgeStyle();

    void setCopiedEdgeStyle(MyNetworkElementStyleBase* style);

    const bool isSetCopiedEdgeStyle();

    void pasteCopiedEdgeStyle(MyNetworkElementBase* element);

    void setNetworkElementSelector();

    void createNetwork(const QJsonObject& json);

    void resetNetworkCanvas();

    void resetNetwork();

    void resetCanvas();

    void setBackgroundColor(const QJsonObject &json);

    void addNodes(const QJsonObject &json);

    void addNode(const QJsonObject &json);

    void addNode(const QPointF& position);

    void addNode(MyNetworkElementBase* n);

    void deleteNode(const QString& nodeName);

    void deleteNode(MyNetworkElementBase* node);

    void removeNode(MyNetworkElementBase* n);

    void updateNodeParents();

    void addEdges(const QJsonObject &json);

    void addEdge(const QJsonObject &json);

    void addEdge(MyNetworkElementBase* e);

    void addNewEdge(MyNetworkElementBase* element);

    void removeEdge(MyNetworkElementBase* e);

    void deleteEdge(MyNetworkElementBase* edge);

    void changeElementSelectionsStatus(MyNetworkElementBase* element);

    const bool areSelectedElementsCopyable();

    const bool areSelectedElementsCuttable();

    const bool areSelectedElementsAlignable();

    const bool areAnyElementsCopied();

    const bool areAnyElementsSelected();

    void copySelectedNetworkElements();

    void cutSelectedNetworkElements();

    void pasteCopiedNetworkElements();

    void pasteCopiedNetworkElements(const QPointF& position);

    const QList<MyNetworkElementBase*> getSelectedElements();

    const QList<MyNetworkElementBase*> getSelectedNodes();

    const QList<MyNetworkElementBase*> getSelectedEdges();

    void setCutNode(MyNetworkElementBase* node);

    void setCopiedNode(MyNetworkElementBase* node);

    QList<MyNetworkElementBase*> copiedNetworkElements();

    void resetCopiedNetworkElements();

    bool isElementNameAlreadyUsed(const QString& elementName);

    void deleteNewEdgeBuilder();

    bool edgeExists(MyNetworkElementBase* n1, MyNetworkElementBase* n2);

    QJsonObject getNetworkElementsAndColorInfo();

    QJsonObject exportNetworkInfo();

    void deleteSelectedNetworkElements();

    void alignSelectedNetworkElements(const QString& alignType);

    void selectNetworkElements(const bool& selected);

    void selectNetworkElementsOfCategory(const bool& selected, const QString& category);

    void selectNodes(const bool& selected);

    void selectNodesOfCategory(const bool& selected, const QString& category);

    void selectEdges(const bool& selected);

    void selectEdgesOfCategory(const bool& selected, const QString& category);

    void setNetworkElementSelected(const QString& networkElementName, const bool& selected);

    MyNetworkElementBase* getOneSingleSelectedElement();

    MyNetworkElementBase* getOneSingleSelectedNode();

    MyNetworkElementBase* getOneSingleSelectedEdge();

    void displaySelectionArea(const QPointF& position);

    void createSelectionAreaGraphicsItem(const QPointF& position);

    void selectSelectionAreaCoveredNodes();

    void selectSelectionAreaCoveredEdges();

    void clearSelectionArea();

    void updateFeatureMenu();

signals:

    void askForClearScene();

    void askForCreateChangeStageCommand();

    void askForAddGraphicsItem(QGraphicsItem*);

    void askForRemoveGraphicsItem(QGraphicsItem*);

    void networkElementsSelectedStatusIsChanged();

    void elementsCuttableStatusChanged(const bool&);

    void elementsCopyableStatusChanged(const bool&);

    const bool askForWhetherShiftModifierIsPressed();

    const bool askForWhetherControlModifierIsPressed();

    void askForAdjustExtentsOfNodes();

    void askForAdjustConnectedEdgesOfNodes();

    void askForDisplaySceneContextMenu(const QPointF&);

    QString askForIconsDirectoryPath();

    void pasteElementsStatusChanged(const bool&);

    QList<QGraphicsItem *> askForItemsAtPosition(const QPointF& position);

    void askForEnableFeatureMenuDisplay();

    QWidget* askForCurrentlyBeingDisplayedFeatureMenu();

    bool askForWhetherFeatureMenuCanBeDisplayed();

    void askForDisplayNullFeatureMenu();

    void askForDisplayFeatureMenu(QWidget*);

    void askForSetToolTip(const QString&);

    void askForSetNetworkBackgroundColor(const QString&);

    const QString askForNetworkBackgroundColor();

    QRectF askForItemsBoundingRect();

    void askForResetScale();

    void askForEnableNormalMode();

    void askForClearUndoStack();

public slots:

    void moveSelectedNetworkElements(MyNetworkElementBase* movedNode, const QPointF& movedDistance);

protected:

    QList<MyNetworkElementBase*> _nodes;
    QList<MyNetworkElementBase*> _edges;
    MyNetworkElementStyleBase* _nodeStyle;
    MyNetworkElementStyleBase* _copiedNodeStyle;
    MyNetworkElementStyleBase* _edgeStyle;
    MyNetworkElementStyleBase* _copiedEdgeStyle;
    QList<MyNetworkElementBase*> _copiedNetworkElements;

    QObject* _newEdgeBuilder;
    QObject* _networkElementSelector;
};

#endif
