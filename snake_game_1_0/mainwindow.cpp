#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), start_menu_(new StartMenu(this)), game_(nullptr)
{
    setCentralWidget(start_menu_);
    setFixedSize(830, 600);
    setWindowTitle("Start Menu");
    connect(start_menu_, &StartMenu::MenuStartGame, this, &MainWindow::StartGame);
}

MainWindow::~MainWindow()
{
    if (game_)
    {
        delete game_;
    }
}

void MainWindow::StartGame(int map_width, int map_height, int base_speed)
{
    start_menu_->hide();
    game_ = new Game(this, map_width, map_height, base_speed);
    setCentralWidget(game_);
    setWindowTitle("Snake Game");
    // 固定窗口大小，使用户不能修改通过拖动边缘来调整地图的大小
    setFixedSize(map_width * 20, map_height * 20);
    game_->StartGame();
}
