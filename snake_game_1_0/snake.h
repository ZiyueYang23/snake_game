#ifndef SNAKE_H
#define SNAKE_H

#include <QVector>
#include <QPointF>
#include <QRandomGenerator>
#include <QColor>

// 方向
enum Direction
{
    Left,
    Up,
    Down,
    Right,
    Auto
};

class Snake
{
private:
    // 身体
    QVector<QPointF> body_;
    // QPointF tail_;
    // 方向
    Direction direction_;
    // 颜色
    QColor color_;
    //
public:
    double mouse_x;
    double mouse_y;
    //
    double move_x;
    double move_y;

public:
    // 默认绿色
    Snake(const QColor &color = QColor("green"), int start_x = 10, int start_y = 10);
    // 身体
    QVector<QPointF> &GetBody();
    void SetBody(const QVector<QPointF> &body);
    void SetBody(const QPointF body, int i);
    // 方向
    Direction GetDirection() const;
    void SetDirection(Direction direction);

    // 获得身体部分
    QPointF &GetHead();
    QPointF GetTail() const;
    QPointF GetSecondLast() const;

    // 获取颜色
    QColor GetHeadColor() const;
    QColor GetBodyColor() const;
    QColor GetTailColor() const;
    QColor GetSecondLastColor() const;

    // 获取大小
    int GetHeadSize() const;
    int GetBodySize() const;
    int GetTailSize() const;
    int GetSecondLastSize() const;

    // 蛇增长
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
