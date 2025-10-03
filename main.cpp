#include "mainwindow.h"
#include "databaseManager.h"
#include <QtSql/QSqlQuery>
#include "mediaPlayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSqlDatabase mainDatabase = initConnectDatabase();
    indexer(&mainDatabase);
    return a.exec();
}

