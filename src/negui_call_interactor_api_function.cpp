#include "negui_call_interactor_api_function.h"
#include "negui_interactor.h"

const QJsonValue callInteractorAPIFunction(QObject* interactor, const QString& functionName, const QJsonValue& inputs) {
    if (inputs.isArray()) {
        QJsonArray inputArray = inputs.toArray();
        if (functionName == "selectElements") {
            if (inputArray.size() == 1 && inputArray[0].isBool())
                ((MyInteractor*)interactor)->selectElements(inputArray[0].toBool());
        }
        else if (functionName == "createNetwork") {
            if (inputArray.size() == 1 && inputArray[0].isObject())
                ((MyInteractor*)interactor)->createNetwork(inputArray[0].toObject());
        }
        else if (functionName == "takeParameterFromUser") {
            if (inputArray.size() == 2 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->takeParameterFromUser(inputArray[0].toString(), inputArray[1]);
        }
        else if (functionName == "getOpenFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getOpenFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveAsFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveAsFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveFileName(inputArray[0].toString());
        }
        else if (functionName == "getSaveFigureFileName") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                return ((MyInteractor*)interactor)->getSaveFigureFileName(inputArray[0].toString());
        }
        else if (functionName == "saveFigure") {
            if (inputArray.size() == 1 && inputArray[0].isString())
                ((MyInteractor*)interactor)->saveFigure(inputArray[0].toString());
        }
        else if (functionName == "deleteSelectedNetworkElements")
            ((MyInteractor*)interactor)->deleteSelectedNetworkElements();
        else if (functionName == "exportNetworkInfo")
            return ((MyInteractor*)interactor)->exportNetworkInfo();
        else if (functionName == "askForAdjustExtentsOfNodes")
            emit ((MyInteractor*)interactor)->askForAdjustExtentsOfNodes();
        else if (functionName == "askForAdjustConnectedEdgesOfNodes")
            emit ((MyInteractor*)interactor)->askForAdjustConnectedEdgesOfNodes();
        else if (functionName == "createChangeStageCommand")
            ((MyInteractor*)interactor)->createChangeStageCommand();
        else if (functionName == "resetCanvas")
            ((MyInteractor*)interactor)->resetCanvas();
        else if (functionName == "enableNormalMode")
            ((MyInteractor*)interactor)->enableNormalMode();
        else if (functionName == "saveCurrentNetwork")
            ((MyInteractor*)interactor)->saveCurrentNetwork();
        else if (functionName == "saveCurrentNetworkWithUserPermission")
            ((MyInteractor*)interactor)->saveCurrentNetworkWithUserPermission();
    }

    return QJsonObject();
}
