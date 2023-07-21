#include "negui_import_plugin.h"
#include <QJsonDocument>

int ImportPlugin::initialize(const QString &appPath, const QString &pluginsPath) {
    _pyInstance = new CPyInstance(appPath.toStdString().c_str());
    _pyInstance->appendPath(pluginsPath.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_import_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject ImportPlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject importInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"importInfo");
        if (importInfoFunc) {
            CPyObject importInfo = PyObject_CallObject(importInfoFunc.getObject(), PyTuple_Pack(0));
            if (importInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(importInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }
    
    return object;
}

const QJsonObject ImportPlugin::readGraphInfoFromFile(const QString& fileName, const QString& fileType) {
    QJsonObject graphInfoObject;
    if (_pyInstance && _script) {
        CPyObject readGraphInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"readGraphInfoFromFile");
        if (readGraphInfoFunc) {
            CPyObject graphInfo = PyObject_CallObject(readGraphInfoFunc.getObject(), PyTuple_Pack(2, PyUnicode_FromString((char*)fileName.toStdString().c_str()), PyUnicode_FromString((char*)fileType.toStdString().c_str())));
            if (graphInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(graphInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    graphInfoObject = doc.object();
            }
        }
    }
    
    return graphInfoObject;
}
