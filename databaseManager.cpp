#include "databaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaMetaData>
#include <QUrl>

QSqlDatabase initConnectDatabase(){
    //Add Sqlite
    QSqlDatabase mainDatabase = QSqlDatabase :: addDatabase("QSQLITE");

    //Load .db Path
    QDir cwd = QDir :: current();

    //Remove these two cdUp()s for the final build
    cwd.cdUp();
    cwd.cdUp();
    QString path = cwd.absolutePath() + "/library.db";

    //Search and Create .db File
    if (!cwd.exists("library.db")) {
        QFile dbFile(path);
        dbFile.open(QIODevice::WriteOnly);
        dbFile.close();
    }

    //Open the .db File
    mainDatabase.setDatabaseName(path);
    mainDatabase.open();

    //Create Table if not exists
    QSqlQuery createTable;
    createTable.exec("CREATE TABLE IF NOT EXISTS library (song_id INT PRIMARY KEY, file_path TEXT NOT NULL)");
    createTable.exec("CREATE TABLE IF NOT EXISTS song_data(song_id INT PRIMARY KEY,title TEXT,artist TEXT,album TEXT,year TEXT,genre TEXT,track_no INT,length TEXT,bit_rate TEXT)");
    mainDatabase.close();
    return mainDatabase;
}

void indexer(QSqlDatabase *mainDatabase){
    // Get Default Music Path from OS
    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Music/";
    QDir musicDir(musicPath);

    // Create Folder if it does not exist
    if (!musicDir.exists()){
        musicDir.mkpath(musicPath);
    }

    // Get all songs from the path
    QStringList music = musicDir.entryList(QStringList{"*.mp3"}, QDir::NoDotAndDotDot | QDir::Files);
    int noOfSongs = music.length();

    mainDatabase->open();

    // Rebuild/Reindex the library
    QSqlQuery indexer;
    indexer.exec("DELETE FROM library");
    for (int songNo = 0; songNo < noOfSongs; ++songNo) {
        indexer.prepare("INSERT INTO library VALUES (?, ?)");
        indexer.addBindValue(songNo + 1); // Add one to start songNo at 1
        indexer.addBindValue(musicPath + music[songNo]);
        indexer.exec();
    }

    indexer.exec("DELETE FROM song_data");
    QMediaPlayer *metadataFetcher = new QMediaPlayer;
    QMediaMetaData metadata;
    QObject::connect(metadataFetcher, &QMediaPlayer::metaDataChanged, [=]() {
            metadata = metadataFetcher -> metaData();
            indexer.prepare("INSERT INTO song_data VALUES (?,?,?,?,?,?,?,?,?)");
            indexer.addBindValue(songNo + 1);
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::Title));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::ContributingArtist));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::AlbumTitle));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::Date));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::Genre));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::TrackNumber));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::Duration));
            indexer.addBindValue(metadata.stringValue(QMediaMetaData::AudioBitRate));
            indexer.exec();
    }
    for (int songNo = 0; songNo < noOfSongs; ++songNo){
        metadataFetcher -> setSource(QUrl :: fromLocalFile(musicPath + music[songNo]));
    }
    mainDatabase->close();
}
