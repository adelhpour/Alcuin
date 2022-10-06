#include "negui_node_style_plugin.h"

int NodeStylePlugin::initialize(const QString &path) {
    _path = path;
    _pyInstance = new CPyInstance();
    _pyInstance->appendPath(_path.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_node_style_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject NodeStylePlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject stylesInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"nodeStylesInfo");
        if (stylesInfoFunc) {
            CPyObject stylesInfo = PyObject_CallObject(stylesInfoFunc.getObject(), PyTuple_Pack(0));
            if (stylesInfo) {
                QJsonDocument doc = QJsonDocument::fromJson(QString(PyBytes_AsString(PyUnicode_AsEncodedString(stylesInfo, "utf-8", "~E~"))).toUtf8());
                if (!doc.isNull())
                    object = doc.object();
            }
        }
    }
    
    return object;
}
