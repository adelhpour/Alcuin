#include "negui_plugin_item_call_function.h"

#include "QJsonArray"

MyPluginItemCallFunction::MyPluginItemCallFunction(const QString& name) : MyBase(name) {

}

void MyPluginItemCallFunction::call() {
    call(_callFunctionInfo);
}

const QJsonValue MyPluginItemCallFunction::call(QJsonValue json) {
    if (json.isObject()) {
        QJsonObject jsonObject = json.toObject();
        if (jsonObject.contains("name") && jsonObject["name"].isString()) {
            jsonObject["inputs"] = processInputArray(json);
            if (jsonObject.contains("api") && jsonObject["api"].isString() && jsonObject["api"].toString() == "python")
                return askForCallPythonFunction(jsonObject["name"].toString(), jsonObject["inputs"]);
            else
                return askForCallCPlusPlusFunction(jsonObject["name"].toString(), jsonObject["inputs"]);
        }
    }

    return QJsonValue();
}

const QJsonArray MyPluginItemCallFunction::processInputArray(QJsonValue json) {
    QJsonArray inputsArray;
    if (json.isObject()) {
        QJsonObject jsonObject = json.toObject();
        if (jsonObject.contains("inputs") && jsonObject["inputs"].isArray()) {
            inputsArray = json["inputs"].toArray();
            for (int inputsIndex = 0; inputsIndex < inputsArray.size(); ++inputsIndex) {
                if (inputsArray[inputsIndex].isObject())
                    inputsArray[inputsIndex] = call(inputsArray[inputsIndex]);
            }
        }
    }

    return inputsArray;
}

void MyPluginItemCallFunction::read(const QJsonObject &json) {
    _callFunctionInfo = json;
}

void MyPluginItemCallFunction::write(QJsonObject &json) {
    json = _callFunctionInfo.toObject();
}