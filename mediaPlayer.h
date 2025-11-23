#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H
#include <QCoreApplication>
#include <QMediaPlayer>
#include <QAudioOutput>

class Player
{
protected:
    QMediaPlayer *player;
    QAudioOutput *output;
    bool playing;
    int currentSongId;
public:
    Player();
    void setCurrentSong(int song_id);
    int getCurrentSong();
    void playSong(QString filePath);
    void togglePause();
};

#endif // MEDIAPLAYER_H

