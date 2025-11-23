#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>

QSqlDatabase initConnectDatabase();
void indexer(QSqlDatabase *mainDatabase);
QString getPath(QSqlDatabase *database, int song_id);

#endif // DATABASEMANAGER_H
