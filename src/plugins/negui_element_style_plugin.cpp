#include "negui_element_style_plugin.h"
#include <QJsonDocument>

int ElementStylePlugin::initialize(const QString &appPath, const QString &pluginsPath) {
    _pyInstance = new CPyInstance(appPath.toStdString().c_str());
    _pyInstance->appendPath(pluginsPath.toStdString().c_str());
    _script.setObject(_pyInstance->importModule("negui_element_style_plugin"));
    if (_script)
        return 0;
    
    return -1;
}

const QJsonObject ElementStylePlugin::loadItemsInfo() {
    QJsonObject object;
    if (_pyInstance && _script) {
        CPyObject stylesInfoFunc = PyObject_GetAttrString(_script.getObject(), (char*)"elementStylesInfo");
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
