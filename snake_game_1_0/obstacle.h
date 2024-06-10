#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPointF>
#include <QColor>
#include <QRandomGenerator>
#include <QRect>
#include <QPixmap>

class Obstacle
{
private:
    // 位置
    QPointF position_;
    // 大小
    int obstacle_size_;

    QImage image_;

public:
    Obstacle(int obstacle_size = 40);
    ~Obstacle();

    // 随机大小
    void RandomizeObstacle();

    // 获取点的位置
    QPointF GetPosition() const;
    void SetPosition(const QPointF &point);

    QImage GetImgae() const;

    // 获得大小
    int GetSize() const;
    // 改大小
    void SetSize(int size);
};

#endif // OBSTACLE_H
