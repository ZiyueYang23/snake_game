#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "start_menu.h"
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//private slots:
//    void startGame(int mapWidth, int mapHeight, int baseSpeed);

private:
//    StartMenu *startMenu_;
    Game *game_;
};

#endif // MAINWINDOW_H
