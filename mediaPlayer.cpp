#include "mediaPlayer.h"

Player::Player(void)
{
    output = new QAudioOutput;
    player = new QMediaPlayer;
    player->setAudioOutput(output);
    playing = false;
}
void Player::playSong(QString filePath)
{
    playing = true;
    player->setSource(QUrl::fromLocalFile(filePath));
    player->play();
}
void Player::togglePause()
{
    if (playing)
    {
        playing = false;
        player->pause();
    }
    else
    {
        playing = true;
        player->play();
    }
}
