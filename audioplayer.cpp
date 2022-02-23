#include "audioplayer.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>

AudioPlayer::AudioPlayer(QObject *parent)
    :QObject (parent)
{
    QString s_curDir = QDir::currentPath() + "/";
    QUrl backgroundMusicUrl1 = QUrl::fromLocalFile(s_curDir + "theme.mp3");
    QUrl backgroundMusicUrl2 = QUrl::fromLocalFile(s_curDir + "boss.mp3");
//    QUrl backgroundMusicUrl1 = QUrl::fromLocalFile("D:/QTproject/TowerDefence/sound/theme.mp3");
//    QUrl backgroundMusicUrl2 = QUrl::fromLocalFile("D:/QTproject/TowerDefence/sound/boss.mp3");
    //WDNMD 傻逼绝对路径
    m_backgroundMusic = new QMediaPlayer(this);
    m_backgroundMusicList1 = new QMediaPlaylist();
    m_backgroundMusicList2 = new QMediaPlaylist();
    QMediaContent media1(backgroundMusicUrl1);
    QMediaContent media2(backgroundMusicUrl2);
    m_backgroundMusicList1->addMedia(media1);
    m_backgroundMusicList2->addMedia(media2);
    m_backgroundMusicList1->setCurrentIndex(0);
    m_backgroundMusicList2->setCurrentIndex(0);
    // 设置背景音乐循环播放
    m_backgroundMusicList1->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_backgroundMusicList2->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

void AudioPlayer::startBGM()
{
    m_backgroundMusic->stop();
    m_backgroundMusic->setPlaylist(m_backgroundMusicList1);
    m_backgroundMusic->setVolume(100);
    m_backgroundMusic->play();
}

void AudioPlayer::startBoss()
{
    m_backgroundMusic->stop();
    m_backgroundMusic->setPlaylist(m_backgroundMusicList2);
    m_backgroundMusic->setVolume(100);
    m_backgroundMusic->play();
}

void AudioPlayer::setVolume(int v)
{
    m_backgroundMusic->setVolume(v);
}
