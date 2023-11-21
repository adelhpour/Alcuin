#include "negui_general_plugin.h"
#include <QJsonDocument>

int GeneralPlugin::initialize(const QString &appPath, const QString &pluginsPath) {
    _pyInstance = new CPyInstance(appPath.toStdString().c_str());
    _pyInstance->appendPath(pluginsPath.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_general_plugin"));
    if (_script)
        return 0;

    return -1;
}

const QJsonObject GeneralPlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject itemsInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"items_info");
        if (itemsInfoFunc) {
            CPyObject itemsInfo = PyObject_CallObject(itemsInfoFunc.getObject(), PyTuple_Pack(0));
            if (itemsInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(itemsInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }

    return object;
}

QStringList GeneralPlugin::call(const QString& functionName, const QStringList& inputList) {
    QStringList outputList;
    if (_pyInstance && _script) {
        CPyObject function = PyObject_GetAttrString(_script.getObject(), (char*)functionName.toStdString().c_str());
        if (function) {
            CPyObject inputs = PyTuple_New(inputList.size());
            for (unsigned int i = 0; i < inputList.size(); i++)
                PyTuple_SetItem(inputs, i, PyUnicode_FromString((char*)inputList.at(i).toStdString().c_str()));
            CPyObject input = PyTuple_Pack(1, inputs.getObject());
            CPyObject output = PyObject_CallObject(function.getObject(), input);
            if (output && PyTuple_Check(output)) {
                for (unsigned int i = 0; i < PyTuple_GET_SIZE(output.getObject()); i++)
                   outputList.append(QString(PyBytes_AsString(PyUnicode_AsEncodedString(PyTuple_GetItem(output.getObject(), i), "utf-8", "~E~"))));
            }
        }
    }

    return outputList;
}
