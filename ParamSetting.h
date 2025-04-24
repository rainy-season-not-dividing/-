#pragma once
#include <afxstr.h>

class ParamSetting
{
public:
    static CString BGPath;      //背景图片路径
    static CString ElementPath; //元素图片路径
    static CString MaskPath;    //掩码图片路径
    static CString ClickSoundPath; //点击图片的声音
    static CString ClearSoundPath; //消除图片的声音
    static CString BGMSoundPath;  //背景声音
    //static int Rows;             //地图行
    //static int Cols;             //地图列
    //static int PicNum;           //地图包含的图片个数
    static bool BGMusicOn;       //是否开启背景音乐
    static int ThemeNo;          //当前的主题编码
};
