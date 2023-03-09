#ifndef CHERILEECOMMON_H
#define CHERILEECOMMON_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>

namespace clz {

class CherileeCommon
{
public:
    CherileeCommon();

    static QJsonObject read_json_object_file(const QString& path);
    static QJsonArray read_json_array_file(const QString& path);

    static QJsonObject qstring_to_json_object(const QString& string);
    static QString json_object_to_qstring(const QJsonObject& jo);

    static QStringList json_array_to_stringlist(const QJsonArray& qja);
    static QString json_array_to_string(QJsonArray qja, bool compact = true);
    static QJsonArray string_to_json_array(QString string);

    static QString read_text_file(const QString& path);

    static void write_text_file_object(const QString &path, const QString &text);

    static void append_log_to_file(const QString& path, const QString& log);

    static void write_csv_file(const QString &path, const QString & text);

    static void json_join(QJsonObject& target, const QJsonObject& jo);

    static QString get_root_dir();

    static QProcess* start_process_detached(QString path, QString name);

    static QStringList generate_file_dialog_get_paths(const QString& title, const QString& filter, QWidget* parent = nullptr);
};

}


#endif // CHERILEECOMMON_H
