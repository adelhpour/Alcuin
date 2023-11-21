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
        CPyObject itemsInfoFunc = PyObject_GetAttrString(script.getObject(), (char*)"items_info");
        if (itemsInfoFunc) {
            CPyObject itemsInfo = PyObject_CallObject(itemsInfoFunc.getObject(), PyTuple_Pack(0));
            if (itemsInfo)
                return createItemsInfoJsonObject(itemsInfo);
        }
    }

    return object;
}

const QJsonObject GeneralPlugin::createItemsInfoJsonObject(CPyObject itemsInfo) {
    QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(itemsInfo, "utf-8", "~E~"))).toUtf8());
    if (!doc.isNull())
        return doc.object();

    return QJsonObject();
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

QStringList GeneralPlugin::call(const QString& functionName, const QStringList& inputList) {
    PyErr_Print();
    for (CPyObject script : _scripts) {
        CPyObject function = PyObject_GetAttrString(script.getObject(), (char*)functionName.toStdString().c_str());
        if (function)
            return processPythonOutput(PyObject_CallObject(function.getObject(), createPythonInput(inputList)));
    }

    return QStringList();
}

CPyObject GeneralPlugin::createPythonInput(const QStringList& inputList) {
    CPyObject inputs = PyTuple_New(inputList.size());
    for (unsigned int i = 0; i < inputList.size(); i++)
        PyTuple_SetItem(inputs, i, PyUnicode_FromString((char*)inputList.at(i).toStdString().c_str()));

    return PyTuple_Pack(1, inputs.getObject());
}

const QStringList GeneralPlugin::processPythonOutput(CPyObject output) {
    QStringList outputList;
    if (output && PyTuple_Check(output)) {
        for (unsigned int i = 0; i < PyTuple_GET_SIZE(output.getObject()); i++)
            outputList.append(QString(PyBytes_AsString(PyUnicode_AsEncodedString(PyTuple_GetItem(output.getObject(), i), "utf-8", "~E~"))));
    }

    return outputList;
}