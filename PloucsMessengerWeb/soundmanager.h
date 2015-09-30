#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QWidget>
#include <QSound>
#include <QMap>
#include <QtCore>

class SoundManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(Name)
public:
    enum Name {
        newMessage
    };

    SoundManager();
    ~SoundManager();

public slots:
    void play(SoundManager::Name name);
    void stop(SoundManager::Name name);
    void setMute(bool b);

private:
    QMap<SoundManager::Name, QSound *> sounds;
    bool mute = false;
};

Q_DECLARE_METATYPE(SoundManager::Name)

#endif // SOUNDMANAGER_H
