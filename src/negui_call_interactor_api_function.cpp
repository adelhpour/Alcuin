#include "negui_call_interactor_api_function.h"
#include "negui_interactor.h"

const QJsonValue callInteractorAPIFunction(QObject* interactor, const QString& functionName, const QJsonValue& inputs) {
    if (inputs.isArray()) {
        QJsonArray inputArray = inputs.toArray();
        if (functionName == "enableNormalMode")
            ((MyInteractor*)interactor)->enableNormalMode();
        else if (functionName == "enableAddNodeMode") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->enableAddNodeMode(inputArray[0].toString());
        }
        else if (functionName == "enableAddEdgeMode") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->enableAddEdgeMode(inputArray[0].toString());
        }
        else if (functionName == "setNewNetworkCanvas")
            ((MyInteractor*)interactor)->setNewNetworkCanvas();
        else if (functionName == "resetNetworkCanvas")
            ((MyInteractor*)interactor)->resetNetworkCanvas();
        else if (functionName == "resetNetwork")
            ((MyInteractor*)interactor)->resetNetwork();
        else if (functionName == "resetCanvas")
            ((MyInteractor*)interactor)->resetCanvas();
        else if (functionName == "createNetwork") {
            if (inputArray.size() == 1 && inputArray[0].isObject())
                ((MyInteractor*)interactor)->createNetwork(inputArray[0].toObject());
        }
        else if (functionName == "saveCurrentNetwork")
            ((MyInteractor*)interactor)->saveCurrentNetwork();
        else if (functionName == "saveCurrentNetworkWithUserPermission")
            ((MyInteractor*)interactor)->saveCurrentNetworkWithUserPermission();
        else if (functionName == "saveFigure") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->saveFigure(inputArray[0].toString());
        }
        else if (functionName == "exportNetworkInfo")
            return ((MyInteractor*)interactor)->exportNetworkInfo();
        else if (functionName == "triggerUndoAction")
            ((MyInteractor*)interactor)->triggerUndoAction();
        else if (functionName == "triggerRedoAction")
            ((MyInteractor*)interactor)->triggerRedoAction();
        else if (functionName == "createChangeStageCommand")
            ((MyInteractor*)interactor)->createChangeStageCommand();
        else if (functionName == "cutSelectedNetworkElements")
            ((MyInteractor*)interactor)->cutSelectedNetworkElements();
        else if (functionName == "copySelectedNetworkElements")
            ((MyInteractor*)interactor)->copySelectedNetworkElements();
        else if (functionName == "pasteCopiedNetworkElements")
            ((MyInteractor*)interactor)->pasteCopiedNetworkElements();
        else if (functionName == "pasteCopiedNetworkElements") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyInteractor*)interactor)->pasteCopiedNetworkElements(inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "resetCopiedNetworkElements")
            ((MyInteractor*)interactor)->resetCopiedNetworkElements();
        else if (functionName == "areSelectedElementsCopyable")
            return ((MyInteractor*)interactor)->areSelectedElementsCopyable();
        else if (functionName == "areSelectedElementsCuttable")
            return ((MyInteractor*)interactor)->areSelectedElementsCuttable();
        else if (functionName == "areAnyElementsCopied")
            return ((MyInteractor*)interactor)->areAnyElementsCopied();
        else if (functionName == "selectNetworkElements") {
            if (inputArray.size() == 1 && inputArray[0].isBool())
                ((MyInteractor*)interactor)->selectNetworkElements( inputArray[0].toBool());
        }
        else if (functionName == "selectNetworkElementsOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyInteractor*)interactor)->selectNetworkElementsOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "selectNodes") {
            if (inputArray.size() == 1 && inputArray[0].isBool()) {
                ((MyInteractor*)interactor)->selectNodes( inputArray[0].toBool());
            }
        }
        else if (functionName == "selectNodesOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyInteractor*)interactor)->selectNodesOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "selectEdges") {
            if (inputArray.size() == 1 && inputArray[0].isBool()) {
                ((MyInteractor*)interactor)->selectEdges( inputArray[0].toBool());
            }
        }
        else if (functionName == "selectEdgesOfCategory") {
            if (inputArray.size() == 2 && inputArray[0].isBool() && inputArray[1].isString()) {
                ((MyInteractor*)interactor)->selectEdgesOfCategory( inputArray[0].toBool(), inputArray[1].toString());
            }
        }
        else if (functionName == "setNetworkElementSelected") {
            if (inputArray.size() == 2 && inputArray[0].isString() && inputArray[1].isBool()) {
                ((MyInteractor*)interactor)->setNetworkElementSelected( inputArray[0].toString(), inputArray[1].toBool());
            }
        }
        else if (functionName == "areAnyElementsSelected")
            return ((MyInteractor*)interactor)->areAnyElementsSelected();
        else if (functionName == "displaySelectionArea") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyInteractor*)interactor)->displaySelectionArea( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "clearSelectionArea")
            ((MyInteractor*)interactor)->clearSelectionArea();
        else if (functionName == "areSelectedElementsAlignable")
            return ((MyInteractor*)interactor)->areSelectedElementsAlignable();
        else if (functionName == "displaySelectionArea") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->alignSelectedNetworkElements(inputArray[0].toString());
        }
        else if (functionName == "deleteSelectedNetworkElements")
            ((MyInteractor*)interactor)->deleteSelectedNetworkElements();
        else if (functionName == "updateFeatureMenu")
            ((MyInteractor*)interactor)->updateFeatureMenu();
        else if (functionName == "addNode") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyInteractor*)interactor)->addNode( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "applicationDirectoryPath")
            return ((MyInteractor*)interactor)->applicationDirectoryPath();
        else if (functionName == "iconsDirectoryPath")
            return ((MyInteractor*)interactor)->iconsDirectoryPath();
        else if (functionName == "workingDirectoryPath")
            return ((MyInteractor*)interactor)->workingDirectoryPath();
        else if (functionName == "setWorkingDirectoryPath") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->setWorkingDirectoryPath(inputArray[0].toString());
        }
        else if (functionName == "getOpenFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getOpenFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveAsFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveAsFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFigureFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveFigureFileName(inputArray[0].toString());
        }
        else if (functionName == "takeParameterFromUser") {
            if (inputArray.size() == 2 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->takeParameterFromUser(inputArray[0].toString(), inputArray[1]);
        }
        else if (functionName == "askForNetworkBackgroundColor")
            return ((MyInteractor*)interactor)->askForNetworkBackgroundColor();
        else if (functionName == "askForSetNetworkBackgroundColor") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->askForSetNetworkBackgroundColor(inputArray[0].toString());
        }
        else if (functionName == "askForAdjustConnectedEdgesOfNodes")
            ((MyInteractor*)interactor)->askForAdjustConnectedEdgesOfNodes();
        else if (functionName == "askForAdjustExtentsOfNodes")
            ((MyInteractor*)interactor)->askForAdjustExtentsOfNodes();
        else if (functionName == "askForSaveFigure") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->askForSaveFigure(inputArray[0].toString());
        }
        else if (functionName == "askForClearScene")
            ((MyInteractor*)interactor)->askForClearScene();
        else if (functionName == "askForResetScale")
            ((MyInteractor*)interactor)->askForResetScale();
        else if (functionName == "askForSetToolTip") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->askForSetToolTip(inputArray[0].toString());
        }
        else if (functionName == "askForDisplayNullFeatureMenu")
            ((MyInteractor*)interactor)->askForDisplayNullFeatureMenu();
        else if (functionName == "askForRemoveFeatureMenu")
            ((MyInteractor*)interactor)->askForRemoveFeatureMenu();
        else if (functionName == "askForEnableFeatureMenuDisplay")
            ((MyInteractor*)interactor)->askForEnableFeatureMenuDisplay();
        else if (functionName == "askForWhetherFeatureMenuCanBeDisplayed")
            return ((MyInteractor*)interactor)->askForWhetherFeatureMenuCanBeDisplayed();
        else if (functionName == "askForDisplaySceneContextMenu") {
            if (inputArray.size() == 2 && inputArray[0].isDouble() && inputArray[1].isDouble())
                ((MyInteractor*)interactor)->askForDisplaySceneContextMenu( inputArray[0].toDouble(), inputArray[1].toDouble());
        }
        else if (functionName == "askForWhetherShiftModifierIsPressed")
            return ((MyInteractor*)interactor)->askForWhetherShiftModifierIsPressed();
        else if (functionName == "askForWhetherControlModifierIsPressed")
            return ((MyInteractor*)interactor)->askForWhetherControlModifierIsPressed();
    }

    return QJsonValue();
}
