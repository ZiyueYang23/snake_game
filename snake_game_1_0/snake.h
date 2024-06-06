#ifndef SNAKE_H
#define SNAKE_H

#include <QVector>
#include <QPoint>
#include <QRandomGenerator>
#include <QColor>

// enum class SnakePart
//{
//     Head,
//     Body,
//     Tail
// };

// 方向
enum Direction
{
    Left,
    Up,
    Down,
    Right,
};

class Snake
{
private:
    // QPoint head_;
    // 身体
    QVector<QPoint> body_;
    // QPoint tail_;
    // 方向
    Direction direction_;
    // 颜色
    // QColor head_color_;
    // QColor body_color_;
    // QColor tail_color_;
    QColor color_;

public:
    // 默认绿色
    Snake(const QColor &color = QColor("green"), int start_x = 10, int start_y = 10);

    // 身体
    QVector<QPoint> GetBody() const;
    void SetBody(const QVector<QPoint> &body);
    // QPoint GetHead() const;
    // void SetHead(const QPoint &head);
    // QPoint GetTail() const;
    // void SetTail(const QPoint &tail);
    // 方向
    Direction GetDirection() const;
    void SetDirection(Direction &direction);
    // 颜色
    QColor GetColor() const;
    void SetColor(QColor color);
    // 蛇增长
    // @ 后续可以加宽度变大
    void SnakeGrow();
    // 蛇移动
    void SnakeMove();
    // ~碰撞检查
    // 检查是否身体碰撞
    bool CheckBodyCollision() const;
    // 检查是否边界碰撞
    bool CheckBoundaryCollision(int width, int height) const;

    // @ 细分身体 代办

};

#endif // SNAKE_H
