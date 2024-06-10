
// start_menu.cpp
#include "start_menu.h"
#include "ui_start_menu.h"

StartMenu::StartMenu(QWidget *parent) : QWidget(parent),
                                        ui_(new Ui::StartMenu),
                                        leaderboard_("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt")
{
    ui_->setupUi(this);


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
    ui_->start_game->setStyleSheet(buttonStyle);
    connect(ui_->start_game, &QPushButton::clicked, this, &StartMenu::OnStartGameClicked);
    connect(ui_->Leaderboard, &QPushButton::clicked, this, &StartMenu::ShowLeaderboard);



}
void StartMenu::ShowLeaderboard()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Leaderboard");
    dialog.setFixedSize(800, 600); // 设置对话框的固定大小

    QTableWidget *table = new QTableWidget(leaderboard_.getRecords().size(), 3, &dialog);
    table->setHorizontalHeaderLabels({"Name", "Score", "Time"});
  table->setFixedSize(760, 560); // 设置表格的固定大小，稍微小于对话框以适应边距
  // 设置列宽自适应
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int row = 0;
    for (const PlayerRecord &record : leaderboard_.getRecords())
    {
        table->setItem(row, 0, new QTableWidgetItem(record.name));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(record.score)));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(record.time)));
        ++row;
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(table);
    dialog.setLayout(layout);

    dialog.exec();  // 使用 exec() 以模态方式显示对话框
}


StartMenu::~StartMenu()
{
    delete ui_;
}

void StartMenu::OnStartGameClicked()
{
    // 获取地图大小选择
    QString mapSizeText = ui_->map_size_select->currentText();
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
    QString difficultyText = ui_->difficulty_select->currentText();
    int baseSpeed = 100;
    if (difficultyText == "Easy")
    {
        baseSpeed = 120;
    }
    else if (difficultyText == "Normal")
     {
        baseSpeed = 100;
    }
    else if (difficultyText == "Hard")
    {
        baseSpeed = 80;
    }

    emit MenuStartGame(mapWidth, mapHeight, baseSpeed);
}
