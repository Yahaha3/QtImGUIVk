#include "CherileeCommon.h"
#include <QJsonDocument>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include <QDataStream>
#include <QFileDialog>

clz::CherileeCommon::CherileeCommon()
{

}

QJsonObject clz::CherileeCommon::read_json_object_file(const QString &path)
{
    QFile file(path);
    if (!QFileInfo(path).exists()) {
        QString err =
                QObject::tr("No such json file ")
                + "("+ path + ")";
        qDebug() << err;
    }
    if (!file.open(QIODevice::ReadOnly)) return QJsonObject();

    QByteArray qba = file.readAll();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(qba, &error);
    if (error.error != QJsonParseError::NoError)
    {
        int char_offset = error.offset;
        QByteArray qba_left = qba.left(char_offset);
        int row = qba_left.count("\n");
        int offset = char_offset - qba_left.lastIndexOf("\n");
        QString err =
                error.errorString()
                + "\n @row: " + QString::number(row) +
                + "\n @offset: " + QString::number(offset);
        qDebug() << err;
        return QJsonObject();
    }

    file.close();

    if (!document.isObject())
    {
        QString s;
        QTextStream ss(&s);
        ss << QObject::tr("The document is invalid json object.");
    }
    return document.object();
}

QJsonArray clz::CherileeCommon::read_json_array_file(const QString &path)
{
    QFile file(path);
    if (!QFileInfo(path).exists()) {
        QString err =
                QObject::tr("No such json file ")
                + "("+ path + ")";
        qDebug() << err;
    }
    if (!file.open(QIODevice::ReadOnly)) return QJsonArray();

    QByteArray qba = file.readAll();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(qba, &error);
    if (error.error != QJsonParseError::NoError)
    {
        int char_offset = error.offset;
        QByteArray qba_left = qba.left(char_offset);
        int row = qba_left.count("\n");
        int offset = char_offset - qba_left.lastIndexOf("\n");
        QString err =
                error.errorString()
                + "\n @row: " + QString::number(row) +
                + "\n @offset: " + QString::number(offset);
        qDebug() << err;
        return QJsonArray();
    }

    file.close();
    if (!document.isArray())
    {
        QString s;
        QTextStream ss(&s);
        ss << QObject::tr("The document is invalid jsonarray object.");
    }
    return document.array();
}

QJsonObject clz::CherileeCommon::qstring_to_json_object(const QString &string)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(string.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            return obj;
        }
    }
    else
    {
        return obj;
    }
    return obj;
}

QString clz::CherileeCommon::json_object_to_qstring(const QJsonObject &jo)
{
    QJsonDocument doc(jo);
    return QString(doc.toJson(QJsonDocument::Compact));
}

QStringList clz::CherileeCommon::json_array_to_stringlist(const QJsonArray &qja)
{
    QStringList list;
    for(auto qjo: qja){
        auto jo = qjo.toObject();
        list.append(json_object_to_qstring(jo));
    }
    return list;
}

QString clz::CherileeCommon::json_array_to_string(QJsonArray qja, bool compact)
{
    QJsonDocument doc(qja);
    if (compact) {
        return QString(doc.toJson(QJsonDocument::Compact));
    } else {
        return QString(doc.toJson(QJsonDocument::Indented));
    }
}

QJsonArray clz::CherileeCommon::string_to_json_array(QString string)
{
    QJsonArray array;
    QJsonDocument doc = QJsonDocument::fromJson(string.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isArray())
        {
            array = doc.array();
        }
        else
        {
            return array;
        }
    }
    else
    {
        return array;
    }
    return array;
}

QString clz::CherileeCommon::read_text_file(const QString &path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        return "";
    }
    QTextStream in(&f);
    return in.readAll();
}

void clz::CherileeCommon::write_text_file_object(const QString &path, const QString &text)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << text;
        file.close();
    }
}

void clz::CherileeCommon::append_log_to_file(const QString &path, const QString &log)
{
    QFile file(path);
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << log;
        file.close();
    }
}

void clz::CherileeCommon::write_csv_file(const QString &path, const QString &text)
{
    auto str = text + "\n";
    append_log_to_file(path, str);
}

void clz::CherileeCommon::json_join(QJsonObject &target, const QJsonObject &jo)
{
    for(auto key: jo.keys()){
        target.insert(key, jo[key]);
    }
}

QString clz::CherileeCommon::get_root_dir()
{
    return QCoreApplication::applicationDirPath();
}

QProcess *clz::CherileeCommon::start_process_detached(QString working_dir, QString name)
{
    QString aos_app_path;
 #if defined (Q_OS_WINDOWS)
     aos_app_path = QString("%1/%2").arg(working_dir).arg(name + ".exe");
 #elif defined (Q_OS_LINUX) || defined(Q_OS_MAC)
     aos_app_path = QString("%1/%2").arg(working_dir).arg(name);
 #endif
     QFileInfo qfi(aos_app_path);
     if (qfi.exists()) {
         QProcess* p = new QProcess();
         p->setProgram(aos_app_path);
         qint64 pid = 0;
         auto d = p->startDetached(&pid);
         if (!d) {
             std::cout << "eqnx-container (Detached) Failed to start." << std::endl;
             return nullptr;
         } else {
             return p;
         }
     } else {
         std::cout << "Failed to locate eqnx-container executable. @" << aos_app_path.toStdString() << std::endl;
         return nullptr;
     }

}

QStringList clz::CherileeCommon::generate_file_dialog_get_paths(const QString &title, const QString &filter, QWidget *parent)
{
    QFileDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setDirectory(".");
    dlg.setNameFilter(filter);
    dlg.setFileMode(QFileDialog::ExistingFiles);
    dlg.setViewMode(QFileDialog::Detail);

    // Execute this dialog
    int res = dlg.exec();
    if (!res) return QStringList();

    // Import kml files
    return dlg.selectedFiles();
}
