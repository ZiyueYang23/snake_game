
// start_menu.cpp
#include "start_menu.h"
#include "ui_start_menu.h"

StartMenu::StartMenu(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::StartMenu)
{
    ui->setupUi(this);

    QString buttonStyle = "QPushButton {"
                              "background-color: #6495ED;"
                              "color: white;"
                              "border: 1px solid #6495ED;"
                              "border-radius: 10px;"
                              "padding: 5px 10px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #4169E1;"
                              "}";
    ui->start_game->setStyleSheet(buttonStyle);
    connect(ui->start_game, &QPushButton::clicked, this, &StartMenu::on_start_game_clicked);
    // 固定窗口大小，使用户不能修改通过拖动边缘来调整地图的大小

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

    emit MenuStartGame(mapWidth, mapHeight, baseSpeed);
}
