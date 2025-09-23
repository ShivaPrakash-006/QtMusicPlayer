#include "mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QApplication>
#include <QtSql/QSqlQuery>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSqlDatabase db1 = QSqlDatabase :: addDatabase("QSQLITE");
    db1.setDatabaseName("C:/Users/lenovo/OneDrive/Documents/QtMusicPlayer/test.db");
    db1.open();
    QSqlQuery testquery;
    testquery.exec("Create table test (name varchar(20),age int,gender varchar(6))");
    testquery.exec("Insert into test values ('Umubbu',34,'Maole') ");
    testquery.exec("Select * from test");
    testquery.next();
    qDebug() << testquery.value(1).toString();
    qDebug() << testquery.value(0).toString();
    qDebug() << testquery.value(2).toString();
    return a.exec();
}

