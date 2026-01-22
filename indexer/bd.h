#ifndef BD_H
#define BD_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
struct DbConfig {
    QString host;
    int port;
    QString name;
    QString user;
    QString password;
};

class BD : public QObject
{
    Q_OBJECT
public:
    explicit BD(QObject *parent = nullptr);
    bool ConnectToDataBase(const QString &path);
    void Select(const QString &request);
    void SendData(QString path, QString name, int size, QDateTime modified_at, QMap<QString, int> word);
    ~BD();
signals:
private:
    QSqlDatabase dataBase;
    QSqlQuery* read =nullptr;
    QSqlQuery* doc =nullptr;
    QSqlQuery* wordBD =nullptr;
    QSqlQuery* doc_word =nullptr;

};

#endif // BD_H
