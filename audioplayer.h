#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlaylist>

class QMediaPlayer;

class AudioPlayer : public QObject
{
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    void startBGM();
    void startBoss();
    void setVolume(int v);

private:
    QMediaPlayer *m_backgroundMusic; // 只用来播放背景音乐
    QMediaPlaylist *m_backgroundMusicList1;
    QMediaPlaylist *m_backgroundMusicList2;
};

#endif // AUDIOPLAYER_H
