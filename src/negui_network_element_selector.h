#ifndef __NEGUI_NETWORK_ELEMENT_SELECTOR_H
#define __NEGUI_NETWORK_ELEMENT_SELECTOR_H

#include "negui_network_element_base.h"

#include <QGraphicsItem>

class MyNetworkElementSelector : public QObject, public MySceneModeElementBase {
    Q_OBJECT

public:

    MyNetworkElementSelector();

    void enableNormalMode() override;

    void selectNetworkElements(const bool& selected);

    void selectNetworkElementsOfCategory(const QString& category, const bool& selected);

    void selectNetworkElements(QList<MyNetworkElementBase*> networkElements, const bool& selected);

    void setNetworkElementSelected(const QString& networkElementName, const bool& selected);

    void setNetworkElementSelected(MyNetworkElementBase* networkElement, const bool& selected);

    void selectNetworkElement(MyNetworkElementBase* networkElement, const bool& selected);

    void changeElementSelectionsStatus(MyNetworkElementBase* networkElement);

    void selectNodes(const bool& selected);

    void selectNodesOfCategory(const QString& category, const bool& selected);

    void selectEdges(const bool& selected);

    void selectEdgesOfCategory(const QString& category, const bool& selected);

    const bool areAnyOtherElementsSelected(QList<MyNetworkElementBase*>);

    const bool areAnyOtherNodesSelected(QList<MyNetworkElementBase*>);

    const bool areAnyOtherEdgesSelected(QList<MyNetworkElementBase*>);

    QList<MyNetworkElementBase*> getSelectedElements();

    QList<MyNetworkElementBase*> getSelectedNodes();

    QList<MyNetworkElementBase*> getSelectedEdges();

    MyNetworkElementBase* getOneSingleSelectedElement();

    MyNetworkElementBase* getOneSingleSelectedNode();

    MyNetworkElementBase* getOneSingleSelectedEdge();

    void displaySelectionArea(const QPointF& position);

    void createSelectionAreaGraphicsItem(const QPointF& position);

    void selectSelectionAreaCoveredElements();

    void selectSelectionAreaCoveredNodes();

    void selectSelectionAreaCoveredEdges();

    void setCollidingElementsSelected(QList<MyNetworkElementBase*> elements);

    void clearSelectionArea();

signals:

    QList<MyNetworkElementBase*> askForNodes();

    QList<MyNetworkElementBase*> askForEdges();

    QList<MyNetworkElementBase*> askForArrowHeads();

    const bool askForWhetherShiftModifierIsPressed();

    void askForAddGraphicsItem(QGraphicsItem*);

    void askForRemoveGraphicsItem(QGraphicsItem*);

protected:

    QGraphicsItem* _selectionAreaGraphicsItem;
};

#endif
