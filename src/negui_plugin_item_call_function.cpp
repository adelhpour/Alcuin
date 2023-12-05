#include "negui_plugin_item_call_function.h"

#include "QJsonArray"

MyPluginItemCallFunction::MyPluginItemCallFunction(const QString& name) : MyBase(name) {

}

void MyPluginItemCallFunction::call() {
    call(_callFunctionInfo);
}

const QJsonObject MyPluginItemCallFunction::call(QJsonObject json) {
    if (json.contains("name") && json["name"].isString()) {
        json["inputs"] = processInputArray(json);
        if (json.contains("api") && json["api"].isString() && json["api"].toString() == "python")
            return askForCallPythonFunction(json["name"].toString(), json);
        else
            return askForCallCPlusPlusFunction(json["name"].toString(), json);
    }

    return QJsonObject();
}

const QJsonArray MyPluginItemCallFunction::processInputArray(QJsonObject json) {
    QJsonArray inputsArray;
    if (json.contains("inputs") && json["inputs"].isArray()) {
        inputsArray = json["inputs"].toArray();
        for (int inputsIndex = 0; inputsIndex < inputsArray.size(); ++inputsIndex) {
            if (inputsArray[inputsIndex].isObject()) {
                QJsonObject output = call(inputsArray[inputsIndex].toObject());
                if (output.contains("value"))
                    inputsArray[inputsIndex] = output["value"];
                else
                    inputsArray[inputsIndex] = output;
            }
        }
    }

    return inputsArray;
}

void MyPluginItemCallFunction::read(const QJsonObject &json) {
    _callFunctionInfo = json;
}

void MyPluginItemCallFunction::write(QJsonObject &json) {
    json = _callFunctionInfo;
}