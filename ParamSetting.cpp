#include "pch.h"
#include "ParamSetting.h"
#include "global.h"

// 静态成员初始化
ParamSetting *ParamSetting::instance = nullptr;

// 静态变量初始化
CString ParamSetting::BGPath = _T("res/fruit_bg.bmp");
CString ParamSetting::ElementPath = _T("res/fruit_element.bmp");
CString ParamSetting::MaskPath = _T("res/fruit_mask.bmp");

CString ParamSetting::ClickSoundPath = _T("sounds/fruit_click.wav");
CString ParamSetting::ClearSoundPath = _T("sounds/fruit_clear.wav");
CString ParamSetting::BGMSoundPath = _T("sounds/bgm.mp3");

// 使用全局作用域的宏值初始化

int ParamSetting::mapRows = Rows;


int ParamSetting::mapCols = Cols;


int ParamSetting::picNum = PicNum;


bool ParamSetting::BGMusicOn = false;
int ParamSetting::ThemeNo = 0;

// 私有构造函数实现
ParamSetting::ParamSetting()
{
    // 构造函数中初始化默认值
    InitDefaultParams();
}

// 获取单例实例
ParamSetting *ParamSetting::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ParamSetting();
    }
    return instance;
}

// 初始化默认参数
void ParamSetting::InitDefaultParams()
{
    // 从global.h中获取默认值，使用预处理条件检查宏是否定义
    mapRows = Rows;


    mapCols = Cols;


    picNum = PicNum;


    // 默认主题设置
    ThemeNo = 0;
    BGMusicOn = false;

    // 默认路径设置
    BGPath = _T("res/fruit_bg.bmp");
    ElementPath = _T("res/fruit_element.bmp");
    MaskPath = _T("res/fruit_mask.bmp");
    ClickSoundPath = _T("sounds/fruit_click.wav");
    ClearSoundPath = _T("sounds/fruit_clear.wav");
    BGMSoundPath = _T("sounds/bgm.mp3");
}