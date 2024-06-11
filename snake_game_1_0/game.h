#ifndef GAME_H
#define GAME_H

#include "head.h"

class Game : public QWidget
{
    Q_OBJECT
private:
    // Food是QPoint
    QVector<Food> foods_;
    Food temp_food_;
    // 地图
    Map map_;
    // 障碍物 
    QVector<Obstacle> obstacles_;
    // QPoint
    Obstacle temp_obstacle_;
    //蛇
    Snake snake_;
    // 排行榜
    Leaderboard leaderboard_{"/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt"};

    // ~ 计时器
    // 这个是记录游戏时间的计时器
    QTimer play_time_timer_;
    // 这个是控制蛇移动速度的计时器
    QTimer game_timer_;
    // 背景音乐
    QMediaPlayer *bg_music_;
    // 吃食物音效
    QMediaPlayer *collision_food_music_;
    // 死亡音效
    QMediaPlayer *dead_music_;

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
    // 标志 鼠标是否移动
    bool mouse_move_;

    // 记录鼠标坐标
    double mouse_x_;
    double mouse_y_;


public:
    Game(QWidget *parent, int map_width, int map_height, int initial_speed, int grid_size = 20);
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
    // 绘制
    void paintEvent(QPaintEvent *event) override;
    // 键盘
    void keyPressEvent(QKeyEvent *event) override;
    // 鼠标释放
    void mouseReleaseEvent(QMouseEvent *event) override;
    // 鼠标移动
    void mouseMoveEvent(QMouseEvent *event) override;
    // 鼠标按下
    void mousePressEvent(QMouseEvent *event) override;
    // 获取焦点
    void focusInEvent(QFocusEvent *event) override;

private:
    // 把重写的paintEvent的函数简化拆分成画蛇画食物等等
    void DrawSnake(QPainter &painter);
    void DrawFood(QPainter &painter);
    void DrawObstacles(QPainter &painter);
    void DrawSocreSpeedTime(QPainter &painter);

private:
    // 背景音乐
    void PlayBackgroundMusic();
    // 吃食物音效
    void PlayCollisionFoodMusic();
    // 死亡音效
    void PlayDeadMusic();

    // 随机放置食物
    void PlaceFood();
    // 检查碰撞食物
    void CheckFoodCollision();
    // 吃食物调整蛇的速度
    void FoodCollisionAdjustSpeed();

    // 检查障碍物碰撞
    void ChackObstacleCollision();
    // 放置障碍物
    void PlaceObstacle();

    // 判断游戏是否结束
    bool GameIsOver();
    // 结束游戏后需要做的例如停止计时器，停止音乐等等
    void EndGame();

    // 鼠标控制蛇移动
    void MouseContralSnakeMove();
};

#endif
