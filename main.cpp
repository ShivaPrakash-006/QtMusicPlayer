#include "mainwindow.h"
#include "databaseManager.h"
#include <QtSql/QSqlQuery>
#include "mediaPlayer.h"

#include <QApplication>
#include<iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSqlDatabase *mainDatabase = w.getDatabase();
    indexer(mainDatabase);
    return a.exec();
}

