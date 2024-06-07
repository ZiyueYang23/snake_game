#include "food.h"

Food::Food(const QColor &color, int food_size, int food_score)
    : color_(color), food_size_(food_size), food_score_(food_score) {}

Food::~Food() {}

void Food::RandomizeFood()
{
    int size = QRandomGenerator::global()->bounded(10, 40);
    int score = size;
    food_score_ = score;
    food_size_ = size;

    QColor color;
    // 随机食物颜色
    int rad = QRandomGenerator::global()->bounded(0, 256);
    int green = QRandomGenerator::global()->bounded(0, 256);
    int blue = QRandomGenerator::global()->bounded(0, 256);

    color = QColor(rad, green, blue);
    color_ = color;
}

// 位置
QPoint Food::GetPosition() const
{
    return position_;
}

void Food::SetPosition(const QPoint &position)
{
    position_ = position;
}

// 颜色
QColor Food::GetColor() const
{
    return color_;
}
void Food::SetColor(const QColor &color)
{
    color_ = color;
}

// 大小
int Food::GetSize() const
{
    return food_size_;
}
void Food::SetSize(int size)
{
    food_size_ = size;
}

// 分数
void Food::SetScore(int score)
{
    food_score_ = score;
}
int Food::GetScore() const
{
    return food_score_;
}
