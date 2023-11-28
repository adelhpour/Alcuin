#include "negui_plugin_item_call_function.h"

#include "QJsonArray"

MyPluginItemCallFunction::MyPluginItemCallFunction(const QString& name) : MyBase(name) {

}

const QStringList& MyPluginItemCallFunction::inputAPIFunctions() const {
    return _inputAPIFunctions;
}

void MyPluginItemCallFunction::read(const QJsonObject &json) {
    _inputAPIFunctions.clear();
    if (json.contains("input-api-functions") && json["input-api-functions"].isArray()) {
        QJsonArray inputAPIFunctionsArray = json["input-api-functions"].toArray();
        for (int inputAPIFunctionIndex = 0; inputAPIFunctionIndex < inputAPIFunctionsArray.size(); ++inputAPIFunctionIndex) {
            if (inputAPIFunctionsArray[inputAPIFunctionIndex].isString())
                _inputAPIFunctions.append(inputAPIFunctionsArray[inputAPIFunctionIndex].toString());
        }
    }
}

void MyPluginItemCallFunction::write(QJsonObject &json) {
    QJsonArray inputAPIFunctionsArray;
    for (QString inputAPIFunction : inputAPIFunctions())
        inputAPIFunctionsArray.append(inputAPIFunction);
    json["input-api-functions"] = inputAPIFunctionsArray;
}
