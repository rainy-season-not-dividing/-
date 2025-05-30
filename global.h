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
#define GAMEWND_WIDTH 800  // 游戏界面宽度
#define GAMEWND_HEIGHT 600 // 游戏界面高度
#define MAP_TOP 50         // 地图距离游戏界面上方的间距
#define MAP_LEFT 20        // 地图距离游戏界面左测的间距
#define PIC_WIDTH 40       // 图片的宽度
#define PIC_HEIGHT 40      // 图片的高度
#define Rows 4             // 地图的行数
#define Cols 4            // 地图的列数
#define PicNum 4           // 图片的个数
#define BLANK -1           // 地图没有图片
#define PROGRESS 100
#define EASY 300
#define MED 200
#define HARD 100

// 全局变量 - 用于运行时修改游戏参数
// 这些变量将在设置对话框中被修改，并在游戏逻辑中使用
extern int g_nMapRows;  // 地图行数
extern int g_nMapCols;  // 地图列数
extern int g_nPicCount; // 图片个数

#endif // GLOBAL_H