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

    // ~ 计时器
    // 这个是记录游戏时间的计时器
    QTimer play_time_timer_;
    // 这个是控制蛇移动速度的计时器
    QTimer game_timer_;
    //
    QTimer* mouse_time_;

    // 背景音乐
    QMediaPlayer *bg_music_;
    // 吃食物音效
    QMediaPlayer *collision_food_music_;

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

    // 记录游玩分数
    int people_score_;
    // 记录游玩时间
    int play_time_;
    // 判断是否暂停
    bool paused_;
    //
    int mouse_x;
    int mouse_y;
    bool mouse_move;

public:
    Game(QWidget *parent,int map_width,int map_height,int initial_speed,int grid_size=20);
    ~Game();
    // 开始游戏
    void StartGame();
    // 暂停游戏
    void PauseGame();
    // 恢复游戏
    void ResumeGame();
    // 获得游玩分数
    int GetScore() const;
    // 获得游玩时间
    int GetPlayTime() const;
    // 设置地图大小
    void SetMapSize(const QSize &size);

    // @ 代办
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
    void SnakeMove();

protected:
    // 重写
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);


private:
    // 背景音乐
    void PlayBackgroundMusic();
    // 吃食物音效
    void PlayCollisionFoodMusic();
    // 随机放置食物
    void PlaceFood();
    // 检查碰撞食物
    void CheckFoodCollision();
    // 吃食物调整蛇的速度
    void FoodCollisionAdjustSpeed();
    // 这个是判断游戏是否结束
    bool GameIsOver();
    // 结束游戏后需要做的例如停止计时器，停止音乐等等
    void EndGame();
    // 蛇宽度增加变大函数
    void SnakeBiger();
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
