#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), start_menu_(new StartMenu(this)), game_(nullptr)
{
    setCentralWidget(start_menu_);
    setFixedSize(830,600);
    setWindowTitle("Start Menu");
    connect(start_menu_, &StartMenu::MenuStartGame, this, &MainWindow::StartGame);

}

MainWindow::~MainWindow()
{
    if (start_menu_)
    {
         //delete start_menu_;
    }
    if (game_)
    {
        delete game_;
    }
}

void MainWindow::StartGame(int mapWidth, int mapHeight, int baseSpeed)
{
    start_menu_->hide();
    delete game_;

    game_ = new Game(this,mapWidth,mapHeight,baseSpeed);
      setCentralWidget(game_);
    // 固定窗口大小，使用户不能修改通过拖动边缘来调整地图的大小
    setFixedSize(mapWidth * 20,  mapHeight * 20);
    // 确定焦点的策略，使得游戏窗口可以接收键盘输入事件。
    setFocusPolicy(Qt::StrongFocus);
    // 确保窗口在创建时立即获得键盘焦点。这意味着用户可以在游戏开始时立即使用键盘控制游戏，而不需要先点击窗口
    setFocus();
    game_->StartGame();

}
