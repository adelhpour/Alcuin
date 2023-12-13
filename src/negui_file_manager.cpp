#include "negui_file_manager.h"
#include "negui_customized_common_widgets.h"

#include <QFileInfo>
#include <QFileDialog>

// MyFileManager

MyFileManager::MyFileManager() {
    _defaultFileNameIndex = 0;
    _isCurrentNetworkUnsaved = false;
}

void MyFileManager::reset() {;
    resetCurrentFileName();
    resetLastSavedFileName();
    _isCurrentNetworkUnsaved = false;
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

const QString MyFileManager::getSaveFigureFileName(const QString& fileExtension) {
    QString fileName = QFileDialog::getSaveFileName(NULL, "Save (." + fileExtension + ") File", workingDirectoryPath() + "/" + currentBaseFileName(), "(*." + fileExtension + ")");
    if (!fileName.isEmpty())
        setWorkingDirectoryPath(QFileInfo(fileName).absolutePath() + "/");

    return fileName;
}
