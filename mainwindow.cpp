#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QTableWidget>
#include "databaseManager.h"
#include "mediaPlayer.h"
#include <QMediaMetaData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    database = initConnectDatabase();
    player = new Player;

    connect(player->getMediaPlayer(), &QMediaPlayer::positionChanged, this, &MainWindow::updateProgress);
    connect(player->getMediaPlayer(), &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->timeProgress, &QSlider::sliderMoved, this, [&](int value) {
        player->setPosition(value);
    });
    connect(player->getMediaPlayer(), &QMediaPlayer::metaDataChanged, this, &MainWindow::updateSongDetails);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgress(qint64 progress) {
    ui->timeProgress->setValue(progress);
    int seconds = progress/1000;
    int mins = seconds/60;
    seconds %= 60;
    QString timeText = "0" + QString::number(mins) + ":" + QString::number(seconds);
    ui->songTime->setText(timeText);
}

void MainWindow::updateDuration(qint64 duration) {
    ui->timeProgress->setMaximum(duration);
    int seconds = duration/1000;
    int mins = seconds/60;
    seconds %= 60;
    QString timeText = "0" + QString::number(mins) + ":" + QString::number(seconds);
    ui->totalSongTime->setText(timeText);
}

void MainWindow::updateSongDetails() {
    QMediaPlayer *mediaPlayer = player->getMediaPlayer();
    QMediaMetaData metadata = mediaPlayer->metaData();

    auto title = metadata.stringValue(QMediaMetaData::Title);
    if (title.length() > 30)
        title = title.sliced(0, 25) + "...";
    auto artist = metadata.stringValue(QMediaMetaData::ContributingArtist);
    if (artist.length() > 30)
        artist = artist.sliced(0, 25) + "...";
    ui->titleLabel->setText(title);
    ui->artistLabel->setText(artist);
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

