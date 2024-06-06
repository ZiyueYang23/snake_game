#ifndef MAP_H
#define MAP_H

class Map
{
private:
    // 宽度
    int width_;
    // 高度
    int height_;
    // 网格大小
    int grid_size_;

public:
    Map(int width = 120, int height = 60, int grid_size = 20);
    // 宽度
    int GetWidth() const;
    void SetWidth(int width);
    // 高度
    int GetHeight() const;
    void SetHeight(int height);
    // 获得网格大小
    int GetGridSize() const;
};

#endif // MAP_H
