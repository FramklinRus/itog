#include <QCoreApplication>
#define DB_path "./bd.ini"
#define indexer_path "./indexer.ini"
#include "bd.h"
#include "indexer.h"

QString request = "CREATE TABLE IF NOT EXISTS documents ("
                  "id SERIAL PRIMARY KEY,"
                  "path TEXT NOT NULL,"
                  "name TEXT NOT NULL,"
                  "size INTEGER,"
                  "modified_at TIMESTAMP,"
                  "indexed_at TIMESTAMP DEFAULT NOW()"
                  ");";

QString request2 = "CREATE TABLE IF NOT EXISTS words ("
                   "id SERIAL PRIMARY KEY,"
                   "word TEXT NOT NULL UNIQUE,"
                   "total_count INTEGER DEFAULT 0"
                   ");";
QString request3 = "CREATE TABLE IF NOT EXISTS word_documents ("
                   "word_id INTEGER REFERENCES words(id) ON DELETE CASCADE,"
                   "document_id INTEGER REFERENCES documents(id) ON DELETE CASCADE,"
                   "frequency INTEGER NOT NULL,"
                   "PRIMARY KEY (word_id, document_id));";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BD * database;
    Indexer *ind;
    database = new BD();
    database->ConnectToDataBase(DB_path);
    database->Select(request);
    database->Select(request2);
    database->Select(request3);
    ind = new Indexer();
    ind->ReadIni(indexer_path);
    QObject::connect(ind, &Indexer::sig_SendDataToDB, [&database]( QString path,  QString name,
                                                                 int size,  QDateTime modified_at,  QMap<QString, int> word)
{
        database->SendData(path, name,  size, modified_at,  word);
});
    ind->change_text();
    int ret=a.exec();
    delete ind;
    delete database;

    return ret;
}
