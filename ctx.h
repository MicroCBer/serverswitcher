#ifndef CTX_H
#define CTX_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

#ifdef ANDROID
#include <QtAndroidExtras>
#endif
class ctx : public QObject
{
    Q_OBJECT
public:
    explicit ctx(QObject *parent = nullptr);
public slots:
    bool write(const QString& source, const QString& data)
    {
        if(requestPermission()){
            if (source.isEmpty())
                return false;
            QFile file(source);
            qDebug()<<file.exists();
            if (!file.open(QFile::WriteOnly | QFile::Truncate))
                return false;
            qDebug()<<"Opened";
            QTextStream out(&file);
            out << data;
            file.close();
            return true;
        }
        return false;
    }
    bool remove(const QString& source){
        if(requestPermission()){
            if (source.isEmpty())
                return false;
            QFile file(source);
            file.remove();
            return true;
        }
    }
    QString read(const QString& source){
        if(requestPermission()){
            if (source.isEmpty())
                return "";
            QFile file(source);
            qDebug()<<file.exists();
            file.open(QFile::ReadOnly);
            return QString(file.readAll());
        }
        return "";
    }
    bool exists(const QString& source){
        if (source.isEmpty())return false;
        QFile file(source);
        if(requestPermission())return file.exists();
        return false;
    }
    QList<QString> files(const QString& source){
        QList<QString> empty;
        if (source.isEmpty())return empty;
        QDir dir(source);
        if(requestPermission())return dir.entryList();
//        dir.
        return empty;
    }

signals:

private:

    bool requestPermission()
    {
        #ifdef ANDROID
        QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
            QtAndroid::requestPermissionsSync(QStringList()<<"android.permission.WRITE_EXTERNAL_STORAGE");
            r = QtAndroid::checkPermission("android.permission.CAMERA");
            if(r == QtAndroid::PermissionResult::Denied) {
                return false;
            }
        }
#endif
        return true;
    }
};

#endif // CTX_H
