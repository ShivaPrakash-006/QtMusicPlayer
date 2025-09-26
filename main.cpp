#include "mainwindow.h"
#include "databaseConnection.h"
#include <QtSql/QSqlQuery>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    initConnectDatabase();
    return a.exec();
}

