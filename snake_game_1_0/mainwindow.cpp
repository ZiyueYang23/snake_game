#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), /*startMenu_(new StartMenu(this)),*/ game_(new Game(this))
{
//    setCentralWidget(startMenu_);
    setCentralWidget(game_);
    setWindowTitle("Snake Game");

//    connect(startMenu_, &StartMenu::startGame, this, &MainWindow::startGame);
}

MainWindow::~MainWindow()
{
    if (game_)
    {
        delete game_;
    }
//    if (startMenu_)
//    {
//        delete startMenu_;
//    }
}

//void MainWindow::startGame(int mapWidth, int mapHeight, int baseSpeed)
//{
//    if (game_)
//    {
//        delete game_;
//    }

//    game_ = new Game(this);
//    game_->SetMapSize(QSize(mapWidth, mapHeight));
//    game_->SetBaseSpeed(baseSpeed);

//    setCentralWidget(game_);
//    game_->setFocus();
//}
