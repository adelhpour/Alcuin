#include "negui_import_tools.h"
#include <QFileDialog>
#include <QIcon>
#include <QJsonObject>

// MyImportTool

MyImportTool::MyImportTool(const QString& name) : MyPluginItemBase(name) {
    _iconSize = QSize(75, 25);
}

const QString MyImportTool::type() const {
    return "importtool";
}

const QString MyImportTool::getOpenFileName() const {
    return QFileDialog::getOpenFileName(NULL, "Select the (." + fileExtension() + ") File", ".", fileExtension() + " files (*." + fileExtension() + ")");
}

const QString& MyImportTool::fileExtension() const {
    return _fileExtension;
}

const QIcon MyImportTool::icon() {
    QPixmap pixMap(iconDirectory());
    return QIcon(pixMap);
}

const QString& MyImportTool::iconDirectory() const {
    return _iconDirectory;
}

void MyImportTool::read(const QJsonObject &json) {
    // file extension
    if (json.contains("file-extension") && json["file-extension"].isString())
        _fileExtension = json["file-extension"].toString();
    
    // icon directory
    if (json.contains("icon-file") && json["icon-file"].isString())
        _iconDirectory = json["icon-file"].toString();
}

void MyImportTool::write(QJsonObject &json) {
    // name
    json["name"] = name();
    
    // extension
    json["file-extension"] = fileExtension();
    
    // icon directory
    json["icon-file"] = iconDirectory();
}
