#include "negui_export_tools.h"
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

// MyExportToolBase

MyExportToolBase::MyExportToolBase(const QString& name) : MyPluginItemBase(name) {
    _iconSize = QSize(75, 25);
}

const QString MyExportToolBase::getSaveFileName(const QString& workingDirectory, const QString& saveFileName) const {
    return QFileDialog::getSaveFileName(NULL, "Save (." + fileExtension() + ") File", workingDirectory + "/" + saveFileName, "(*." + fileExtension() + ")");
}

const QString MyExportToolBase::getSaveFileName(const QString& workingDirectory) const {
    return QFileDialog::getSaveFileName(NULL, "Save (." + fileExtension() + ") File", workingDirectory + defaultSaveFileName(), "(*." + fileExtension() + ")");
}

const QString& MyExportToolBase::fileExtension() const {
    return _fileExtension;
}

const QString& MyExportToolBase::defaultSaveFileName() const {
    return _defaultSaveFileName;
}

void MyExportToolBase::read(const QJsonObject &json) {
    MyPluginItemBase::read(json);
    // file extension
    if (json.contains("file-extension") && json["file-extension"].isString())
        _fileExtension = json["file-extension"].toString();
    
    // icon directory
    if (json.contains("icon-file") && json["icon-file"].isString())
        _iconDirectory = json["icon-file"].toString();
    
    // default save file name
    if (json.contains("default-save-file-name") && json["default-save-file-name"].isString())
        _defaultSaveFileName = json["default-save-file-name"].toString();
}

void MyExportToolBase::write(QJsonObject &json) {
    MyPluginItemBase::write(json);
    // name
    json["name"] = name();
    
    // extension
    json["file-extension"] = fileExtension();
    
    // icon directory
    json["icon-file"] = iconDirectory();
    
    // default save file name
    json["default-save-file-name"] = defaultSaveFileName();
}

// MyPrintExportTool

MyPrintExportTool::MyPrintExportTool(const QString& name) : MyExportToolBase(name) {

}

const QString MyPrintExportTool::type() const {
    return "printexporttool";
}
