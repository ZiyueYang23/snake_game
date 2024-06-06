#ifndef GAME_H
#define GAME_H

#include "head.h"

class Game : public QWidget
{
    Q_OBJECT
private:
    QVector<Food> foods_;
    Food temp_food_;
    Snake snake_;
    Map map_;

    int people_score_;
    int play_time_;
    bool paused_;

    // 计时器
    QTimer play_time_timer_;
    QTimer game_timer_;
    QMediaPlayer* bg_music_;


    // ~ speed
    // 初始速度
    int initial_speed_;
    // 基本速度
    int base_speed_;
    // 当前速度
    int current_speed_;
    // 速度增量
    int add_speed_;
    // 一档加速
    int gear_1_speed_;
    // 二挡加速
    int gear_2_speed_;
    // 三挡加速
    int gear_3_speed_;
    // 最大速度
    int max_speed_;

public:
    Game(QWidget *parent = nullptr);
    ~Game();
    void StartGame();
    void PauseGame();
    void ResumeGame();
    void EndGame();

    bool GameIsOver();

    int GetScore() const;
    int GetPlayTime() const;

    // 设置地图大小
    void SetMapSize(const QSize &size);

    // @ 代办
    // 背景音乐
    void PlayBackgroundMusic();
    // 排行榜
    void AddToLeaderboard(const QString &name, int score);
    void ShowLeaderboard();
    void SetBaseSpeed(const int base_speed);

signals:
    // 分数改变信号
    void ScoreChanged(int score);
    // 游戏结束信号
    void GameEnded(int people_score_, int play_time_);
private slots:
    // 槽 更新游戏
    void UpdateGame();
    void UpdatePlayTime();

protected:
    // 重写
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    // 随机放置食物
    void PlaceFood();

    // 检查碰撞食物
    void CheckFoodCollision();
    void FoodCollisionAdjustSpeed();

};

#endif // GAME_H

// @ 好点子 代办 黑洞功能，随机产生黑洞并且不断扩大
// @ 好点子 代办 生成AI蛇，遍历全地图然后锁定一个食物
// @ 对抗功能，碰撞吃蛇，再生成蛇

// 6月之前实现到三颜色，完善加速的版本 --1.0
// 6.3 实现食物的随机颜色，时间显示
//      还有几个代办 时间显示那里有bug不知道为啥一次跳两秒
//                   然后蛇头蛇尾的形状
//                   地图大小的选择控制，难度大小的选择控制
//                   一个小bug 不知为何会弹两次
// 6.3 在室友帮助下成功修复完小bug 并且优化了速度调整 加入速度调整因素 可以更加平滑的调整速度
//     基本完成了最基础的功能而且后面的难度可不小 个人称为版本 --2.0
//     后续代办 地图大小可以自己选择
//


//#ifndef GAME_H
//#define GAME_H

//#include "head.h"

//class Game : public QWidget
//{
//    Q_OBJECT
//private:
//    QVector<Food> foods_;
//    Food temp_food_;
//    Snake snake_;
//    Map map_;

//    int people_score_;
//    int play_time_;
//    bool paused_;

//    // 计时器
//    QTimer play_time_timer_;
//    QTimer game_timer_;

//    // ~ speed
//    // 初始速度
//    int initial_speed_;
//    // 基本速度
//    int base_speed_;
//    // 当前速度
//    int current_speed_;
//    // 速度增量
//    int add_speed_;
//    // 一档加速
//    int gear_1_speed_;
//    // 二挡加速
//    int gear_2_speed_;
//    // 三挡加速
//    int gear_3_speed_;
//    // 最大速度
//    int max_speed_;

//public:
//    Game(QWidget *parent = nullptr);
//    ~Game();

//    int GetScore() const;
//    int GetPlayTime() const;

//    // 设置地图大小
//    void SetMapSize(const QSize &size);
//    void SetBaseSpeed(const int base_speed);

//    // @ 代办
//    // 背景音乐
//    void PlayBackgroundMusic();
//    // 排行榜
//    void AddToLeaderboard(const QString &name, int score);
//    void ShowLeaderboard();

//signals:
//    // 分数改变信号
//    void ScoreChanged(int score);
//    // 游戏结束信号
//    void GameEnded(int people_score_, int play_time_);

//private slots:
//    // 槽 更新游戏
//    void StartGame();
//    void UpdateGame();
//    void UpdatePlayTime();

//protected:
//    // 重写
//    void paintEvent(QPaintEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;

//private:
//    // 随机放置食物
//    void PlaceFood();

//    // 检查碰撞食物
//    void CheckFoodCollision();
//    void FoodCollisionAdjustSpeed();
//    void PauseGame();
//    void ResumeGame();
//    void EndGame();
//    bool GameIsOver();
//};

//#endif // GAME_H

//// @ 好点子 代办 黑洞功能，随机产生黑洞并且不断扩大
//// @ 好点子 代办 生成AI蛇，遍历全地图然后锁定一个食物
//// @ 对抗功能，碰撞吃蛇，再生成蛇

//// 6月之前实现到三颜色，完善加速的版本 --1.0
//// 6.3 实现食物的随机颜色，时间显示
////      还有几个代办 时间显示那里有bug不知道为啥一次跳两秒
////                   然后蛇头蛇尾的形状
////                   地图大小的选择控制，难度大小的选择控制
////                   一个小bug 不知为何会弹两次
//// 6.3 在室友帮助下成功修复完小bug 并且优化了速度调整 加入速度调整因素 可以更加平滑的调整速度
////     基本完成了最基础的功能而且后面的难度可不小 个人称为版本 --2.0
////     后续代办 地图大小可以自己选择
////
///
///
//
