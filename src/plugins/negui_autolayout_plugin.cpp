#include "negui_autolayout_plugin.h"

int AutoLayoutPlugin::initialize(const QString &path) {
    _path = path;
    _pyInstance = new CPyInstance();
    _pyInstance->appendPath(_path.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_autolayout_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject AutoLayoutPlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject autoLayoutInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"autoLayoutInfo");
        if (autoLayoutInfoFunc) {
            CPyObject autoLayoutInfo = PyObject_CallObject(autoLayoutInfoFunc.getObject(), PyTuple_Pack(0));
            if (autoLayoutInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(autoLayoutInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
        else if (PyObject_GetAttrString(_script.getObject(), (char*)"autoLayout")) {
            QJsonObject defaultEngineObject;
            defaultEngineObject["engine"] = "Default";
            QJsonArray enginesArray;
            enginesArray.append(defaultEngineObject);
            object["engines"] = enginesArray;
        }
    }
    
    return object;
}

void AutoLayoutPlugin::autoLayout(QJsonObject &graphInfoObject, QJsonObject &autoLayoutInfoObject) {
    if (_pyInstance && _script) {
        CPyObject autoLayoutFunc = PyObject_GetAttrString(_script.getObject(), (char*)"autoLayout");
        if (autoLayoutFunc) {
            CPyObject graphInfo = PyObject_CallObject(autoLayoutFunc.getObject(), PyTuple_Pack(2, PyUnicode_FromString((char*)QJsonDocument(graphInfoObject).toJson(QJsonDocument::Compact).toStdString().c_str()), PyUnicode_FromString((char*)QJsonDocument(autoLayoutInfoObject).toJson(QJsonDocument::Compact).toStdString().c_str())));
            if (graphInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(graphInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    graphInfoObject = doc.object();
            }
        }
    }
}
