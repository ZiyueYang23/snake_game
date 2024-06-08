#include "obstacle.h"

Obstacle::Obstacle(int obstacle_size)
    :obstacle_size_(obstacle_size){
    map_[0].load("seadamn.png");
//    rect_.setSize
}

Obstacle::~Obstacle() {}

void Obstacle::RandomizeObstacle()
{
    int size = QRandomGenerator::global()->bounded(70, 100);
    obstacle_size_ = size;

}

// 位置
QPointF Obstacle::GetPosition() const
{
    return position_;
}

void Obstacle::SetPosition(const QPointF &position)
{
    position_ = position;
}

// 大小
int Obstacle::GetSize() const
{
    return obstacle_size_;
}
void Obstacle::SetSize(int size)
{
    obstacle_size_ = size;
}
