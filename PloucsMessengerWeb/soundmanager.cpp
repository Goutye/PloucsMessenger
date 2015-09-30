#include "soundmanager.h"

SoundManager::SoundManager() : QObject()
{
    sounds.insert(SoundManager::newMessage, new QSound(QCoreApplication::applicationDirPath() + "/resources/sounds/newMessage.wav"));
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
