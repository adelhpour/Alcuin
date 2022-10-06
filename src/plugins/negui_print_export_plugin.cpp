#include "negui_print_export_plugin.h"

int PrintExportPlugin::initialize(const QString &path) {
    _path = path;
    _pyInstance = new CPyInstance();
    _pyInstance->appendPath(_path.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_print_export_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject PrintExportPlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject printExportInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"printExportInfo");
        if (printExportInfoFunc) {
            CPyObject printExportInfo = PyObject_CallObject(printExportInfoFunc.getObject(), PyTuple_Pack(0));
            if (printExportInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(printExportInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }
    
    return object;
}
