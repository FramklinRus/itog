#ifndef INDEXER_H
#define INDEXER_H

#include <QObject>
#include <QDirIterator>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QMap>
#include <QTime>
struct indexer {
    QString path;
    QStringList extensions;
};


class Indexer : public QObject
{
    Q_OBJECT
public:
    explicit Indexer(QObject *parent = nullptr);
    void ReadIni(const QString &path);
    void change_text();
    ~Indexer();
signals:
    void sig_SendDataToDB(QString path, QString name, int size, QDateTime modified_at, QMap<QString, int> word);
private:
    QDirIterator* iterator=nullptr;
    QMap <QString, int> freq;
    indexer cfg;
};

#endif // INDEXER_H
