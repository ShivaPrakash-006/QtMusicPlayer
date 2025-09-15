#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H
#include <QCoreApplication>
#include <QMediaPlayer>
#include <QTimer>
#include <QDebug>
#include <QAudioOutput>
class Player
{
protected:
    QMediaPlayer *player;
    QAudioOutput *output;
public:
    Player();
    void playSong(QString filePath);
};
#endif // MEDIAPLAYER_H

