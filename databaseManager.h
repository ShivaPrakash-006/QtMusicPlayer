#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>

QSqlDatabase initConnectDatabase();
void indexer(QSqlDatabase *mainDatabase);

#endif // DATABASEMANAGER_H
