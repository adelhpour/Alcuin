#include "negui_data_export_plugin.h"
#include <QJsonDocument>

int DataExportPlugin::initialize(const QString &appPath, const QString &pluginsPath) {
    _pyInstance = new CPyInstance(appPath.toStdString().c_str());
    _pyInstance->appendPath(pluginsPath.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_data_export_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject DataExportPlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject dataExportInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"dataExportInfo");
        if (dataExportInfoFunc) {
            CPyObject dataExportInfo = PyObject_CallObject(dataExportInfoFunc.getObject(), PyTuple_Pack(0));
            if (dataExportInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(dataExportInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }
    
    return object;
}

void DataExportPlugin::writeGraphInfoToFile(const QJsonObject& graphInfoObject, const QString& fileName, const QString& fileType) {
    if (_pyInstance && _script) {
        CPyObject writeGraphInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"writeGraphInfoToFile");
        if (writeGraphInfoFunc)
            PyObject_CallObject(writeGraphInfoFunc.getObject(), PyTuple_Pack(3, PyUnicode_FromString((char*)QJsonDocument(graphInfoObject).toJson(QJsonDocument::Compact).toStdString().c_str()), PyUnicode_FromString((char*)fileName.toStdString().c_str()), PyUnicode_FromString((char*)fileType.toStdString().c_str())));
    }
}

const QJsonObject DataExportPlugin::checkForGraphInfoCompatibility(const QJsonObject& graphInfoObject, const QString& fileType) {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject checkForGraphInfoCompatibiliyFunc = PyObject_GetAttrString(_script.getObject(), (char*)"checkForGraphInfoCompatibiliy");
        if (checkForGraphInfoCompatibiliyFunc) {
            CPyObject graphInfoCompatibiliyResults = PyObject_CallObject(checkForGraphInfoCompatibiliyFunc.getObject(), PyTuple_Pack(2, PyUnicode_FromString((char*)QJsonDocument(graphInfoObject).toJson(QJsonDocument::Compact).toStdString().c_str()), PyUnicode_FromString((char*)fileType.toStdString().c_str())));
            if (graphInfoCompatibiliyResults) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(graphInfoCompatibiliyResults, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }
    
    return object;
}
