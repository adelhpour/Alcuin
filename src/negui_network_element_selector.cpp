#include "negui_network_element_selector.h"
#include "negui_selection_area_graphics_item.h"

// MyNetworkElementSelector

MyNetworkElementSelector::MyNetworkElementSelector() {
    _selectionAreaGraphicsItem = NULL;
}

void MyNetworkElementSelector::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    selectNetworkElements(false);
}

void MyNetworkElementSelector::selectNetworkElements(const bool& selected) {
    selectNodes(selected);
    selectEdges(selected);
}

void MyNetworkElementSelector::selectNetworkElementsOfCategory(const QString& category, const bool& selected) {
    selectNodesOfCategory(category, selected);
    selectEdgesOfCategory(category, selected);
}

void MyNetworkElementSelector::selectNetworkElements(QList<MyNetworkElementBase*> networkElements, const bool& selected) {
    for (MyNetworkElementBase* networkElement : networkElements)
        selectNetworkElement(networkElement, selected);
}

void MyNetworkElementSelector::setNetworkElementSelected(const QString& networkElementName, const bool& selected) {
    setNetworkElementSelected(getNetworkElement(askForNodes() + askForEdges(), networkElementName), selected);
}

void MyNetworkElementSelector::setNetworkElementSelected(MyNetworkElementBase* networkElement, const bool& selected) {
    networkElement->setSelected(selected);
}

void MyNetworkElementSelector::selectNetworkElement(MyNetworkElementBase* networkElement, const bool& selected) {
    if (networkElement->isSelected() != selected)
        networkElement->setSelected(selected);
}

void MyNetworkElementSelector::changeElementSelectionsStatus(MyNetworkElementBase* networkElement) {
    if (getSceneMode() == MyNetworkElementBase::NORMAL_MODE) {
        if (askForWhetherShiftModifierIsPressed()) {
            if (!networkElement->isSelected())
                selectNetworkElement(networkElement, true);
            else
                selectNetworkElement(networkElement, false);
        }
        else {
            QList<MyNetworkElementBase*> networkElements;
            networkElements.push_back(networkElement);
            if (!(networkElement->isSelected() && areAnyOtherElementsSelected(networkElements)))
                selectNetworkElements(false);
            selectNetworkElement(networkElement, true);
        }
    }
}

void MyNetworkElementSelector::selectNodes(const bool& selected) {
    selectNetworkElements(askForNodes(), selected);
}

void MyNetworkElementSelector::selectNodesOfCategory(const QString& category, const bool& selected) {
    selectNetworkElements(getNetworkElementsOfCategory(askForNodes(), category), selected);
}

void MyNetworkElementSelector::selectEdges(const bool& selected) {
    selectNetworkElements(askForEdges(), selected);
}

void MyNetworkElementSelector::selectEdgesOfCategory(const QString& category, const bool& selected) {
    selectNetworkElements(getNetworkElementsOfCategory(askForEdges(), category), selected);
}

const bool MyNetworkElementSelector::areAnyOtherElementsSelected(QList<MyNetworkElementBase*> elements) {
    if (areAnyOtherNodesSelected(elements))
        return true;
    else if (areAnyOtherEdgesSelected(elements))
        return true;

    return false;
}

const bool MyNetworkElementSelector::areAnyOtherNodesSelected(QList<MyNetworkElementBase*> elements) {
    QList<MyNetworkElementBase*> nodes = askForNodes();
    for (MyNetworkElementBase* node : qAsConst(nodes)) {
        if (node->isSelected()) {
            if (!whetherNetworkElementExistsInTheListOfNetworkElements(node, elements))
                return true;
        }
    }

    return false;
}

const bool MyNetworkElementSelector::areAnyOtherEdgesSelected(QList<MyNetworkElementBase*> elements) {
    QList<MyNetworkElementBase*> edges = askForEdges();
    for (MyNetworkElementBase* edge : qAsConst(edges)) {
        if (edge->isSelected()) {
            if (!whetherNetworkElementExistsInTheListOfNetworkElements(edge, elements))
                return true;
        }
    }

    return false;
}

QList<MyNetworkElementBase*> MyNetworkElementSelector::getSelectedElements() {
    return getSelectedNodes() + getSelectedEdges();
}

QList<MyNetworkElementBase*> MyNetworkElementSelector::getSelectedNodes() {
    QList<MyNetworkElementBase*> nodes = askForNodes();
    QList<MyNetworkElementBase*> selectedNodes;
    for (MyNetworkElementBase* node : nodes) {
        if (node->isSelected())
            selectedNodes.push_back(node);
    }

    return selectedNodes;
}

QList<MyNetworkElementBase*> MyNetworkElementSelector::getSelectedEdges() {
    QList<MyNetworkElementBase*> edges = askForEdges();
    QList<MyNetworkElementBase*> selectedEdges;
    for (MyNetworkElementBase* edge : edges) {
        if (edge->isSelected())
            selectedEdges.push_back(edge);
    }

    return selectedEdges;
}

MyNetworkElementBase* MyNetworkElementSelector::getOneSingleSelectedElement() {
    QList<MyNetworkElementBase*> selectedElements = getSelectedElements();
    if (selectedElements.size() == 1)
        return selectedElements.at(0);

    return NULL;
}

MyNetworkElementBase* MyNetworkElementSelector::getOneSingleSelectedNode() {
    QList<MyNetworkElementBase*> selectedNodes = getSelectedNodes();
    if (selectedNodes.size() == 1)
        return selectedNodes.at(0);

    return NULL;
}

MyNetworkElementBase* MyNetworkElementSelector::getOneSingleSelectedEdge() {
    QList<MyNetworkElementBase*> selectedEdges = getSelectedEdges();
    if (selectedEdges.size() == 1)
        return selectedEdges.at(0);

    return NULL;
}

void MyNetworkElementSelector::displaySelectionArea(const QPointF& position) {
    if (getSceneMode() == NORMAL_MODE)
        createSelectionAreaGraphicsItem(position);
}

void MyNetworkElementSelector::createSelectionAreaGraphicsItem(const QPointF& position) {
    if (!_selectionAreaGraphicsItem) {
        _selectionAreaGraphicsItem = new MySelectionAreaGraphicsItem(position);
        emit askForAddGraphicsItem(_selectionAreaGraphicsItem);
    }
    ((MySelectionAreaGraphicsItem*)_selectionAreaGraphicsItem)->updateExtents(position);
}

void MyNetworkElementSelector::selectSelectionAreaCoveredElements() {
    setCollidingElementsSelected(askForNodes() + askForEdges());
}

void MyNetworkElementSelector::selectSelectionAreaCoveredNodes() {
    setCollidingElementsSelected(askForNodes());
}

void MyNetworkElementSelector::selectSelectionAreaCoveredEdges() {
    setCollidingElementsSelected(askForEdges());
}

void MyNetworkElementSelector::setCollidingElementsSelected(QList<MyNetworkElementBase*> elements) {
    QList<QGraphicsItem *> selectedItems = _selectionAreaGraphicsItem->collidingItems();
    for (MyNetworkElementBase* element : elements) {
        for (QGraphicsItem* item : qAsConst(selectedItems)) {
            if (item->parentItem()) {
                if (element->graphicsItem() == item->parentItem())
                    element->setSelected(true);
            }
        }
    }
}

void MyNetworkElementSelector::clearSelectionArea() {
    if (_selectionAreaGraphicsItem) {
        if (!askForWhetherShiftModifierIsPressed())
            selectNetworkElements(false);
        selectSelectionAreaCoveredNodes();
        selectSelectionAreaCoveredEdges();

        askForRemoveGraphicsItem(_selectionAreaGraphicsItem);
        delete _selectionAreaGraphicsItem;
        _selectionAreaGraphicsItem = NULL;
    }
}

