#include "soundmanager.h"

SoundManager::SoundManager() : QObject()
{
    QString soundsPath(":/sounds/new_message");
    if (QFile(QCoreApplication::applicationDirPath() + "/resources/sounds/newMessage.wav").exists())
        soundsPath = QCoreApplication::applicationDirPath() + "/resources/sounds/newMessage.wav";
    sounds.insert(SoundManager::newMessage, new QSound(soundsPath));
}

SoundManager::~SoundManager()
{

}

void SoundManager::play(SoundManager::Name name)
{
    if (!mute)
        sounds.value(name)->play();
}

void SoundManager::stop(SoundManager::Name name)
{
    sounds.value(name)->stop();
}

void SoundManager::setMute(bool b)
{
    mute = b;
}
