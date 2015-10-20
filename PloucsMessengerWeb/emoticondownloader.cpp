#include "emoticondownloader.h"
#include <QFile>
#include <QCoreApplication>

EmoticonDownloader::EmoticonDownloader(QString name, QTextCursor c, QObject *parent) : QObject(parent), c(c), name(name)
{
    QUrl url("http://www.goutye.com/ploucs/emoticons/" + name);
    position = c.position();
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)), this, SLOT (fileDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
    qDebug() << "dl launched at: " << url.toString();
}

EmoticonDownloader::~EmoticonDownloader()
{
}

void EmoticonDownloader::fileDownloaded(QNetworkReply* pReply) {
    if (pReply->error() != QNetworkReply::NoError) {
        qDebug() << name << pReply->errorString();
        c.setPosition(position);
        name.remove(name.count() - 4, 4);
        c.insertHtml(QString("<span style='font-family: Roboto;font-size:15px;'>:") + name + ":</span>");
        return;
    }
    m_DownloadedData = pReply->readAll();
    qDebug() << "Download emoticon successful";
    pReply->deleteLater();
    image = new QPixmap();
    image->loadFromData(m_DownloadedData);
    c.setPosition(position);
    c.insertImage(image->toImage());

    QFile file(QCoreApplication::applicationDirPath() + "/resources/emoticons/" + name);
    file.open(QFile::WriteOnly);
    file.write(m_DownloadedData);
    file.close();

    emit downloaded(name, image);
}

QPixmap* EmoticonDownloader::downloadedData() const {
    return image;
}
