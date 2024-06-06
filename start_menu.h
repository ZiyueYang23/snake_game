// start_menu.h
#ifndef START_MENU_H
#define START_MENU_H

#include <QWidget>

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
    void startGame(int mapWidth, int mapHeight, int baseSpeed);

private slots:
    void on_start_game_clicked();

private:
    Ui::StartMenu *ui;
};

#endif // START_MENU_H
