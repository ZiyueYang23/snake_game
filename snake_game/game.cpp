

#include "game.h"

 Game::Game(QWidget *parent)
     : QWidget(parent), people_score_(0), play_time_(0), paused_(false),
       map_(120, 60, 20), base_speed_(100), gear_1_speed_(10), gear_2_speed_(20), gear_3_speed_(30),
       max_speed_(10), initial_speed_(100), current_speed_(base_speed_)
 {
     // 固定窗口大小，使用户不能修改通过拖动边缘来调整地图的大小
     setFixedSize(map_.GetWidth() * map_.GetGridSize(), map_.GetHeight() * map_.GetGridSize());
     // 确定焦点的策略，使得游戏窗口可以接收键盘输入事件。
     setFocusPolicy(Qt::StrongFocus);
     // 确保窗口在创建时立即获得键盘焦点。这意味着用户可以在游戏开始时立即使用键盘控制游戏，而不需要先点击窗口
     setFocus();
     int start_x = QRandomGenerator::global()->bounded(10, map_.GetWidth() - 10);
     int start_y = QRandomGenerator::global()->bounded(5, map_.GetHeight() - 5);
     snake_ = Snake(QColor(0, 255, 255), start_x, start_y);
     PlayBackgroundMusic();
     StartGame();
 }

 void Game::StartGame()
 {
     PlaceFood();

     // 连接计时器
     connect(&play_time_timer_, &QTimer::timeout, this, &Game::UpdatePlayTime);
     // 每秒触发一次
     play_time_timer_.start(1000);

     // 连接定时器
     connect(&game_timer_, &QTimer::timeout, this, &Game::UpdateGame);
     // 150毫秒就会发出一次超时信号进而调用UpdateGame函数
     game_timer_.start(initial_speed_);
 }

void Game::UpdatePlayTime()
{
    if (!paused_)
    {
        play_time_++; // 每次增加1秒
        update();     // 更新界面
    }
}

void Game::FoodCollisionAdjustSpeed()
{
    // 每次得分后减少的时间间隔
    // 调整速度的因子，0.9表示每次速度减小10%
    double speed_increase_factor = 0.9;
    int temp_speed = base_speed_;
    base_speed_ = static_cast<int>(base_speed_ * speed_increase_factor);
    // 限制最小速度
    base_speed_ = std::max(base_speed_, max_speed_);
    current_speed_ -=(temp_speed-base_speed_);
    if (current_speed_ < max_speed_)
    {
        current_speed_ = max_speed_;
    }
    game_timer_.setInterval(current_speed_);

}

void Game::UpdateGame()
{

    if (paused_ || GameIsOver())
    {
        return;
    }
    // play_time_++;
    snake_.SnakeMove();

    // 检查边界碰撞
    if (GameIsOver())
    {
        EndGame();
    }
    CheckFoodCollision();
    update();
}

void Game::paintEvent(QPaintEvent *event)
{
    // 虽然 event 变量被声明了，但在当前的代码中并没有被使用，可以忽略关于未使用变量的警告。
    Q_UNUSED(event);

    QPainter painter(this);

    // Draw snake
    painter.setBrush(snake_.GetColor());
    for (const QPoint &point : snake_.GetBody())
    {
        painter.drawRect(point.x() * map_.GetGridSize(), point.y() * map_.GetGridSize(), map_.GetGridSize(), map_.GetGridSize());
    }

    // Draw foods
    for (const Food &food : foods_)
    {
        painter.setBrush(food.GetColor());
        int size = food.GetSize();
        QPoint position = food.GetPosition();
        painter.drawRect(position.x() * map_.GetGridSize(), position.y() * map_.GetGridSize(), size, size);
    }

    // Draw score
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 16));
    painter.drawText(QRect(20, 20, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Score: %1").arg(people_score_));
    painter.drawText(QRect(20, 80, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Speed: %1").arg(current_speed_));
    painter.drawText(QRect(20, 140, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Time: %1 s").arg(play_time_));
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
        return;
    }

    Direction newDirection = snake_.GetDirection();
    bool directionChanged = false;

    switch (event->key())
    {
    case Qt::Key_Up:
        newDirection = Up;
        directionChanged = true;
        break;
    case Qt::Key_Down:
        newDirection = Down;
        directionChanged = true;
        break;
    case Qt::Key_Left:
        newDirection = Left;
        directionChanged = true;
        break;
    case Qt::Key_Right:
        newDirection = Right;
        directionChanged = true;
        break;
    case Qt::Key_Space:
        if (paused_)
        {
            // 继续游戏
            ResumeGame();
        }
        else
        {
            // 暂停游戏
            PauseGame();
        }
        break;
    case Qt::Key_1:
        if (add_speed_ == gear_2_speed_ || add_speed_ == gear_3_speed_)
        {
            add_speed_ = gear_1_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else if (add_speed_ == gear_1_speed_)
        {
            add_speed_ = 0;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else
        {
            add_speed_ = gear_1_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        break;
    case Qt::Key_2:
        if (add_speed_ == gear_1_speed_ || add_speed_ == gear_3_speed_)
        {
            add_speed_ = gear_2_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else if (add_speed_ == gear_2_speed_)
        {
            add_speed_ = 0;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else
        {
            add_speed_ = gear_2_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        break;
    case Qt::Key_3:
        if (add_speed_ == gear_1_speed_ || add_speed_ == gear_2_speed_)
        {
            add_speed_ = gear_3_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else if (add_speed_ == gear_3_speed_)
        {
            add_speed_ = 0;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }
        else
        {
            add_speed_ = gear_3_speed_;
            current_speed_ = base_speed_ - add_speed_;
            if (current_speed_ < max_speed_)
            {
                current_speed_ = max_speed_;
            }
            game_timer_.setInterval(current_speed_);
        }

        break;
    }

    // 确保蛇不会立即反向
    if (directionChanged && ((newDirection == Up && snake_.GetDirection() != Down) ||
                             (newDirection == Down && snake_.GetDirection() != Up) ||
                             (newDirection == Left && snake_.GetDirection() != Right) ||
                             (newDirection == Right && snake_.GetDirection() != Left)))
    {
        snake_.SetDirection(newDirection);
    }

    QWidget::keyPressEvent(event);
}

void Game::PlaceFood()
{
    while (foods_.size() < 10)
    {
        int x = QRandomGenerator::global()->bounded(6, map_.GetWidth() - 6);
        int y = QRandomGenerator::global()->bounded(6, map_.GetHeight() - 6);
        temp_food_.SetPosition(QPoint(x, y));

        // 设置随机大小和颜色
        temp_food_.RandomizeFood();
        foods_.append(temp_food_);
    }
}

void Game::CheckFoodCollision()
{
    for (int i = 0; i < foods_.size(); ++i)
    {
        QRect foodRect(
            foods_[i].GetPosition().x() * map_.GetGridSize(),
            foods_[i].GetPosition().y() * map_.GetGridSize(),
            foods_[i].GetSize(),
            foods_[i].GetSize());

        QPoint snakeHead = snake_.GetBody().front();
        QRect snakeHeadRect(
            snakeHead.x() * map_.GetGridSize(),
            snakeHead.y() * map_.GetGridSize(),
            map_.GetGridSize(),
            map_.GetGridSize());

        if (snakeHeadRect.intersects(foodRect))
        {
            int temp_befor_score = people_score_;
            temp_befor_score /= 20;
            people_score_ += foods_[i].GetScore();
            int temp_after_score = people_score_;
            temp_after_score /= 20;
            emit ScoreChanged(people_score_);
            int add_body_length = temp_after_score - temp_befor_score;
            if (add_body_length > 0)
            {
                for (int n = 0; n < add_body_length; n++)
                {
                    snake_.SnakeGrow();
                }
            }

            foods_.removeAt(i);
            // 调整速度
            FoodCollisionAdjustSpeed();
            break;
        }
    }
    PlaceFood();
}

// ~ 封装好了的简易函数，不用调整
// 检验碰墙或碰身体
bool Game::GameIsOver()
{
    return snake_.CheckBodyCollision() || snake_.CheckBoundaryCollision(map_.GetWidth(), map_.GetHeight());
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

// 结束
void Game::EndGame()
{
    // timer_.stop();                             // 停止定时器
    // emit GameEnded(people_score_, play_time_); // 发送游戏结束信号
    // QMessageBox::information(this, "Game Over", QString("Game Over!\nScore: %1").arg(people_score_));
    // 确保正确释放资源
    bg_music_->stop();
    bg_music_->deleteLater();

    game_timer_.stop();
    play_time_timer_.stop();
    emit GameEnded(people_score_, play_time_); // 发送游戏结束信号
    QMessageBox::information(this, "Game Over", QString("Game Over!\nScore: %1\nTime: %2 s").arg(people_score_).arg(play_time_));
}
void Game::SetBaseSpeed(const int base_speed)
{
    base_speed_=base_speed;
}
Game::~Game()
{
}

void Game::SetMapSize(const QSize &size)
{
    map_.SetWidth(size.width());
    map_.SetHeight(size.height());
}

// @ 代办功能
void Game::PlayBackgroundMusic()
{
    // 播放背景音乐的代码
    bg_music_ = new QMediaPlayer;
    bg_music_->setMedia(QUrl::fromLocalFile("/home/ziyueyang/Music/CloudMusic/daoxiang.map3"));
    bg_music_->setVolume(50); // 设置音量（0-100之间的值）
    bg_music_->play();

}

void Game::AddToLeaderboard(const QString &name, int score)
{
    // 添加到排行榜的逻辑
}

void Game::ShowLeaderboard()
{
    // 显示排行榜的逻辑
}


//#include "game.h"

//Game::Game(QWidget *parent)
//    : QWidget(parent), people_score_(0), play_time_(0), paused_(false),
//      map_(120, 60, 20), base_speed_(100), gear_1_speed_(10), gear_2_speed_(20), gear_3_speed_(30),
//      max_speed_(10), initial_speed_(100), current_speed_(base_speed_)
//{
//    // 固定窗口大小，使用户不能修改通过拖动边缘来调整地图的大小
//    setFixedSize(map_.GetWidth() * map_.GetGridSize(), map_.GetHeight() * map_.GetGridSize());
//    // 确定焦点的策略，使得游戏窗口可以接收键盘输入事件。
//    setFocusPolicy(Qt::StrongFocus);
//    // 确保窗口在创建时立即获得键盘焦点。这意味着用户可以在游戏开始时立即使用键盘控制游戏，而不需要先点击窗口
//    setFocus();
//    int start_x = QRandomGenerator::global()->bounded(10, map_.GetWidth() - 10);
//    int start_y = QRandomGenerator::global()->bounded(5, map_.GetHeight() - 5);
//    snake_ = Snake(QColor(0, 255, 255), start_x, start_y);
//    StartGame();
//}

//void Game::StartGame()
//{

//    PlaceFood();

//    connect(&play_time_timer_, &QTimer::timeout, this, &Game::UpdatePlayTime);
//    play_time_timer_.start(1000);

//    connect(&game_timer_, &QTimer::timeout, this, &Game::UpdateGame);
//    game_timer_.start(initial_speed_);
//}

//void Game::UpdatePlayTime()
//{
//    if (!paused_)
//    {
//        play_time_++; // 每次增加1秒
//        update();     // 更新界面
//    }
//}

//void Game::FoodCollisionAdjustSpeed()
//{
//    // 每次得分后减少的时间间隔
//    // 调整速度的因子，0.9表示每次速度减小10%
//    double speed_increase_factor = 0.9;
//    int temp_speed = base_speed_;
//    base_speed_ = static_cast<int>(base_speed_ * speed_increase_factor);
//    // 限制最小速度
//    base_speed_ = std::max(base_speed_, max_speed_);
//    current_speed_ -= (temp_speed - base_speed_);
//    if (current_speed_ < max_speed_)
//    {
//        current_speed_ = max_speed_;
//    }
//    game_timer_.setInterval(current_speed_);
//}

//void Game::UpdateGame()
//{

//    if (paused_ || GameIsOver())
//    {
//        return;
//    }
//    // play_time_++;
//    snake_.SnakeMove();

//    // 检查边界碰撞
//    if (GameIsOver())
//    {
//        EndGame();
//    }
//    CheckFoodCollision();
//    update();
//}

//void Game::paintEvent(QPaintEvent *event)
//{
//    // 虽然 event 变量被声明了，但在当前的代码中并没有被使用，可以忽略关于未使用变量的警告。
//    Q_UNUSED(event);

//    QPainter painter(this);

//    // Draw snake
//    painter.setBrush(snake_.GetColor());
//    for (const QPoint &point : snake_.GetBody())
//    {
//        painter.drawRect(point.x() * map_.GetGridSize(), point.y() * map_.GetGridSize(), map_.GetGridSize(), map_.GetGridSize());
//    }

//    // Draw foods
//    for (const Food &food : foods_)
//    {
//        painter.setBrush(food.GetColor());
//        int size = food.GetSize();
//        QPoint position = food.GetPosition();
//        painter.drawRect(position.x() * map_.GetGridSize(), position.y() * map_.GetGridSize(), size, size);
//    }

//    // Draw score
//    painter.setPen(Qt::black);
//    painter.setFont(QFont("Arial", 16));
//    painter.drawText(QRect(20, 20, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Score: %1").arg(people_score_));
//    painter.drawText(QRect(20, 80, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Speed: %1").arg(current_speed_));
//    painter.drawText(QRect(20, 140, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Time: %1 s").arg(play_time_));
//}

//void Game::keyPressEvent(QKeyEvent *event)
//{
//    if (event->isAutoRepeat())
//    {
//        event->ignore();
//        return;
//    }

//    Direction newDirection = snake_.GetDirection();
//    bool directionChanged = false;

//    switch (event->key())
//    {
//    case Qt::Key_Up:
//        newDirection = Up;
//        directionChanged = true;
//        break;
//    case Qt::Key_Down:
//        newDirection = Down;
//        directionChanged = true;
//        break;
//    case Qt::Key_Left:
//        newDirection = Left;
//        directionChanged = true;
//        break;
//    case Qt::Key_Right:
//        newDirection = Right;
//        directionChanged = true;
//        break;
//    case Qt::Key_Space:
//        if (paused_)
//        {
//            // 继续游戏
//            ResumeGame();
//        }
//        else
//        {
//            // 暂停游戏
//            PauseGame();
//        }
//        break;
//    case Qt::Key_1:
//        if (add_speed_ == gear_2_speed_ || add_speed_ == gear_3_speed_)
//        {
//            add_speed_ = gear_1_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else if (add_speed_ == gear_1_speed_)
//        {
//            add_speed_ = 0;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else
//        {
//            add_speed_ = gear_1_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        break;
//    case Qt::Key_2:
//        if (add_speed_ == gear_1_speed_ || add_speed_ == gear_3_speed_)
//        {
//            add_speed_ = gear_2_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else if (add_speed_ == gear_2_speed_)
//        {
//            add_speed_ = 0;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else
//        {
//            add_speed_ = gear_2_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        break;
//    case Qt::Key_3:
//        if (add_speed_ == gear_1_speed_ || add_speed_ == gear_2_speed_)
//        {
//            add_speed_ = gear_3_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else if (add_speed_ == gear_3_speed_)
//        {
//            add_speed_ = 0;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }
//        else
//        {
//            add_speed_ = gear_3_speed_;
//            current_speed_ = base_speed_ - add_speed_;
//            if (current_speed_ < max_speed_)
//            {
//                current_speed_ = max_speed_;
//            }
//            game_timer_.setInterval(current_speed_);
//        }

//        break;
//    }

//    // 确保蛇不会立即反向
//    if (directionChanged && ((newDirection == Up && snake_.GetDirection() != Down) ||
//                             (newDirection == Down && snake_.GetDirection() != Up) ||
//                             (newDirection == Left && snake_.GetDirection() != Right) ||
//                             (newDirection == Right && snake_.GetDirection() != Left)))
//    {
//        snake_.SetDirection(newDirection);
//    }

//    QWidget::keyPressEvent(event);
//}

//void Game::PlaceFood()
//{
//    while (foods_.size() < 10)
//    {
//        int x = QRandomGenerator::global()->bounded(6, map_.GetWidth() - 6);
//        int y = QRandomGenerator::global()->bounded(6, map_.GetHeight() - 6);
//        temp_food_.SetPosition(QPoint(x, y));

//        // 设置随机大小和颜色
//        temp_food_.RandomizeFood();
//        foods_.append(temp_food_);
//    }
//}

//void Game::CheckFoodCollision()
//{
//    for (int i = 0; i < foods_.size(); ++i)
//    {
//        QRect foodRect(
//            foods_[i].GetPosition().x() * map_.GetGridSize(),
//            foods_[i].GetPosition().y() * map_.GetGridSize(),
//            foods_[i].GetSize(),
//            foods_[i].GetSize());

//        QPoint snakeHead = snake_.GetBody().front();
//        QRect snakeHeadRect(
//            snakeHead.x() * map_.GetGridSize(),
//            snakeHead.y() * map_.GetGridSize(),
//            map_.GetGridSize(),
//            map_.GetGridSize());

//        if (snakeHeadRect.intersects(foodRect))
//        {
//            int temp_befor_score = people_score_;
//            temp_befor_score /= 20;
//            people_score_ += foods_[i].GetScore();
//            int temp_after_score = people_score_;
//            temp_after_score /= 20;
//            emit ScoreChanged(people_score_);
//            int add_body_length = temp_after_score - temp_befor_score;
//            if (add_body_length > 0)
//            {
//                for (int n = 0; n < add_body_length; n++)
//                {
//                    snake_.SnakeGrow();
//                }
//            }

//            foods_.removeAt(i);
//            // 调整速度
//            FoodCollisionAdjustSpeed();
//            break;
//        }
//    }
//    PlaceFood();
//}

//// ~ 封装好了的简易函数，不用调整
//// 检验碰墙或碰身体
//bool Game::GameIsOver()
//{
//    return snake_.CheckBodyCollision() || snake_.CheckBoundaryCollision(map_.GetWidth(), map_.GetHeight());
//}

//int Game::GetScore() const
//{
//    return people_score_;
//}

//int Game::GetPlayTime() const
//{
//    return play_time_;
//}

//void Game::PauseGame()
//{
//    paused_ = true;
//    game_timer_.stop();
//    play_time_timer_.stop();
//}

//void Game::ResumeGame()
//{
//    paused_ = false;
//    game_timer_.start();
//    play_time_timer_.start();
//}

//// 结束
//void Game::EndGame()
//{
//    // timer_.stop();                             // 停止定时器
//    // emit GameEnded(people_score_, play_time_); // 发送游戏结束信号
//    // QMessageBox::information(this, "Game Over", QString("Game Over!\nScore: %1").arg(people_score_));
//    // 确保正确释放资源
//    game_timer_.stop();
//    play_time_timer_.stop();
//    emit GameEnded(people_score_, play_time_); // 发送游戏结束信号
//    QMessageBox::information(this, "Game Over", QString("Game Over!\nScore: %1\nTime: %2 s").arg(people_score_).arg(play_time_));
//}
//void Game::SetBaseSpeed(int baseSpeed)
//{
//    base_speed_ = baseSpeed;
//    initial_speed_ = baseSpeed;
//    current_speed_ = baseSpeed;
//    game_timer_.setInterval(initial_speed_);
//}

//Game::~Game()
//{
//}

//void Game::SetMapSize(const QSize &size)
//{
//    map_.SetWidth(size.width());
//    map_.SetHeight(size.height());
//    setFixedSize(size.width() * map_.GetGridSize(), size.height() * map_.GetGridSize());
//}
//// @ 代办功能
//void Game::PlayBackgroundMusic()
//{
//    // 播放背景音乐的代码
//}

//void Game::AddToLeaderboard(const QString &name, int score)
//{
//    // 添加到排行榜的逻辑
//}

//void Game::ShowLeaderboard()
//{
//    // 显示排行榜的逻辑
//}
