#ifndef __NEGUI_FILE_MANAGER_H
#define __NEGUI_FILE_MANAGER_H

#include <QObject>

class MyFileManager : public QObject {
    Q_OBJECT

public:
    MyFileManager();

    void reset();

    void resetCurrentFileName();

    void setCurrentFileName(const QString& fileName);

    const QString& currentFileName();

    const QString currentBaseFileName();

    void setWorkingDirectoryPath(const QString& workingDirectoryPath);

    const QString& workingDirectoryPath();

    void resetLastSavedFileName();

    void setLastSavedFileName(const QString& fileName);

    const QString lastSavedFileName();

    const QString createDefaultFileName();

    const bool canSaveCurrentNetwork();

    const bool isWillingToSaveCurrentNetwork();

    const bool isCurrentNetworkUnsaved();

    void setCurrentNetworkUnsaved(const bool& currentNetworkUnsaved);

    const QString getOpenFileName(const QString& fileExtension);

    const QString getSaveFileName(const QString& defaultFileExtension);

    const QString getSaveAsFileName(const QString& fileExtension);

    const QString getSaveFigureFileName(const QString& fileExtension);

signals:

    QJsonObject askForNetworkInfo();
    void currentFileNameIsUpdated(const QString&);

protected:
    QString _currentFileName;
    QString _workingDirectoryPath;
    QString _lastSavedFileName;
    qint32 _defaultFileNameIndex;
    bool _isCurrentNetworkUnsaved;
};

#endif
