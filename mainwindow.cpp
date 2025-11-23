#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QTableWidget>
#include "databaseManager.h"
#include "mediaPlayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    database = initConnectDatabase();
    player = new Player;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSqlDatabase* MainWindow::getDatabase() {
    return &database;
}

void MainWindow::loadSongTable(QTableWidget *table, QString tableName) {
    database.open();
    QSqlQuery songFetcher;
    songFetcher.exec("SELECT song_id, title, artist, album FROM song_data WHERE song_id IN (SELECT song_id FROM" + tableName + ")");
    int row_no = 0;

    while (songFetcher.next()) {
        table->setItem(row_no, 0, new QTableWidgetItem(songFetcher.value(0).toString()));
        table->setItem(row_no, 1, new QTableWidgetItem(songFetcher.value(1).toString()));
        table->setItem(row_no, 2, new QTableWidgetItem(songFetcher.value(2).toString()));
        table->setItem(row_no, 3, new QTableWidgetItem(songFetcher.value(3).toString()));
        row_no++;
    }
    table->setRowCount(row_no);

    database.close();
}

void MainWindow::on_homeButton_clicked()
{
    loadSongTable(ui->songList, "song_data");
}


void MainWindow::on_songList_cellDoubleClicked(int row, int column)
{
    auto path = getPath(&database, row+1);
    player->playSong(path);
    player->setCurrentSong(row+1);
    ui->pause_play->setChecked(true);
}

void MainWindow::on_pause_play_clicked()
{
    player->togglePause();
}


void MainWindow::on_previous_clicked()
{
    int song = player->getCurrentSong();
    if (song > 1)
        song--;
    auto path = getPath(&database, song);
    player->playSong(path);
    player->setCurrentSong(song);
    ui->pause_play->setChecked(true);
}


void MainWindow::on_next_clicked()
{
    int song = player->getCurrentSong();
    database.open();
    QSqlQuery max("Select count(*) FROM library");
    max.next();
    int max_songs = max.value(0).toInt();
    if (song < max_songs)
        song++;
    auto path = getPath(&database, song);
    player->playSong(path);
    player->setCurrentSong(song);
    ui->pause_play->setChecked(true);
}

