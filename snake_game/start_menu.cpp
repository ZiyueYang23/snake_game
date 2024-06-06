
// start_menu.cpp
#include "start_menu.h"
#include "ui_start_menu.h"

StartMenu::StartMenu(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::StartMenu)
{
    ui->setupUi(this);
    connect(ui->start_game, &QPushButton::clicked, this, &StartMenu::on_start_game_clicked);
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::on_start_game_clicked()
{
    // 获取地图大小选择
    QString mapSizeText = ui->map_size_select->currentText();
    int mapWidth = 120, mapHeight = 60;
    if (mapSizeText == "Small (60*40)")
    {
        mapWidth = 60;
        mapHeight = 40;
    }
    else if (mapSizeText == "Middle (90*50)")
    {
        mapWidth = 90;
        mapHeight = 50;
    }
    else if (mapSizeText == "Big (120*60)")
    {
        mapWidth = 120;
        mapHeight = 60;
    }

    // 获取难度选择
    QString difficultyText = ui->difficulty_select->currentText();
    int baseSpeed = 120;
    if (difficultyText == "Easy")
    {
        baseSpeed = 150;
    }
    else if (difficultyText == "Normal")
     {
        baseSpeed = 120;
    }
    else if (difficultyText == "Hard")
    {
        baseSpeed = 100;
    }

    emit startGame(mapWidth, mapHeight, baseSpeed);
}
