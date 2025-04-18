#ifndef GLOBAL_H
#define GLOBAL_H

// 定义顶点结构体
typedef struct tagVertex
{
    int row;  // 行
    int col;  // 列
    int info; // 值为BLANK表示当前点没有图片
} Vertex;

// 宏定义
#define GAMEWND_WIDTH 1600  // 游戏界面宽度
#define GAMEWND_HEIGHT 1200 // 游戏界面高度
#define MAP_TOP 50         // 地图距离游戏界面上方的间距
#define MAP_LEFT 20        // 地图距离游戏界面左测的间距
#define PIC_WIDTH 40       // 图片的宽度
#define PIC_HEIGHT 40      // 图片的高度
#define Rows 4             // 地图的行数
#define Cols 4             // 地图的列数
#define PicNum 4           // 图片的个数
#define BLANK -1           // 地图没有图片
#define PROGRESS 100

#endif // GLOBAL_H