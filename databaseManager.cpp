#include "databaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QStandardPaths>

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

    mainDatabase->close();
}
