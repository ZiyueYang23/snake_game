#include "start_menu.h"
#include "ui_start_menu.h"

StartMenu::StartMenu(QWidget *parent) : QWidget(parent),
                                        ui_(new Ui::StartMenu),
                                        leaderboard_("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt")
{
    ui_->setupUi(this);
    // 美化了一下开始按钮
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
    // 链接开始按钮
    connect(ui_->start_game, &QPushButton::clicked, this, &StartMenu::OnStartGameClicked);
    // 链接排行榜
    connect(ui_->Leaderboard, &QPushButton::clicked, this, &StartMenu::ShowLeaderboard);
    // 链接帮助界面
    connect(ui_->help, &QPushButton::clicked, this, &StartMenu::ShowHelpDialog);
}
void StartMenu::ShowLeaderboard()
{
    // 对话框
    QDialog dialog(this);
    // 命名对话框
    dialog.setWindowTitle("Leaderboard");
    // 设置对话框的固定大小
    dialog.setFixedSize(800, 600);
    // 在对话框绘制表格
    QTableWidget *table = new QTableWidget(leaderboard_.GetRecords().size(), 3, &dialog);
    // 绘制名字
    table->setHorizontalHeaderLabels({"Name", "Score", "Time"});
    // 设置表格的固定大小，稍微小于对话框以适应边距
    table->setFixedSize(760, 560);
    // 设置列宽自适应
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int row = 0;
    // 把name,score,time分别传进去
    for (const PlayerRecord &record : leaderboard_.GetRecords())
    {
        table->setItem(row, 0, new QTableWidgetItem(record.name));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(record.score)));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(record.time)));
        ++row;
    }
    // 布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(table);
    dialog.setLayout(layout);

    // 使用 exec() 以模态方式显示对话框
    dialog.exec(); 
}

StartMenu::~StartMenu()
{
    delete ui_;
}

void StartMenu::OnStartGameClicked()
{
    // 获取地图大小选择
    QString mapSizeText = ui_->map_size_select->currentText();
    int mapWidth = 120, mapHeight = 70;
    if (mapSizeText == "Small (60*40)")
    {
        mapWidth = 100;
        mapHeight = 60;
    }
    else if (mapSizeText == "Middle (120*70)")
    {
        mapWidth = 120;
        mapHeight = 70;
    }
    else if (mapSizeText == "Big (140*80)")
    {
        mapWidth = 140;
        mapHeight = 80;
    }

    // 获取难度选择
    QString difficultyText = ui_->difficulty_select->currentText();
    int baseSpeed = 100;
    if (difficultyText == "Easy")
    {
        baseSpeed = 100;
    }
    else if (difficultyText == "Normal")
    {
        baseSpeed = 90;
    }
    else if (difficultyText == "Hard")
    {
        baseSpeed = 80;
    }

    emit MenuStartGame(mapWidth, mapHeight, baseSpeed);
}
void StartMenu::ShowHelpDialog() {
    // 使用QMessageBox显示帮助信息
    QMessageBox::information(this, "How to Play", "Instructions on how to play the game:\n\n"
                                                  "1. Use w_a_s_d,up_down_right_left and !mouse! to move.\n\n"
                                                  "2. Eating food makes you grow longer and wider.\n\n"
                                                  "3. Hit a wall or an obstacle and you'll die.\n\n"
                                                  "4. All obstacles grow larger over time, and an additional obstacle is added every 8s.\n\n"
                                                  " Good luck!");
}
