#include "bd.h"

BD::BD(QObject *parent)
    : QObject{parent}
{


    dataBase = QSqlDatabase::addDatabase("QPSQL", "searcher");




}





bool BD::ConnectToDataBase(const QString &path)
{

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("DB");
    qDebug() << "all" << settings.childGroups();

    DbConfig cfg;
    cfg.host = settings.value("host").toString();
    cfg.port = settings.value("port").toInt();
    cfg.name = settings.value("database").toString();
    cfg.user = settings.value("user").toString();
    cfg.password = settings.value("password").toString();

    settings.endGroup();


    dataBase.setHostName(cfg.host);
    dataBase.setPort(cfg.port);
    dataBase.setDatabaseName(cfg.name);
    dataBase.setUserName(cfg.user);
    dataBase.setPassword(cfg.password);




    if (!dataBase.open()) {
        qCritical() << "no:" << dataBase.lastError();
        return false;


    }

    qDebug() << "yes!";
    return true;
}

void BD::Select(const QString &request)
{

    read = new QSqlQuery(dataBase);

 // read->exec("DROP TABLE IF EXISTS word_documents;");
  // read->exec("DROP TABLE IF EXISTS words;");
  // read->exec("DROP TABLE IF EXISTS documents;");
read->exec(request);
    if (!read->isActive())
    {
        qDebug()<<"no";
    }
    else
    {
        qDebug()<<"yes";
    }

}

void BD::SendData(QString path, QString name, int size, QDateTime modified_at, QMap<QString, int> word)
{
    QSqlQuery del(dataBase);
    del.prepare("DELETE FROM word_documents WHERE document_id = (SELECT id FROM documents WHERE path = :path AND name = :name)");
    del.bindValue(":path", path);
    del.bindValue(":name", name);
    del.exec();

    del.prepare("DELETE FROM documents WHERE path = :path AND name = :name");
    del.bindValue(":path", path);
    del.bindValue(":name", name);
    del.exec();



    doc = new QSqlQuery(dataBase);
    doc->prepare("INSERT INTO documents (path, name, size, modified_at) "
                 "VALUES (:path, :name, :size, :modified_at) "
                 "RETURNING id");

    doc->bindValue(":path", path);
    doc->bindValue(":name", name);
    doc->bindValue(":size", size);
    doc->bindValue(":modified_at", modified_at);

    if(! doc->exec()) {
        qDebug() << "Insert document error:" << doc->lastError();
        return;
    }
    int documentId = -1;
    if(doc->next())
        documentId = doc->value(0).toInt();

    for(auto it = word.begin(); it != word.end(); ++it) {
        QString word = it.key();
        int frequency = it.value();

        wordBD = new QSqlQuery(dataBase);
        wordBD->prepare("INSERT INTO words (word, total_count) "
                        "VALUES(:word, :count) "
                        "ON CONFLICT(word) DO UPDATE "
                        "SET total_count = words.total_count + EXCLUDED.total_count "
                        "RETURNING id");

        wordBD->bindValue(":word", word);
        wordBD->bindValue(":count", frequency);

        if(!wordBD->exec()) {
            qDebug() << "Insert word error:" <<wordBD->lastError();
            continue;
        }

        int wordId = -1;
        if(wordBD->next())
            wordId = wordBD->value(0).toInt();

        doc_word = new QSqlQuery(dataBase);
        doc_word->prepare("INSERT INTO word_documents "
                          "(word_id, document_id, frequency) "
                          "VALUES (:word_id, :doc_id, :freq)");

        doc_word->bindValue(":word_id", wordId);
        doc_word->bindValue(":doc_id", documentId);
        doc_word->bindValue(":freq", frequency);

        if(!doc_word->exec())
            qDebug() << "Insert wd error:" << doc_word->lastError();
    }
}






