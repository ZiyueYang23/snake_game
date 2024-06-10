#include "game.h"

void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    DrawSnake(painter);
    DrawFood(painter);
    DrawObstacles(painter);
    DrawSocreSpeedTime(painter);
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
        // int cur_befor_play_time = play_time_;
        // add_speed_ = gear_1_speed_;
        // current_speed_ = base_speed_ - add_speed_;
        // if (current_speed_ < max_speed_)
        // {
        //     current_speed_ = max_speed_;
        // }
        // game_timer_.setInterval(current_speed_);

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
    case Qt::Key_4:
        int decrease_speed(10);
        current_speed_ = base_speed_ + decrease_speed;
        base_speed_ += decrease_speed;
        game_timer_.setInterval(current_speed_);
        break;
        //    case Qt::Key_5:
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



void Game::DrawSnake(QPainter &painter)
{
    // 绘制蛇头和身体的代码
    // 绘制蛇头

    int flag = 1;
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
    double end_size = head_size + (snake_.GetBody().size() - 2) * 0.3;
    painter.drawEllipse(QPointF(head.x() * map_.GetGridSize(), head.y() * map_.GetGridSize()), end_size / 2, end_size / 2);

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
        double end_size = body_size + (snake_.GetBody().size() - 2) * 0.3;
        painter.drawEllipse(QPointF((point.x() * map_.GetGridSize()), (point.y() * map_.GetGridSize())), end_size / 2, end_size / 2);
    }

    // 绘制倒数第二部分
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
    end_size = second_last_size + (snake_.GetBody().size() - 2) * 0.3;
    painter.drawEllipse(QPointF(second_last.x() * map_.GetGridSize(), second_last.y() * map_.GetGridSize()), end_size / 2, end_size / 2);

    // 绘制蛇尾
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
    end_size = tail_size + (snake_.GetBody().size() - 2) * 0.3;
    painter.drawEllipse(QPointF(tail.x() * map_.GetGridSize(), tail.y() * map_.GetGridSize()), end_size / 2, end_size / 2);
}

void Game::DrawFood(QPainter &painter)
{
    // 绘制食物的代码
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
}

void Game::DrawObstacles(QPainter &painter)
{
    // 绘制障碍物的代码

    for (auto &obstacle : obstacles_)
    {

        int obstacle_size = obstacle.GetSize();
        QPointF obstacle_position = obstacle.GetPosition();
        double end_size = obstacle_size + (play_time_) * 0.3;

            painter.drawImage(QRect(obstacle_position.x() * map_.GetGridSize(), obstacle_position.y() * map_.GetGridSize(), end_size, end_size),obstacle.GetImgae());

    }
}

void Game::DrawSocreSpeedTime(QPainter &painter)
{
    // 绘制分数、速度和时间的代码
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
    mouse_x = event->x() - snake_.GetHeadSize() * 0.5;
    mouse_y = event->y() - snake_.GetHeadSize() * 0.5;
    // delete mouse_time_;
    // mouse_time_ = new QTimer(this);
    // mouse_time_->start(10);
    // connect(mouse_time_,&QTimer::timeout,this,&Game::SnakeMove);
    // m_hero.setPosition(x,y);
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_move = 0;
    snake_.SetDirection(Auto);
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    mouse_x = event->pos().x();
    mouse_y = event->pos().y();
    mouse_x /= map_.GetGridSize();
    mouse_y /= map_.GetGridSize();
    mouse_move = 1;
    snake_.mouse_x = mouse_x;
    snake_.mouse_y = mouse_y;
}
