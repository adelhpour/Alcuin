#include "negui_call_api_function.h"
#include "negui_main_widget.h"

#include <QJsonObject>
#include <QJsonArray>

const QJsonValue callAPIFunction(QObject* mainWidget, const QString& functionName, const QJsonValue& inputs) {
    if (inputs.isArray()) {
        QJsonArray inputArray = inputs.toArray();
        if (functionName == "enableNormalMode")
            ((MyMainWidget*)mainWidget)->enableNormalMode();
        else if (functionName == "enableAddNodeMode") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->enableAddNodeMode(inputArray[0].toString());
        }
        else if (functionName == "enableAddEdgeMode") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->enableAddEdgeMode(inputArray[0].toString());
        }
        else if (functionName == "setNewNetworkCanvas")
            ((MyMainWidget*)mainWidget)->setNewNetworkCanvas();
        else if (functionName == "resetNetworkCanvas")
            ((MyMainWidget*)mainWidget)->resetNetworkCanvas();
        else if (functionName == "resetNetwork")
            ((MyMainWidget*)mainWidget)->resetNetwork();
        else if (functionName == "resetCanvas")
            ((MyMainWidget*)mainWidget)->resetCanvas();
        else if (functionName == "createNetwork") {
            if (inputArray.size() == 1 && inputArray[0].isObject())
                ((MyMainWidget*)mainWidget)->createNetwork(inputArray[0].toObject());
        }
        else if (functionName == "saveCurrentNetwork")
            ((MyMainWidget*)mainWidget)->saveCurrentNetwork();
        else if (functionName == "saveCurrentNetworkWithUserPermission")
            ((MyMainWidget*)mainWidget)->saveCurrentNetworkWithUserPermission();
        else if (functionName == "saveFigure") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->saveFigure(inputArray[0].toString());
        }
        else if (functionName == "exportNetworkInfo")
            return ((MyMainWidget*)mainWidget)->exportNetworkInfo();
        else if (functionName == "triggerUndoAction")
            ((MyMainWidget*)mainWidget)->triggerUndoAction();
        else if (functionName == "triggerRedoAction")
            ((MyMainWidget*)mainWidget)->triggerRedoAction();
        else if (functionName == "createChangeStageCommand")
            ((MyMainWidget*)mainWidget)->createChangeStageCommand();
        else if (functionName == "cutSelectedNetworkElements")
            ((MyMainWidget*)mainWidget)->cutSelectedNetworkElements();
        else if (functionName == "copySelectedNetworkElements")
            ((MyMainWidget*)mainWidget)->copySelectedNetworkElements();
        else if (functionName == "pasteCopiedNetworkElements")
            ((MyMainWidget*)mainWidget)->pasteCopiedNetworkElements();
        else if (functionName == "pasteCopiedNetworkElements") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyMainWidget*)mainWidget)->pasteCopiedNetworkElements(inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "resetCopiedNetworkElements")
            ((MyMainWidget*)mainWidget)->resetCopiedNetworkElements();
        else if (functionName == "areSelectedElementsCopyable")
            return ((MyMainWidget*)mainWidget)->areSelectedElementsCopyable();
        else if (functionName == "areSelectedElementsCuttable")
            return ((MyMainWidget*)mainWidget)->areSelectedElementsCuttable();
        else if (functionName == "areAnyElementsCopied")
            return ((MyMainWidget*)mainWidget)->areAnyElementsCopied();
        else if (functionName == "selectNetworkElements") {
            if (inputArray.size() == 1 && inputArray[0].isBool())
                ((MyMainWidget*)mainWidget)->selectNetworkElements( inputArray[0].toBool());
        }
        else if (functionName == "selectNetworkElementsOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyMainWidget*)mainWidget)->selectNetworkElementsOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "selectNodes") {
            if (inputArray.size() == 1 && inputArray[0].isBool()) {
                ((MyMainWidget*)mainWidget)->selectNodes( inputArray[0].toBool());
            }
        }
        else if (functionName == "selectNodesOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyMainWidget*)mainWidget)->selectNodesOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "selectEdges") {
            if (inputArray.size() == 1 && inputArray[0].isBool()) {
                ((MyMainWidget*)mainWidget)->selectEdges( inputArray[0].toBool());
            }
        }
        else if (functionName == "selectEdgesOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyMainWidget*)mainWidget)->selectEdgesOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "setNetworkElementSelected") {
            if (inputArray.size() == 2 && inputArray[0].isString() && inputArray[1].isBool()) {
                ((MyMainWidget*)mainWidget)->setNetworkElementSelected( inputArray[0].toString(), inputArray[1].toBool());
            }
        }
        else if (functionName == "areAnyElementsSelected")
            return ((MyMainWidget*)mainWidget)->areAnyElementsSelected();
        else if (functionName == "displaySelectionArea") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyMainWidget*)mainWidget)->displaySelectionArea( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "clearSelectionArea")
            ((MyMainWidget*)mainWidget)->clearSelectionArea();
        else if (functionName == "areSelectedElementsAlignable")
            return ((MyMainWidget*)mainWidget)->areSelectedElementsAlignable();
        else if (functionName == "displaySelectionArea") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->alignSelectedNetworkElements(inputArray[0].toString());
        }
        else if (functionName == "deleteSelectedNetworkElements")
            ((MyMainWidget*)mainWidget)->deleteSelectedNetworkElements();
        else if (functionName == "updateFeatureMenu")
            ((MyMainWidget*)mainWidget)->updateFeatureMenu();
        else if (functionName == "addNode") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyMainWidget*)mainWidget)->addNode( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "addEdge") {
            if (inputArray.size() == 2 && inputArray[0].isArray() && inputArray[1].isArray()) {
                QList<QString> sourceNodes;
                QList<QString> targetNodes;
                for (unsigned int inputArrayZeroIndex = 0; inputArrayZeroIndex < inputArray[0].toArray().size(); inputArrayZeroIndex++)
                    if (inputArray[0].toArray()[inputArrayZeroIndex].isString())
                        sourceNodes.push_back(inputArray[0].toArray()[inputArrayZeroIndex].toString());
                for (unsigned int inputArrayOneIndex = 0; inputArrayOneIndex < inputArray[1].toArray().size(); inputArrayOneIndex++)
                    if (inputArray[1].toArray()[inputArrayOneIndex].isString())
                        targetNodes.push_back(inputArray[1].toArray()[inputArrayOneIndex].toString());
                ((MyMainWidget*)mainWidget)->addEdge(sourceNodes, targetNodes);
            }
        }
        else if (functionName == "applicationDirectoryPath")
            return ((MyMainWidget*)mainWidget)->applicationDirectoryPath();
        else if (functionName == "iconsDirectoryPath")
            return ((MyMainWidget*)mainWidget)->iconsDirectoryPath();
        else if (functionName == "workingDirectoryPath")
            return ((MyMainWidget*)mainWidget)->workingDirectoryPath();
        else if (functionName == "setWorkingDirectoryPath") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->setWorkingDirectoryPath(inputArray[0].toString());
        }
        else if (functionName == "getOpenFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->getOpenFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->getSaveFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveAsFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->getSaveAsFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFigureFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->getSaveFigureFileName(inputArray[0].toString());
        }
        else if (functionName == "takeParameterFromUser") {
            if (inputArray.size() == 2 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->takeParameterFromUser(inputArray[0].toString(), inputArray[1]);
        }
        else if (functionName == "showParameterValue") {
            if (inputArray.size() == 2 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->showParameterValue(inputArray[0].toString(), inputArray[1]);
        }
        else if (functionName == "getNetworkBackgroundColor")
            return ((MyMainWidget*)mainWidget)->getNetworkBackgroundColor();
        else if (functionName == "setNetworkBackgroundColor") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->setNetworkBackgroundColor(inputArray[0].toString());
        }
        else if (functionName == "adjustConnectedEdgesOfNodes")
            ((MyMainWidget*)mainWidget)->adjustConnectedEdgesOfNodes();
        else if (functionName == "adjustExtentsOfNodes")
            ((MyMainWidget*)mainWidget)->adjustExtentsOfNodes();
        else if (functionName == "saveFigure") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->saveFigure(inputArray[0].toString());
        }
        else if (functionName == "clearScene")
            ((MyMainWidget*)mainWidget)->clearScene();
        else if (functionName == "resetScale")
            ((MyMainWidget*)mainWidget)->resetScale();
        else if (functionName == "setToolTip") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyMainWidget*)mainWidget)->setToolTip(inputArray[0].toString());
        }
        else if (functionName == "displayNullFeatureMenu")
            ((MyMainWidget*)mainWidget)->displayNullFeatureMenu();
        else if (functionName == "removeFeatureMenu")
            ((MyMainWidget*)mainWidget)->removeFeatureMenu();
        else if (functionName == "enableDisplayFeatureMenu") {
            if (inputArray.size() == 1 && inputArray[0].isBool())
                ((MyMainWidget*)mainWidget)->enableDisplayFeatureMenu(inputArray[0].toBool());
        }

        else if (functionName == "canDisplayFeatureMenu")
            return ((MyMainWidget*)mainWidget)->canDisplayFeatureMenu();
        else if (functionName == "displaySceneContextMenu") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyMainWidget*)mainWidget)->displaySceneContextMenu( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "isShiftModifierPressed")
            return ((MyMainWidget*)mainWidget)->isShiftModifierPressed();
        else if (functionName == "isControlModifierPressed")
            return ((MyMainWidget*)mainWidget)->isControlModifierPressed();
        else if (functionName == "zoomIn")
            ((MyMainWidget*)mainWidget)->zoomIn();
        else if (functionName == "zoomOut")
            ((MyMainWidget*)mainWidget)->zoomOut();
        else if (functionName == "displayNullFeatureMenu")
            ((MyMainWidget*)mainWidget)->displayNullFeatureMenu();
        else if (functionName == "removeFeatureMenu")
            ((MyMainWidget*)mainWidget)->removeFeatureMenu();
        else if (functionName == "getName")
            return ((MyMainWidget*)mainWidget)->getName();
        else if (functionName == "getVersionNumber")
            return ((MyMainWidget*)mainWidget)->getVersionNumber();
        else if (functionName == "listOfPluginItemNames") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->listOfPluginItemNames(inputArray[0].toString());
        }
        else if (functionName == "listOfPluginItemCategories") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyMainWidget*)mainWidget)->listOfPluginItemCategories(inputArray[0].toString());
        }
        else if (functionName == "listOfNodes")
            return ((MyMainWidget*)mainWidget)->listOfNodes();
        else if (functionName == "listOfEdges")
            return ((MyMainWidget*)mainWidget)->listOfEdges();
        else if (functionName == "numberOfNodes")
            return ((MyMainWidget*)mainWidget)->numberOfNodes();
        else if (functionName == "numberOfEdges")
            return ((MyMainWidget*)mainWidget)->numberOfEdges();
    }

    return QJsonValue();
}
