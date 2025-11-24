#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QTableWidget>
#include "databaseManager.h"
#include "mediaPlayer.h"
#include <QMediaMetaData>
#include <random>
#include <QtSql>

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
    connect(player->getMediaPlayer(), &QMediaPlayer::mediaStatusChanged, this, &MainWindow::endOfSong);
    srand(time(0));
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
    if (title.length() > 25)
        title = title.sliced(0, 25) + "...";
    auto artist = metadata.stringValue(QMediaMetaData::ContributingArtist);
    if (artist.length() > 25)
        artist = artist.sliced(0, 25) + "...";
    ui->titleLabel->setText(title);
    ui->artistLabel->setText(artist);
}

void MainWindow::endOfSong(QMediaPlayer::MediaStatus mediaStatus) {
    if (mediaStatus == QMediaPlayer::EndOfMedia) {
        bool repeat = ui->repeat->isChecked();
        bool shuffle = ui->shuffle->isChecked();
        int nextSong;
        database.open();
        QSqlQuery max("SELECT COUNT(*) FROM library");
        max.next(); int maxNo = max.value(0).toInt();
        nextSong = player->getCurrentSong();
        if (not repeat) {
            if (nextSong == maxNo)
                nextSong = 0;
            nextSong++;
        }
        else if (shuffle) {
            nextSong = rand() % maxNo + 1;
        }

        auto path = getPath(&database, nextSong);
        player->playSong(path);
        player->setCurrentSong(nextSong);
    }
}

QSqlDatabase* MainWindow::getDatabase() {
    return &database;
}

void MainWindow::loadSongTable(QTableWidget *table, QString tableName, QString searchQuery="") {
    database.open();
    QSqlQuery songFetcher;
    songFetcher.exec("SELECT song_id, title, artist, album FROM song_data WHERE song_id IN (SELECT song_id FROM " + tableName + ") AND (title LIKE '%"+searchQuery+"%' OR artist LIKE '%"+searchQuery+"%' OR album LIKE '%"+searchQuery+"%')" );
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
    int song = ui->songList->item(row, 0)->text().toInt();
    auto path = getPath(&database, song);
    player->playSong(path);
    player->setCurrentSong(song);
    ui->pause_play->setChecked(true);
}

void MainWindow::on_pause_play_clicked()
{
    player->togglePause();
}


void MainWindow::on_previous_clicked()
{
    int song = player->getCurrentSong();
    database.open();
    QSqlQuery max("Select count(*) FROM library");
    max.next();
    int max_songs = max.value(0).toInt();
    if (song > 1)
        song--;
    else
        song = max_songs;
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
    if (song == max_songs)
        song = 0;
    song++;
    if (ui->shuffle->isChecked()) {
        song = rand() % max_songs + 1;
    }
    ui->repeat->setChecked(false);

    auto path = getPath(&database, song);
    player->playSong(path);
    player->setCurrentSong(song);
    ui->pause_play->setChecked(true);
}


void MainWindow::on_searchButton_2_clicked()
{
    loadSongTable(ui->songList, "library", ui->searchBar->text());
    ui->searchButton->setChecked(true);
}

