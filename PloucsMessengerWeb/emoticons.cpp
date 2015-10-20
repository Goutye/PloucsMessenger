#include <qDebug>
#include <QStringList>
#include "emoticons.h"
#include "emoticondownloader.h"
#include <QDir>
#include <QCoreApplication>
#include <QSettings>

Emoticons::Emoticons(QObject *parent) : QObject(parent)
{
    if (!QDir(QCoreApplication::applicationDirPath() + "/resources/emoticons").exists()) {
        QDir(QCoreApplication::applicationDirPath() + "/resources").mkdir("emoticons");
    }
    if (!QFile(QCoreApplication::applicationDirPath() + "/resources/user_shortcuts.ini").exists()) {
        fd = new FileDownloader(QUrl("http://www.goutye.com/ploucs/user_shortcuts.ini"), this);
        connect(fd, SIGNAL(downloaded()), this, SLOT(isDownloaded()));
    } else {
        load();
    }
}

Emoticons::~Emoticons()
{
    for (QMap<QString, QPixmap*>::iterator it = emotes.begin(); it != emotes.end(); ++it) {
        delete it.value();
    }
}

void Emoticons::isDownloaded()
{
    QFile file(QCoreApplication::applicationDirPath() + "/resources/user_shortcuts.ini");
    file.open(QFile::WriteOnly);
    file.write(fd->downloadedData());
    file.close();
    load();
    delete fd;
}

void Emoticons::createShortcuts()
{
    QSettings settingsUser(QCoreApplication::applicationDirPath() + "/resources/user_shortcuts.ini", QSettings::IniFormat);
    QDir emotesDir;
    emotesDir.setPath(QCoreApplication::applicationDirPath() + "/resources/emoticons");
    int i = 0;
    QStringList sl = emotesDir.entryList();
    for (QStringList::iterator it = sl.begin(); it != sl.end(); ++it) {
        if (it->endsWith(".gif")) {
            QFile img(QCoreApplication::applicationDirPath() + "/resources/emoticons/" + *it);
            it->remove(it->count()-4, 4);
            settingsUser.setValue(QString(":") + *it + ":", i);
            img.rename(QCoreApplication::applicationDirPath() + QString("/resources/emoticons/%1.gif").arg(i));
            ++i;
        }
    }
}

void Emoticons::load()
{
    QDir emotesDir;
    emotesDir.setPath(QCoreApplication::applicationDirPath() + "/resources/emoticons");

    QStringList sl = emotesDir.entryList();
    for (QStringList::iterator it = sl.begin(); it != sl.end(); ++it) {
        if (it->endsWith(".gif")) {
            QPixmap *emote = new QPixmap(QCoreApplication::applicationDirPath() + "/resources/emoticons/" + *it);
            if (emote->width() > EMOTICON_SIZE || emote->height() > EMOTICON_SIZE) {
                //*emote = emote->scaled(EMOTICON_SIZE, EMOTICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            emotes.insert(*it, emote);
        }
    }

    QSettings settingsUser(QCoreApplication::applicationDirPath() + "/resources/user_shortcuts.ini", QSettings::IniFormat);
    sl = settingsUser.allKeys();
    for (QStringList::iterator it = sl.begin(); it != sl.end(); ++it) {
        shortcutToIds.insert(*it, settingsUser.value(*it).toString());
    }
}

bool Emoticons::exist(QString name)
{
    return emotes.contains(name + ".gif");
}

QPixmap* Emoticons::emote(QString name)
{
    return emotes.value(name + ".gif"); //QMap of String <-> Gif/Png/Jpg
}

void Emoticons::convertShortcutToId(QString *message)
{
    for (QMap<QString, QString>::iterator it = shortcutToIds.begin(); it != shortcutToIds.end(); ++it) {
        message->replace(it.key(), QString(":") + it.value() + ":");
    }

    qDebug() << *message;
}

void Emoticons::registerEmoticon(QString name, QPixmap *image) {
    emotes.insert(name, image);
}

void Emoticons::loadEmoticon(QString name, QTextCursor c)
{
    EmoticonDownloader *emDownloader = new EmoticonDownloader(name + ".gif", c, this); //QMap of String <-> Gif/Png/Jpg
    connect(emDownloader, SIGNAL(downloaded(QString,QPixmap*)), this, SLOT(registerEmoticon(QString,QPixmap*)));
}
