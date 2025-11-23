#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QTableWidget>
#include "mediaPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSqlDatabase database;
    Player *player;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase* getDatabase();
    void loadSongTable(QTableWidget *table, QString tableName);

private slots:
    void on_homeButton_clicked();

    void on_songList_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
