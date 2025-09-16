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
    bool playing;
public:
    Player();
    void playSong(QString filePath);
    void togglePause();
};
#endif // MEDIAPLAYER_H

