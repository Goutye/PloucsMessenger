#ifndef EMOTICONS_H
#define EMOTICONS_H
#include <QMap>
#include <QPixmap>
#include <QTextCursor>
#include <QObject>
#include "filedownloader.h"

#define EMOTICON_SIZE 32

class Emoticons : public QObject
{
    Q_OBJECT
public:
    explicit Emoticons(QObject *parent = 0);
    ~Emoticons();
    QPixmap* emote(QString name);
    bool exist(QString name);
    void convertShortcutToId(QString *message);
    void loadEmoticon(QString name, QTextCursor c);

private slots:
    void registerEmoticon(QString name, QPixmap *image);
    void isDownloaded();

private:
    void load();
    void createShortcuts();

    FileDownloader *fd;
    QMap<QString, QPixmap*> emotes;
    QMap<QString, QString> shortcutToIds;
};

#endif // EMOTICONS_H
