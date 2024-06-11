#include "game.h"

Game::Game(QWidget *parent, int map_width, int map_height, int initial_speed, int grid_size)
    : QWidget(parent), people_score_(0), play_time_(0), paused_(false),
      map_(map_width, map_height, grid_size), base_speed_(initial_speed), gear_1_speed_(10), gear_2_speed_(20), gear_3_speed_(30),
      max_speed_(15), initial_speed_(initial_speed), current_speed_(initial_speed), mouse_move_(0), dead_music_(nullptr)
{
    // 确定焦点的策略，使得游戏窗口可以接收键盘输入事件。
    setFocusPolicy(Qt::StrongFocus);
    // 确保窗口在创建时立即获得键盘焦点。这意味着用户可以在游戏开始时立即使用键盘控制游戏，而不需要先点击窗口
    setFocus();

    int start_x = QRandomGenerator::global()->bounded(10, map_.GetWidth() - 10);
    int start_y = QRandomGenerator::global()->bounded(5, map_.GetHeight() - 5);
    snake_ = Snake(start_x, start_y);

}

void Game::StartGame()
{
    // 播放背景音乐
    PlayBackgroundMusic();
    // PlayDeadMusic();

    // 放置食物
    PlaceFood();
    // 放置障碍物
    PlaceObstacle();

    // 连接计时器
    connect(&play_time_timer_, &QTimer::timeout, this, &Game::UpdatePlayTime);
    // 每秒触发一次
    play_time_timer_.start(1000);
    // 连接定时器
    connect(&game_timer_, &QTimer::timeout, this, &Game::UpdateGame);
    // 150毫秒就会发出一次超时信号进而调用UpdateGame函数
    game_timer_.start(initial_speed_);
}
// 结束
void Game::EndGame()
{

    bg_music_->stop();
    bg_music_->deleteLater();

    if (collision_food_music_ != nullptr)
    {
        collision_food_music_->stop();
        collision_food_music_->deleteLater();
    }

    dead_music_->stop();
    dead_music_->deleteLater();

    game_timer_.stop();
    play_time_timer_.stop();

    bool ok;
    QString playerName = QInputDialog::getText(this, "Game Over", "Enter your name:", QLineEdit::Normal, "", &ok);

    if (ok && !playerName.isEmpty())
    {
        leaderboard_.AddRecord(playerName, people_score_, play_time_);
    }
    // 发送游戏结束信号
    emit GameEnded(people_score_, play_time_);
    QMessageBox::information(this, "Game Over", QString("Game Over !\nScore:  %1\nTime:  %2 s").arg(people_score_).arg(play_time_));
}

void Game::PlaceFood()
{
    // 放置逻辑为场上永远存在10个，吃掉一个就刷新出一个
    while (foods_.size() < 10)
    {
        int x = QRandomGenerator::global()->bounded(6, map_.GetWidth() - 6);
        int y = QRandomGenerator::global()->bounded(6, map_.GetHeight() - 6);
        temp_food_.SetPosition(QPointF(x, y));

        // 设置随机大小和颜色
        temp_food_.RandomizeFood();
        foods_.append(temp_food_);
    }
}

// 检查障碍物碰撞
void Game::ChackObstacleCollision()
{

    for (int i = 0; i < obstacles_.size(); ++i)
    {
        // 障碍物的矩形
        QRect obstacle_rect(
            obstacles_[i].GetPosition().x() * map_.GetGridSize(),
            obstacles_[i].GetPosition().y() * map_.GetGridSize(),
            obstacles_[i].GetSize(),
            obstacles_[i].GetSize());

        // 蛇头的位置和尺寸
        QPointF snake_head = snake_.GetHead();
        int snake_head_size = snake_.GetHeadSize();
        QRect snake_head_rect(
            (snake_head.x() * map_.GetGridSize()) - snake_head_size / 2,
            (snake_head.y() * map_.GetGridSize()) - snake_head_size / 2,
            snake_head_size,
            snake_head_size);

        // 如果撞上就结束游戏
        if (snake_head_rect.intersects(obstacle_rect))
        {
            //@ 代办加入死亡音效
            PlayDeadMusic();
            EndGame();
        }
    }
    PlaceObstacle();
}
// 放置障碍物
void Game::PlaceObstacle()
{
    // 放置逻辑为每8s增加一个，并且会不断长大
    while (obstacles_.size() < play_time_ / 8)
    {
        int x = QRandomGenerator::global()->bounded(3, map_.GetWidth() - 3);
        int y = QRandomGenerator::global()->bounded(3, map_.GetHeight() - 3);
        temp_obstacle_.SetPosition(QPointF(x, y));
        temp_obstacle_.RandomizeObstacle();
        // 将随机大小的障碍物插入QVector中
        obstacles_.append(temp_obstacle_);
    }
}

void Game::FoodCollisionAdjustSpeed()
{
    // 速度调整当速度（timer可以理解成刷新率）>50毫秒时 每次吃食物会使得速度增加4%，更加平滑的度过前期
    if (base_speed_ >= 50)
    {
        // 每次得分后减少的时间间隔
        // 调整速度的因子，0.96表示每次速度增加4%
        double speed_increase_factor = 0.96;

        int temp_speed = base_speed_;
        base_speed_ = static_cast<int>(base_speed_ * speed_increase_factor);
        base_speed_ = std::max(base_speed_, max_speed_);
        current_speed_ -= (temp_speed - base_speed_);
        if (current_speed_ < max_speed_)
        {
            current_speed_ = max_speed_;
        }
        game_timer_.setInterval(current_speed_);
    }
    else
    {
        // 当速度小于50时每次吃食物增加1,使得后面不至于过快
        int temp_speed = base_speed_;
        base_speed_ -= 1;
        base_speed_ = std::max(base_speed_, max_speed_);
        current_speed_ -= (temp_speed - base_speed_);
        if (current_speed_ < max_speed_)
        {
            current_speed_ = max_speed_;
        }
        game_timer_.setInterval(current_speed_);
    }
}

void Game::MouseContralSnakeMove()
{
    // 位置交换 头是由鼠标控制的 从尾部开始传递，把i-1的给i,就不用创建临时变量，比较巧妙
    for (int i = snake_.GetBody().size() - 1; i > 0; --i)
    {
        // 改的是i的，把i-1的给i
        snake_.SetBody(snake_.GetBody()[i - 1], i);
    }
    double x2 = snake_.GetHead().rx();
    double y2 = snake_.GetHead().ry();
    double dx = mouse_x_ - x2;
    double dy = mouse_y_ - y2;
    double distance = sqrt(dx * dx + dy * dy);

    // 设置一个接近阈值
    if (distance < 1) 
    {
        snake_.GetHead().setX(mouse_x_);
        snake_.GetHead().setY(mouse_y_);
    }
    else
    {
        double speed = 0.8;
        double moveX = speed * (dx / distance);
        double moveY = speed * (dy / distance);
        snake_.move_x_ = moveX;
        snake_.move_y_ = moveY;
        double end_x = snake_.GetHead().rx() + moveX;
        double end_y = snake_.GetHead().ry() + moveY;
        QPointF &head = snake_.GetHead();
        head.rx() = end_x;
        head.ry() = end_y;
    }
}

void Game::CheckFoodCollision()
{
    for (int i = 0; i < foods_.size(); ++i)
    {
        // 食物的矩形
        QRect food_rect(
            foods_[i].GetPosition().x() * map_.GetGridSize(),
            foods_[i].GetPosition().y() * map_.GetGridSize(),
            foods_[i].GetSize(),
            foods_[i].GetSize());

        // 蛇头的位置和尺寸
        QPointF snake_head = snake_.GetHead();
        int snake_head_size = snake_.GetHeadSize();
        QRect snake_head_rect(
            (snake_head.x() * map_.GetGridSize()) - snake_head_size / 2,
            (snake_head.y() * map_.GetGridSize()) - snake_head_size / 2,
            snake_head_size,
            snake_head_size);

        if (snake_head_rect.intersects(food_rect))
        {
            PlayCollisionFoodMusic();
            // 先记住未吃食物之前的玩家分数
            int temp_befor_score = people_score_;
            temp_befor_score /= 30;
            // 然后再加上食物的分数
            people_score_ += foods_[i].GetScore();
            // 再记录加入食物分数后的玩家分数
            int temp_after_score = people_score_;
            // 取余算增加的长度
            temp_after_score /= 30;

            // 注意这个地方为何不直接用食物取余，是因为可能有两个食物可以加出三个食物，一个加了一个半，那半个也是要加入玩家分数里面的
            // 然后保留到下一次累加
            emit ScoreChanged(people_score_);
            int add_body_length = temp_after_score - temp_befor_score;
            if (add_body_length > 0)
            {
                for (int n = 0; n < add_body_length; n++)
                {
                    snake_.SnakeGrow();
                }
            }
            // 遍历每一个食物，判断碰撞到了就从QVector中除去
            foods_.removeAt(i);

            // 调整速度
            FoodCollisionAdjustSpeed();

            break;
        }
    }
    // 更新食物
    PlaceFood();
}

// ~ 封装好的函数 暂时不需要调整
void Game::PlayBackgroundMusic()
{
    // 播放背景音乐的代码
    bg_music_ = new QMediaPlayer;
    bg_music_->setMedia(QUrl::fromLocalFile("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/music/classical_bg.mp3"));
    // 设置音量（0-100之间的值）
    bg_music_->setVolume(50); 
    bg_music_->play();
}

// ！不知道为何就是有bug
void Game::PlayDeadMusic()
{
    qDebug() << "PlayDeadMusic called";
    if (!dead_music_)
    {
        dead_music_ = new QMediaPlayer(this);
        if (!dead_music_)
        {
            qDebug() << "Failed to create QMediaPlayer";
            return;
        }
        qDebug() << "QMediaPlayer created";
    }

    QUrl mediaUrl = QUrl::fromLocalFile("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/music/dead.mp3");
    qDebug() << "Media URL:" << mediaUrl.toString();
    dead_music_->setMedia(mediaUrl);

    if (dead_music_->mediaStatus() == QMediaPlayer::NoMedia)
    {
        qDebug() << "Failed to set media";
        return;
    }
    // 设置音量（0-100之间的值）
    dead_music_->setVolume(100);
    dead_music_->play();
    qDebug() << "Media played";
}

void Game::PlayCollisionFoodMusic()
{
    collision_food_music_ = new QMediaPlayer;
    collision_food_music_->setMedia(QUrl::fromLocalFile("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/music/collision_food.mp3"));
    // 设置音量（0-100之间的值）
    collision_food_music_->setVolume(70); 
    collision_food_music_->play();
}

// ~ 封装好了的简易函数不用调整
// 检验碰墙或碰身体
bool Game::GameIsOver()
{
    return /*snake_.CheckBodyCollision() ||*/ snake_.CheckBoundaryCollision(map_.GetWidth(), map_.GetHeight());
}

int Game::GetScore() const
{
    return people_score_;
}

int Game::GetPlayTime() const
{
    return play_time_;
}

void Game::PauseGame()
{
    paused_ = true;
    game_timer_.stop();
    play_time_timer_.stop();
}

void Game::ResumeGame()
{
    paused_ = false;
    game_timer_.start();
    play_time_timer_.start();
}

Game::~Game()
{
}

void Game::SetMapSize(const QSize &size)
{
    map_.SetWidth(size.width());
    map_.SetHeight(size.height());
}

void Game::UpdatePlayTime()
{
    if (!paused_)
    {
        // 每次增加1秒
        play_time_++;
        // 更新界面
        update();    
    }
}

void Game::UpdateGame()
{

    if (paused_ || GameIsOver())
    {
        return;
    }

    if (mouse_move_ == 0)
    {

        snake_.SnakeMove();
    }
    else
    {
       MouseContralSnakeMove();
    }

    // 检查边界碰撞
    if (GameIsOver())
    {
        PlayDeadMusic();
        EndGame();
    }
    CheckFoodCollision();
    ChackObstacleCollision();
    update();
}
