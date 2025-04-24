#pragma once
#include <afxstr.h>
#include "global.h" // 包含global.h以获取宏定义

class ParamSetting
{
private:
    static ParamSetting *instance; // 单例实例

    // 私有构造函数，防止外部创建实例
    ParamSetting();

public:
    static CString BGPath;         // 背景图片路径
    static CString ElementPath;    // 元素图片路径
    static CString MaskPath;       // 掩码图片路径
    static CString ClickSoundPath; // 点击图片的声音
    static CString ClearSoundPath; // 消除图片的声音
    static CString BGMSoundPath;   // 背景声音

    // 不要使用宏名称作为变量名
    static int mapRows; // 地图行
    static int mapCols; // 地图列
    static int picNum;  // 地图包含的图片个数

    static bool BGMusicOn; // 是否开启背景音乐
    static int ThemeNo;    // 当前的主题编码

    // 获取单例实例
    static ParamSetting *GetInstance();

    // 初始化默认参数
    void InitDefaultParams();

    // 获取和设置地图行数
    static int GetMapRow() { return mapRows; }
    static void SetMapRow(int rows) { mapRows = rows; }

    // 获取和设置地图列数
    static int GetMapCol() { return mapCols; }
    static void SetMapCol(int cols) { mapCols = cols; }

    // 获取和设置图片数量
    static int GetPicCount() { return picNum; }
    static void SetPicCount(int count) { picNum = count; }
};
