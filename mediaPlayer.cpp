#include "mediaPlayer.h"
#include "databaseManager.h"
Player::Player(void)
{
    output = new QAudioOutput;
    player = new QMediaPlayer;
    player->setAudioOutput(output);
    playing = false;
}
void Player::setCurrentSong(int songId) {
    currentSongId = songId;
}
int Player::getCurrentSong() {return currentSongId;}
void Player::setPosition(qint64 position) {
    player->setPosition(position);
}
QMediaPlayer* Player::getMediaPlayer() {
    return player;
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
