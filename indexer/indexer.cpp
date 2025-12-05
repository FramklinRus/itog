#include "indexer.h"

Indexer::Indexer(QObject *parent)
    : QObject{parent}
{



}

void Indexer::ReadIni(const QString &path)
{
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Indexer");

    QStringList lol;

    QString exts;
    cfg.path=settings.value("path").toString();
    exts = settings.value("extensions").toString();
    cfg.extensions=exts.split(" ", Qt::SkipEmptyParts);;





}

void Indexer::change_text()
{

    iterator = new QDirIterator(cfg.path,cfg.extensions,QDir::Files ,QDirIterator::Subdirectories);

    while (iterator->hasNext()) {
        QString dir = iterator->next();
        //qDebug() << dir;
        QFile file(dir);
        QString path;
        QString file_name;
        int size;
        QDateTime time;
        QFileInfo info(file);
        // Путь, имя файла, размер и время изменения
        file_name=info.fileName();
        path = info.path();
        size=info.size();
        time=info.lastModified();



        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

 QByteArray data = file.readAll();
        file.close();
        int count=0;
        freq.clear();
                 QString text;
            text = QString::fromUtf8(data).toLower();
            text.remove(QRegularExpression("[^a-z\\s]"));


            QRegularExpression re("(\\w+)");
            QRegularExpressionMatchIterator i = re.globalMatch(text);
            while (i.hasNext()) {
                QString word = i.next().captured();


                freq[word]++;
                //   qDebug() << word;
            }

            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                QTextStream out(&file);
                out << text;
                file.close();
            } else {
                qWarning() << "Не удалось открыть файл для перезаписи:" << dir;
            }



        emit sig_SendDataToDB(path, file_name, size, time, freq);

    }



}




