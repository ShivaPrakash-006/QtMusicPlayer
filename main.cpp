#include "mainwindow.h"
#include <QApplication>
#include"mediaPlayer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Player p1;
    p1.playSong("C:/Users/vadhavarshini/Downloads/Monica - Lyric Video_ COOLIE _ Superstar Rajinikanth _ Sun Pictures _ Lokesh _ Anirudh _ Pooja Hegde.mp3");
    return a.exec();
}
