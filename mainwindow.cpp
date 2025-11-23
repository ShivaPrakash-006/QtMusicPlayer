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
    int song_id = row+1;
    database.open();
    QSqlQuery getFilePath;
    getFilePath.prepare("SELECT * FROM library WHERE song_id = ?");
    getFilePath.addBindValue(song_id);
    getFilePath.exec();
    getFilePath.next();
    QString path = getFilePath.value(1).toString();
    player->playSong(path);
}

