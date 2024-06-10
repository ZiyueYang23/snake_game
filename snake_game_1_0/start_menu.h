#ifndef START_MENU_H
#define START_MENU_H

#include "leaderboard.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QDialog>
#include <QHeaderView>

namespace Ui
{
    class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);
    ~StartMenu();

signals:
    void MenuStartGame(int mapWidth, int mapHeight, int baseSpeed);

private slots:
    void OnStartGameClicked();

public slots:
    void ShowLeaderboard();

private:
    Ui::StartMenu *ui_;
    Leaderboard leaderboard_{"/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt"};
};

#endif
