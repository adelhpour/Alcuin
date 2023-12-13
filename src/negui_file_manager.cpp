#include "negui_file_manager.h"
#include "negui_customized_common_widgets.h"
#include "negui_export_tools.h"

#include <QFileInfo>
#include <QFileDialog>

// MyFileManager

MyFileManager::MyFileManager(QList<MyPluginItemBase*> importTools, QList<MyPluginItemBase*> exportTools) {
    _importTools = importTools;
    _exportTools = exportTools;
    _defaultFileNameIndex = 0;
    _isCurrentNetworkUnsaved = false;
}

void MyFileManager::reset() {
    resetCurrentExportTool();
    resetCurrentFileName();
    resetLastSavedFileName();
    _isCurrentNetworkUnsaved = false;
}

QList<MyPluginItemBase*> MyFileManager::importTools() {
    return _importTools;
}

QList<MyPluginItemBase*> MyFileManager::exportTools() {
    return _exportTools;
}

void MyFileManager::resetCurrentFileName() {
    setCurrentFileName(createDefaultFileName());
}

void MyFileManager::setCurrentFileName(const QString& fileName) {
    _currentFileName = fileName;
    emit currentFileNameIsUpdated(_currentFileName);
}

const QString& MyFileManager::currentFileName() {
    return _currentFileName;
}

const QString MyFileManager::currentBaseFileName() {
    return QFileInfo(currentFileName()).completeBaseName();
}

void MyFileManager::setWorkingDirectoryPath(const QString& workingDirectoryPath) {
    _workingDirectoryPath = workingDirectoryPath;
}

const QString& MyFileManager::workingDirectoryPath() {
    return _workingDirectoryPath;
}

void MyFileManager::resetCurrentExportTool() {
    if (_exportTools.size())
        setCurrentExportTool(_exportTools.at(0));
    else
        setCurrentExportTool(NULL);
}

void MyFileManager::setCurrentExportTool(MyPluginItemBase* exportTool) {
    _currentExportTool = exportTool;
}

void MyFileManager::setCurrentExportToolCompatibleWithImportTool(MyPluginItemBase* importTool) {
    /*
    for (MyPluginItemBase* exportTool : exportTools()) {
        if (((MyExportToolBase*)exportTool)->fileExtension() == ((MyImportTool*)importTool)->fileExtension()) {
            setCurrentExportTool(exportTool);
            return;
        }
    }
     */
}

MyPluginItemBase* MyFileManager::currentExportTool() {
    return _currentExportTool;
}

void MyFileManager::resetLastSavedFileName() {
    setLastSavedFileName("");
}

void MyFileManager::setLastSavedFileName(const QString& fileName) {
    _lastSavedFileName = fileName;
    _isCurrentNetworkUnsaved = false;
}

const QString MyFileManager::lastSavedFileName() {
    return _lastSavedFileName;
}

const QString MyFileManager::createDefaultFileName() {
    return "network" + QString::number(++_defaultFileNameIndex);
}

const bool MyFileManager::canSaveCurrentNetwork() {
    if (isCurrentNetworkUnsaved())
        return isWillingToSaveCurrentNetwork();

    return false;
}

const bool MyFileManager::isWillingToSaveCurrentNetwork() {
    QMessageBox* autoSaveMessageBox =  new MyAutoSaveMessageBox(currentFileName());
    if (autoSaveMessageBox->exec() == QMessageBox::Yes)
        return true;

    return false;
}

const bool MyFileManager::isCurrentNetworkUnsaved() {
    return _isCurrentNetworkUnsaved;
}

void MyFileManager::setCurrentNetworkUnsaved(const bool& currentNetworkUnsaved) {
    _isCurrentNetworkUnsaved = currentNetworkUnsaved;
}

const QString MyFileManager::getOpenFileName(const QString& fileExtension) {
    QString fileName = QFileDialog::getOpenFileName(NULL, "Select the (." + fileExtension + ") File", workingDirectoryPath(), fileExtension + " files (*." + fileExtension + ")");
    if (!fileName.isEmpty()) {
        setCurrentFileName(fileName);
        setLastSavedFileName(fileName);
        setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");
    }

    return fileName;
}

const QString MyFileManager::getSaveFileName(const QString& defaultFileExtension) {
    if (isCurrentNetworkUnsaved()) {
        if (lastSavedFileName().isEmpty())
            return getSaveAsFileName(defaultFileExtension);
        else
            return lastSavedFileName();
    }

    return "";
}

const QString MyFileManager::getSaveAsFileName(const QString& fileExtension) {
    QString fileName = QFileDialog::getSaveFileName(NULL, "Save (." + fileExtension + ") File", workingDirectoryPath() + "/" + currentBaseFileName(), "(*." + fileExtension + ")");
    if (!fileName.isEmpty()) {
        setCurrentFileName(fileName);
        setLastSavedFileName(fileName);
        setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");
    }

    return fileName;
}

MyPluginItemBase* MyFileManager::getSavePlugin(QList<MyPluginItemBase*> plugins) {
    QList<MyPluginItemBase*> savePlugins = getPluginsOfType(plugins, "datasavetool");
    if (savePlugins.size())
        return savePlugins.at(0);

    return NULL;
}


MyPluginItemBase* getDataExportTool(QList<MyPluginItemBase*> dataExportTools, const QString& fileName) {
    QStringList fileNameSections = fileName.split(".");
    if (fileNameSections.size() > 1) {
        for (MyPluginItemBase* dataExportTool : dataExportTools) {
            if (fileNameSections.at(1) == ((MyDataExportTool*)dataExportTool)->fileExtension())
                return dataExportTool;
        }
    }
    if (dataExportTools.size())
        return dataExportTools.at(0);

    return NULL;
}