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

const QJsonValue GeneralPlugin::loadItemsInfo(CPyObject script) {
    PyErr_Print();
    if (script) {
        CPyObject function = PyObject_GetAttrString(script.getObject(), (char*)"items_info");
        if (function)
            return processFunctionOutput(PyObject_CallObject(function.getObject(), PyTuple_Pack(0)));
    }

    return QJsonValue();
}

const QJsonObject GeneralPlugin::addItemsInfo(QJsonObject itemsInfo, const QJsonValue& newItemsInfo) {
    if (itemsInfo.contains("items") && itemsInfo["items"].isArray()) {
        QJsonArray itemsArray = itemsInfo["items"].toArray();
        if (newItemsInfo.isObject()) {
            QJsonObject newItemsInfoObject = newItemsInfo.toObject();
            if (newItemsInfoObject.contains("items") && newItemsInfoObject["items"].isArray()) {
                QJsonArray newItemsArray = newItemsInfoObject["items"].toArray();
                for (int itemIndex = 0; itemIndex < newItemsArray.size(); ++itemIndex)
                    itemsArray.append(newItemsArray[itemIndex].toObject());
            }
        }
        itemsInfo["items"] = itemsArray;
    }

    return itemsInfo;
}

const QJsonValue GeneralPlugin::call(const QString& functionName, const QJsonValue& functionInput) {
    PyErr_Print();
    for (CPyObject script : _scripts) {
        CPyObject function = PyObject_GetAttrString(script.getObject(), (char*)functionName.toStdString().c_str());
        if (function)
            return processFunctionOutput(PyObject_CallObject(function.getObject(), processFunctionInput(functionInput)));
    }

    return QJsonValue();
}

CPyObject GeneralPlugin::createFunctionInput(const QStringList& functionInputList) {
    CPyObject inputs = PyTuple_New(functionInputList.size());
    for (unsigned int i = 0; i < functionInputList.size(); i++)
        PyTuple_SetItem(inputs, i, PyUnicode_FromString((char*)functionInputList.at(i).toStdString().c_str()));

    return PyTuple_Pack(1, inputs.getObject());
}

CPyObject GeneralPlugin::processFunctionInput(const QJsonValue& functionInput) {
    CPyObject inputs = createPythonObject(functionInput);
    if (!inputs.getObject())
        inputs = PyTuple_New(0);
    return PyTuple_Pack(1, inputs.getObject());
}

CPyObject GeneralPlugin::createPythonObject(const QJsonValue& jsonInput) {
    if (jsonInput.isArray()) {
        QJsonArray jsonInputArray = jsonInput.toArray();
        CPyObject pythonInputs = PyTuple_New(jsonInputArray.size());
        for (int inputIndex = 0; inputIndex < jsonInputArray.size(); ++inputIndex) {
            CPyObject pythonInput = createPythonObject(jsonInputArray[inputIndex]);
            if (pythonInput.getObject())
                PyTuple_SetItem(pythonInputs, inputIndex, pythonInput.getObject());
        }
        return pythonInputs;
    }
    else if (jsonInput.isString())
        return PyUnicode_FromString((char*)jsonInput.toString().toStdString().c_str());
    else if (jsonInput.isDouble())
        return PyFloat_FromDouble(jsonInput.toDouble());
    else if (jsonInput.isBool()) {
        if (jsonInput.toBool())
            return Py_True;
        else
            return Py_False;
    }
    else if (jsonInput.isObject())
        return PyUnicode_FromString((char*)QJsonDocument(jsonInput.toObject()).toJson(QJsonDocument::Compact).toStdString().c_str());

    return CPyObject();
}

const QJsonValue GeneralPlugin::processFunctionOutput(CPyObject functionOutput) {
    if (PyUnicode_Check(functionOutput.getObject())) {
        QString outputString = QString(PyBytes_AsString(PyUnicode_AsEncodedString(functionOutput.getObject(), "utf-8", "~E~")));
        QJsonDocument doc = QJsonDocument::fromJson(outputString.toUtf8());
        if (!doc.isNull())
            return doc.object();
        else
            return outputString;
    }
    else if (PyFloat_Check(functionOutput.getObject()))
        return PyFloat_AsDouble(functionOutput.getObject());
    else if (PyBool_Check(functionOutput.getObject())) {
        if (PyObject_IsTrue(functionOutput.getObject()))
            return true;
        else
            return false;
    }

    return QJsonValue();
}