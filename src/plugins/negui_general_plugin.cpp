#include "negui_general_plugin.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

int GeneralPlugin::initialize(const QString &appPath, const QString &pluginsPath) {
    _pyInstance = new CPyInstance(appPath.toStdString().c_str());
    _pyInstance->appendPath(pluginsPath.toStdString().c_str());
    loadPluginScripts(pluginsPath);
    if (_scripts.size())
        return 0;

    return -1;
}

void GeneralPlugin::loadPluginScripts(const QString &pluginsPath) {
    QDir pluginsDir = QDir(pluginsPath);
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        if (fileName.split(".").size() && fileName.split(".").last() == "py") {
            CPyObject script;
            script.setObject(_pyInstance->importModule(fileName.split(".").first().toStdString().c_str()));
            _scripts.append(script);
        }
    }
}

const QJsonObject GeneralPlugin::loadItemsInfo() {
    QJsonObject object;
    object["items"] = QJsonArray();
    for (CPyObject script : _scripts)
        object = addItemsInfo(object, loadItemsInfo(script));

    return object;
}

const QJsonObject GeneralPlugin::loadItemsInfo(CPyObject script) {
    QJsonObject object;
    PyErr_Print();
    if (script) {
        CPyObject function = PyObject_GetAttrString(script.getObject(), (char*)"items_info");
        if (function)
            return processFunctionOutput(PyObject_CallObject(function.getObject(), PyTuple_Pack(0)));
    }

    return object;
}

const QJsonObject GeneralPlugin::addItemsInfo(QJsonObject itemsInfo, const QJsonObject& newItemsInfo) {
    if (itemsInfo.contains("items") && itemsInfo["items"].isArray()) {
        QJsonArray itemsArray = itemsInfo["items"].toArray();
        if (newItemsInfo.contains("items") && newItemsInfo["items"].isArray()) {
            QJsonArray newItemsArray = newItemsInfo["items"].toArray();
            for (int itemIndex = 0; itemIndex < newItemsArray.size(); ++itemIndex)
                itemsArray.append(newItemsArray[itemIndex].toObject());
        }
        itemsInfo["items"] = itemsArray;
    }

    return itemsInfo;
}

const QJsonObject GeneralPlugin::call(const QString& functionName, const QStringList& callFunctionInput) {
    PyErr_Print();
    for (CPyObject script : _scripts) {
        CPyObject function = PyObject_GetAttrString(script.getObject(), (char*)functionName.toStdString().c_str());
        if (function)
            return processFunctionOutput(PyObject_CallObject(function.getObject(), createFunctionInput(callFunctionInput)));
    }

    return QJsonObject();
}

CPyObject GeneralPlugin::createFunctionInput(const QStringList& functionInputList) {
    CPyObject inputs = PyTuple_New(functionInputList.size());
    for (unsigned int i = 0; i < functionInputList.size(); i++)
        PyTuple_SetItem(inputs, i, PyUnicode_FromString((char*)functionInputList.at(i).toStdString().c_str()));

    return PyTuple_Pack(1, inputs.getObject());
}

const QJsonObject GeneralPlugin::processFunctionOutput(CPyObject functionOutput) {
    if (functionOutput) {
        QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(functionOutput, "utf-8", "~E~"))).toUtf8());
        if (!doc.isNull())
            return doc.object();
    }

    return QJsonObject();
}