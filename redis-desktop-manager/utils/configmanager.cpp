#include "configmanager.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

QString ConfigManager::getApplicationConfigPath(const QString &configFile)
{
#ifdef Q_OS_MACX
    QString libraryDir = "/Library/Application Support/rdm/";
    QDir libraryPath(libraryDir);
    libraryPath.mkpath("/Library/Application Support/rdm/");

    QString configDir;

    if (libraryPath.exists()) {
        configDir = libraryDir;
    } else {
        configDir = QDir::homePath();
    }
#else
    QString configDir = QDir::homePath();
#endif

    QString configPath = QString("%1/%2").arg(configDir).arg(configFile);

    if (!chechPath(configPath))
        return QString();

    return configPath;
}


bool ConfigManager::chechPath(const QString &configPath)
{
    QFile testConfig(configPath);
    QFileInfo checkPermissions(configPath);

    if (!testConfig.exists() && testConfig.open(QIODevice::ReadWrite))
        testConfig.close();

    if (checkPermissions.isWritable()) {
        setPermissions(testConfig);
        return true;
    }

    return false;
}

void ConfigManager::setPermissions(QFile &file)
{
#ifdef Q_OS_WIN
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    qt_ntfs_permission_lookup++;
#endif

    if (file.setPermissions(QFile::ReadUser|QFile::WriteUser))
        qDebug() << "Permission changed";

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif
}
