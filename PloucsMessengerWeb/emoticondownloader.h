#ifndef EMOTICONDOWNLOADER_H
#define EMOTICONDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCursor>

class EmoticonDownloader : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonDownloader(QString name, QTextCursor c, QObject *parent = 0);
    ~EmoticonDownloader();
    QPixmap* downloadedData() const;

signals:
    void downloaded(QString name, QPixmap *image);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
    QPixmap *image;
    QTextCursor c;
    int position;
    QString name;
};

#endif // EMOTICONDOWNLOADER_H
