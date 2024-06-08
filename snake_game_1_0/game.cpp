#include "game.h"
#include <QMouseEvent>

Game::Game(QWidget *parent,int map_width,int map_height,int initial_speed,int grid_size)
    : QWidget(parent), people_score_(0), play_time_(0), paused_(false),
      map_(map_width, map_height, grid_size), base_speed_(initial_speed), gear_1_speed_(10), gear_2_speed_(20), gear_3_speed_(30),
      max_speed_(15), initial_speed_(initial_speed), current_speed_(initial_speed),mouse_move(0)
{
    setFocus();
    int start_x = QRandomGenerator::global()->bounded(10, map_.GetWidth() - 10);
    int start_y = QRandomGenerator::global()->bounded(5, map_.GetHeight() - 5);
    snake_ = Snake(QColor(0, 255, 255), start_x, start_y);
    mouse_time_=new QTimer();
}

void Game::StartGame()
{
    PlayBackgroundMusic();
    PlayCollisionFoodMusic();
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
// 结束
void Game::EndGame()
{

    bg_music_->stop();
    bg_music_->deleteLater();
    collision_food_music_->deleteLater();

    game_timer_.stop();
    play_time_timer_.stop();
    // 发送游戏结束信号
    emit GameEnded(people_score_, play_time_);
    QMessageBox::information(this, "Game Over", QString("Game Over!\nScore: %1\nTime: %2 s").arg(people_score_).arg(play_time_));
}

void Game::PlaceFood()
{
    while (foods_.size() < 10)
    {
        int x = QRandomGenerator::global()->bounded(6, map_.GetWidth() - 6);
        int y = QRandomGenerator::global()->bounded(6, map_.GetHeight() - 6);
        temp_food_.SetPosition(QPointF(x, y));

        // 设置随机大小和颜色
        temp_food_.RandomizeFood();
        foods_.append(temp_food_);
        //test
        qDebug()<<x<<" "<<y;
        qDebug()<<map_.GetWidth();
        qDebug()<<map_.GetHeight();

    }
}

void Game::FoodCollisionAdjustSpeed()
{
    // 每次得分后减少的时间间隔
    // 调整速度的因子，0.96表示每次速度减小4%
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

void Game::paintEvent(QPaintEvent *event)
{
    // 虽然 event 变量被声明了，但在当前的代码中并没有被使用，可以忽略关于未使用变量的警告。
    Q_UNUSED(event);

    QPainter painter(this);

    // 绘制蛇头
    int flag = 1;


    // painter.setBrush(snake_.GetHeadColor());
    if (flag == 1)
    {
        painter.setBrush(snake_.GetBodyColor());
        flag = -1;
    }
    else
    {
        painter.setBrush(snake_.GetHeadColor());
        flag = 1;
    }
    QPointF head = snake_.GetHead();
    int head_size = snake_.GetHeadSize();
    int half_head_size = head_size / 2;
    painter.drawEllipse((head.x() * map_.GetGridSize()) - half_head_size, (head.y() * map_.GetGridSize()) - half_head_size, head_size, head_size);

    // 绘制蛇身体

    int body_size = snake_.GetBodySize();
    int half_body_size = body_size / 2;
    for (int i = 1; i < snake_.GetBody().size() - 2; ++i)
    {
        if (flag == 1)
        {
            painter.setBrush(snake_.GetBodyColor());
            flag = -1;
        }
        else
        {
            painter.setBrush(snake_.GetHeadColor());
            flag = 1;
        }
        const QPointF &point = snake_.GetBody()[i];
        double end_size=point.x() * map_.GetGridSize();
        painter.drawEllipse((point.x() * map_.GetGridSize()) - half_body_size, (point.y() * map_.GetGridSize()) - half_body_size, body_size, body_size);
    }

    // 绘制倒数第二部分
    // painter.setBrush(snake_.GetHeadColor());
    if (flag == 1)
    {
        painter.setBrush(snake_.GetBodyColor());
        flag = -1;
    }
    else
    {
        painter.setBrush(snake_.GetHeadColor());
        flag = 1;
    }
    QPointF second_last = snake_.GetSecondLast();
    int second_last_size = snake_.GetSecondLastSize();
    int half_second_last_size = second_last_size / 2;
    painter.drawEllipse((second_last.x() * map_.GetGridSize()) - half_second_last_size, (second_last.y() * map_.GetGridSize()) - half_second_last_size, second_last_size, second_last_size);

    // 绘制蛇尾
    // painter.setBrush(snake_.GetTailColor());
    if (flag == 1)
    {
        painter.setBrush(snake_.GetBodyColor());
        flag = -1;
    }
    else
    {
        painter.setBrush(snake_.GetHeadColor());
        flag = 1;
    }
    QPointF tail = snake_.GetTail();
    int tail_size = snake_.GetTailSize();
    int halfTailSize = tail_size / 2;
    painter.drawEllipse((tail.x() * map_.GetGridSize()) - halfTailSize, (tail.y() * map_.GetGridSize()) - halfTailSize, tail_size, tail_size);

    // 绘制食物
    for (const Food &food : foods_)
    {
        int food_size = food.GetSize();
        QPointF food_position = food.GetPosition();

        // 增加颜色的径向渐变
        // 注意这个地方的坐标，正常都是取左上角的点为坐标但是，由于你要完成中心渐变，于是要把坐标点加二分之一size,也就是加一个半径移到中心点然后半径就是size/2
        QRadialGradient gradient(food_position.x() * map_.GetGridSize() + food_size / 2,
                                 food_position.y() * map_.GetGridSize() + food_size / 2,
                                 food_size / 2);
        // 设置中心颜色为白色
        gradient.setColorAt(0, Qt::white);
        // 周围的颜色为食物颜色
        gradient.setColorAt(1, food.GetColor());

        // 创建高光的径向渐变
        QRadialGradient highlightGradient(food_position.x() * map_.GetGridSize() + food_size / 2,
                                          food_position.y() * map_.GetGridSize() + food_size / 2,
                                          food_size / 2);
        // 高光中心的颜色为半透明的白色
        highlightGradient.setColorAt(0, QColor(255, 255, 255, 150));
        // 边缘颜色为完全透明，最后一个通道就是透明度通道，高光打的是白色高光
        highlightGradient.setColorAt(0.3, QColor(255, 255, 255, 0));

        // 设置色彩刷
        painter.setBrush(gradient);
        // 设置不需要边框
        painter.setPen(Qt::NoPen);

        // 画渐变颜色
        painter.drawEllipse(food_position.x() * map_.GetGridSize(), food_position.y() * map_.GetGridSize(), food_size, food_size);

        // 设置高光刷
        painter.setBrush(highlightGradient);

        // 刷一遍高光
        painter.drawEllipse(food_position.x() * map_.GetGridSize(), food_position.y() * map_.GetGridSize(), food_size, food_size);
    }

    // 设置画笔顔色为黑色
    painter.setPen(Qt::black);
    // 字体和字号
    painter.setFont(QFont("Arial", 16));
    painter.drawText(QRect(20, 20, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Score: %1").arg(people_score_));
    painter.drawText(QRect(20, 80, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Speed: %1").arg(current_speed_));
    painter.drawText(QRect(20, 140, 400, 60), Qt::AlignTop | Qt::AlignLeft, QString("Time: %1 s").arg(play_time_));
}

void Game::mousePressEvent(QMouseEvent *event)
{
    mouse_x = event->x() - snake_.GetHeadSize()*0.5;
    mouse_y = event->y() - snake_.GetHeadSize()*0.5;
    //delete mouse_time_;
    //mouse_time_ = new QTimer(this);
    //mouse_time_->start(10);
    //connect(mouse_time_,&QTimer::timeout,this,&Game::SnakeMove);
    //m_hero.setPosition(x,y);

}
void Game::SnakeMove()
{

    for(int i=snake_.GetBody().size() - 1;i>0;--i)
    {
//                QPointF temp=snake_.GetBody()[i];
        snake_.SetBody(snake_.GetBody()[i-1],i);
//                snake_.SetBody(snake_.GetBody()[i+1],i+1);
    }
        double x2 = snake_.GetHead().rx();
        double y2 = snake_.GetHead().ry();
        double dx = mouse_x - x2;
        double dy = mouse_y - y2;
        //qDebug()<<dx;
        double distance = sqrt(dx * dx + dy * dy);
        //
        //qDebug()<<snake_.GetHead().rx();


        if (distance < 1) // 设置一个接近阈值
        {
            snake_.GetHead().setX(mouse_x);
            snake_.GetHead().setY(mouse_y);
        }
        else
        {
            double speed = 1;
            double moveX = speed * (dx / distance);
            double moveY = speed * (dy / distance);
            //qDebug()<<moveX;
            double end_x=snake_.GetHead().rx()+ moveX;
            double end_y=snake_.GetHead().ry()+ moveY;
            //qDebug()<<end_x;
            QPointF& head=snake_.GetHead();
            head.rx()=end_x;
            head.ry()=end_y;
            //snake_.GetBody().prepend(head);
            //snake_.GetBody().removeLast();
//            snake_.GetHead().setX(head.rx());
//            snake_.GetHead().setY(head.ry());


        }
        //
        qDebug()<<snake_.GetHead().rx();

        //snake_.Rect().moveTo(m_hero.m_X, m_hero.m_Y);
}
void Game::mouseReleaseEvent(QMouseEvent *event)
{
    //mouse_time_->stop();
    mouse_move = 0;
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    mouse_x = event->pos().x();
    mouse_y = event->pos().y();
    mouse_x/=20;
    mouse_y/=20;
    mouse_move = 1;
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
    case Qt::Key_W:
    case Qt::Key_Up:
        newDirection = Up;
        directionChanged = true;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        newDirection = Down;
        directionChanged = true;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        newDirection = Left;
        directionChanged = true;
        break;
    case Qt::Key_D:
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
            // 暂停游戏width, g
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

void Game::CheckFoodCollision()
{
    for (int i = 0; i < foods_.size(); ++i)
    {
        // 食物的矩形
        QRect foodRect(
            foods_[i].GetPosition().x() * map_.GetGridSize(),
            foods_[i].GetPosition().y() * map_.GetGridSize(),
            foods_[i].GetSize(),
            foods_[i].GetSize());

        // 蛇头的位置和尺寸
        QPointF snakeHead = snake_.GetHead();
        int snakeHeadSize = snake_.GetHeadSize();
        QRect snakeHeadRect(
            (snakeHead.x() * map_.GetGridSize()) - snakeHeadSize / 2,
            (snakeHead.y() * map_.GetGridSize()) - snakeHeadSize / 2,
            snakeHeadSize,
            snakeHeadSize);

        if (snakeHeadRect.intersects(foodRect))
        {
            collision_food_music_->play();

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
void Game::SnakeBiger()
{
}

// @ 代办功能
void Game::AddToLeaderboard(const QString &name, int score)
{
    // 添加到排行榜的逻辑
}

void Game::ShowLeaderboard()
{
    // 显示排行榜的逻辑
}

// ~ 封装好的函数 暂时不需要调整
void Game::PlayBackgroundMusic()
{
    // 播放背景音乐的代码
    bg_music_ = new QMediaPlayer;
    bg_music_->setMedia(QUrl::fromLocalFile("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/music/daoxiang.mp3"));
    bg_music_->setVolume(70); // 设置音量（0-100之间的值）
    bg_music_->play();
}
void Game::PlayCollisionFoodMusic()
{
    collision_food_music_ = new QMediaPlayer;
    collision_food_music_->setMedia(QUrl::fromLocalFile("/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/music/collision_food.mp3"));
    collision_food_music_->setVolume(70); // 设置音量（0-100之间的值）
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

void Game::SetBaseSpeed(const int base_speed)
{
    base_speed_ = base_speed;
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
        play_time_++; // 每次增加1秒
        update();     // 更新界面
    }
}

void Game::UpdateGame()
{

    if (paused_ || GameIsOver())
    {
        return;
    }
    // play_time_++;
    if(mouse_move==0)
    snake_.SnakeMove();
    else
    {
        SnakeMove();
    }

    // 检查边界碰撞
    if (GameIsOver())
    {
        EndGame();
    }
    CheckFoodCollision();
    update();
}
