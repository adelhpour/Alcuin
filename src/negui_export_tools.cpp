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

// MyDataExportTool

MyDataExportTool::MyDataExportTool(const QString& name) : MyExportToolBase(name) {
    _isInfoCompatible = false;
}

const QString MyDataExportTool::type() const {
    return "dataexporttool";
}

void MyDataExportTool::readCompatibilityInfo(const QJsonObject &json) {
    // compatibility
    if (json.contains("isInfoCompatible") && json["isInfoCompatible"].isBool())
        _isInfoCompatible = json["isInfoCompatible"].toBool();
    
    // messages
    _messages.clear();
    if (json.contains("messages") && json["messages"].isArray()) {
        QJsonArray messagesArray = json["messages"].toArray();
        for (int messageIndex = 0; messageIndex < messagesArray.size(); ++messageIndex) {
            QJsonObject messageObject = messagesArray[messageIndex].toObject();
            if (messageObject.contains("message") && messageObject["message"].isString())
                _messages.push_back(messageObject["message"].toString());
        }
    }
}

const QString MyDataExportTool::defaultUnsuccessfullExport() const {
    return "The graph info could not be exported.";
}

const QString MyDataExportTool::messagesAsString() const {
    QString messagesString;
    for (int messageIndex = 0; messageIndex < _messages.size(); ++messageIndex) {
        messagesString += "Message " + QString::number(messageIndex + 1) + ": ";
        messagesString += _messages.at(messageIndex);
        if (messageIndex != _messages.size() - 1)
            messagesString += "\n\n";
    }
    return messagesString;
}

void MyDataExportTool::showMessages() {
    if (!isInfoCompatible()) {
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setWindowTitle("Export Info");
        messageBox->setText(defaultUnsuccessfullExport());
        messageBox->setInformativeText(messagesAsString());
        messageBox->setIcon(QMessageBox::Critical);
        messageBox->exec();
    }
}

// MyPrintExportTool

MyPrintExportTool::MyPrintExportTool(const QString& name) : MyExportToolBase(name) {

}

const QString MyPrintExportTool::type() const {
    return "printexporttool";
}
