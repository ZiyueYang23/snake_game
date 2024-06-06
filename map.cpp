#include "map.h"

Map::Map(int width, int height, int grid_size)
    : width_(width), height_(height), grid_size_(grid_size) {}

int Map::GetWidth() const
{
    return width_;
}
void Map::SetWidth(int width)
{
    width_ = width;
}

int Map::GetHeight() const
{
    return height_;
}
void Map::SetHeight(int height)
{
    height_ = height;
}

int Map::GetGridSize() const
{
    return grid_size_;
}
