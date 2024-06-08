#ifndef FOOD_H
#define FOOD_H

#include <QPointF>
#include <QColor>
#include <QRandomGenerator>

class Food
{
private:
    // 位置
    QPointF position_;
    // 颜色
    QColor color_;
    // 大小
    int food_size_;
    // 分数
    int food_score_;

public:
    // 默认红色
    Food(const QColor &color = QColor("red"), int food_size = 20, int food_score = 20);
    ~Food();

    // 随机食物的颜色，大小和分数
    void RandomizeFood();

    // 获取点的位置
    QPointF GetPosition() const;
    void SetPosition(const QPointF &point);

    // 颜色
    QColor GetColor() const;
    void SetColor(const QColor &color);

    // 获得大小
    int GetSize() const;
    // 改大小
    void SetSize(int size);

    // 获得分数
    void SetScore(int score);
    // 改分数
    int GetScore() const;
};

#endif // FOOD_H
