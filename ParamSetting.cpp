#include "pch.h"
#include "ParamSetting.h"

CString ParamSetting::BGPath = _T("theme/picture/fruit_bg.bmp");
CString ParamSetting::ElementPath = _T("theme/picture/fruit_element.bmp");
CString ParamSetting::MaskPath = _T("theme/picture/fruit_mask.bmp");

CString ParamSetting::ClickSoundPath = _T("theme/sound/fruit_click.wav");
CString ParamSetting::ClearSoundPath = _T("theme/sound/fruit_clear.wav");
CString ParamSetting::BGMSoundPath = _T("theme/sound/bgm.mp3");

//int ParamSetting::Rows = 4;
//int ParamSetting::Cols = 4;
//int ParamSetting::PicNum = 4;

bool ParamSetting::BGMusicOn = false;
int ParamSetting::ThemeNo = 0;