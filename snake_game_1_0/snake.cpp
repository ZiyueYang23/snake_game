#include "snake.h"

Snake::Snake(const QColor &color, int start_x, int start_y) : color_(color)
{
    body_ << QPoint(start_x, start_y) << QPoint(start_x - 1, start_y) << QPoint(start_x - 2, start_y) << QPoint(start_x - 3, start_y) << QPoint(start_x - 4, start_y);

    direction_ = Right;
}

void Snake::SnakeGrow()
{
    body_.append(body_.last());
}

void Snake::SnakeMove()
{
    QPoint head = body_.front();

    switch (direction_)
    {
    case Up:
        head.ry() -= 1;
        break;
    case Down:
        head.ry() += 1;
        break;
    case Left:
        head.rx() -= 1;
        break;
    case Right:
        head.rx() += 1;
        break;
    }

    // head此处存储的是此次按下上下左右，后head此时存储了更新后的蛇头的位置
    // prepend函数的功能就是在QVector中的头部插入一个新的QPoint,把head传进去完成了蛇头的更新
    // 随后就是removeLast删除掉尾部，完成蛇尾更新
    body_.prepend(head);
    body_.removeLast();
}

// 检查碰撞

bool Snake::CheckBodyCollision() const
{
    QPoint head = body_.front();

    for (int i = 1; i < body_.size(); ++i)
    {
        if (body_[i] == head)
        {
            return true;
        }
    }
    return false;
}

bool Snake::CheckBoundaryCollision(int width, int height) const
{
    QPoint head = body_.front();
    // 四个边界
    return head.x() < 0 || head.x() >= width || head.y() < 0 || head.y() >= height;
}

// 方向
Direction Snake::GetDirection() const
{
    return direction_;
}
void Snake::SetDirection(Direction &direction)
{
    this->direction_ = direction;
}
// 身体
QVector<QPoint> Snake::GetBody() const
{
    return body_;
}
void Snake::SetBody(const QVector<QPoint> &body)
{
    body_ = body;
}
QPoint Snake::GetHead() const
{
    return body_.front();
}

QPoint Snake::GetTail() const
{
    return body_.back();
}

QPoint Snake::GetSecondLast() const
{
    return body_[body_.size() - 2];
}

QColor Snake::GetHeadColor() const
{
    // 红色 头的颜色
    // return QColor(255, 0, 0);
    return QColor(189, 255, 140);
}

QColor Snake::GetBodyColor() const
{
    // 青色 身体颜色
    return QColor(126, 143, 234);
}

QColor Snake::GetTailColor() const
{
    // 青色 同身体颜色
    return QColor(126, 143, 234);
}

QColor Snake::GetSecondLastColor() const
{
    // 青色 同身体颜色
    return QColor(126, 143, 234);
}

int Snake::GetHeadSize() const
{
    // 头部大小
    return 35;
}

int Snake::GetBodySize() const
{
    // 身体大小
    return 25;
}
int Snake::GetSecondLastSize() const
{
    // 倒数第二部分大小
    return 20;
}

int Snake::GetTailSize() const
{
    // 尾部大小
    return 15;
}
