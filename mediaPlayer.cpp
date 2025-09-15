#include"mediaPlayer.h"
Player::Player(void)
{
    output=new QAudioOutput;
    player=new QMediaPlayer;
    player->setAudioOutput(output);
}
void Player:: playSong(QString filePath)
{
    player->setSource(QUrl::fromLocalFile(filePath));
    player->play();
}
