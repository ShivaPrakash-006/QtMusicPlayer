#include "databaseConnection.h"
#include <QtSql/QSqlDatabase>
#include <QDir>
#include <QApplication>
#include <QFile>

QSqlDatabase initConnectDatabase(){
    //Add Sqlite
    QSqlDatabase mainDatabase = QSqlDatabase :: addDatabase("QSQLITE");

    //Load .db Path
    QDir cwd = QDir :: current();
    qDebug() << cwd.absolutePath();
    cwd.cdUp();
    cwd.cdUp();
    qDebug() << cwd.absolutePath();
    QString path = cwd.absolutePath() + "/library.db";
    qDebug() << path;

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
    QSqlQuery createTable("CREATE TABLE IF NOT EXISTS library (song_id INT PRIMARY KEY, file_path TEXT NOT NULL)");
}
