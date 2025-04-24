#pragma once
#include "afxdialogex.h"
#include "CGameControl.h"

// 添加MCI支持
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// CGameDIg 对话框

class CGameDIg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDIg)

public:
	CGameDIg(CWnd *pParent = nullptr); // 标准构造函数
	virtual ~CGameDIg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_GAME_DIALOG
	};
#endif

private:
	CPoint m_GameRegionTop; // 窗体地图左上角坐标
	CSize m_sizeElem;		// 每个图片的大小
	CRect m_GameRegion;		// 地图所在的矩形框
	int GameMode;			// 标识游戏模式

	// 音频相关成员变量
	CString m_strBGMusicPath;		 // 背景音乐路径
	CString m_strClickSoundPath;	 // 普通点击音效路径
	CString m_strClickPicSoundPath;	 // 点击图片音效路径
	CString m_strEliminateSoundPath; // 消除音效路径
	BOOL m_bPlayingBGMusic;			 // 背景音乐播放状态

	// 新增变量，用于处理缩放
	float m_fScaleX;	 // X方向的缩放比例
	float m_fScaleY;	 // Y方向的缩放比例
	int m_nClientWidth;	 // 当前客户区宽度
	int m_nClientHeight; // 当前客户区高度

	CProgressCtrl GameProgress; // 创建一个滚动条对象
	int TimeCount;				// 创建一个时间量

	// 按钮原始位置和大小
	struct ButtonInfo
	{
		int id;			   // 按钮ID
		CRect originalPos; // 原始位置
	};
	ButtonInfo m_ButtonInfos[4]; // 存储4个按钮的信息

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持
	HICON m_hIcon;									 // 系统图标
	CDC m_dcMem;									 // 内存DC
	CDC m_dcBG;										 // 背景
	CDC m_dcEle;									 // 元素
	CDC m_dcMask;									 // 掩码

	static CString BGPath;		// 背景图片路径
	static CString ElementPath; // 元素图片路径
	static CString MaskPath;	// 掩码图片路径

	// 创建一个CGameControl对象
	CGameControl m_GameC;
	bool playing;	  // 游戏是否进行中
	bool firstSelect; // true表示prepared for第一个点

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnStart();
	afx_msg void OnClickedBtnStop();
	afx_msg void OnClickedBtnPrompt();
	afx_msg void OnClickedBtnReset();
	afx_msg void OnSize(UINT nType, int cx, int cy); // 新增处理窗口大小变化的函数

public:
	void SetGameMode(int mode);

private:
	void InitMode(int mode);
	void InitBackground(void);
	void SetButton(bool start, bool stop, bool prompt, bool reset);
	void InitElement(CString ElementPath, CString MaskPath);
	void UpdateMap(void);
	void DrawTipFrame(int nRow, int nCol);
	void DrawTipLine(stack<Vertex> verList);
	void UpdateGameRegion(void);	  // 新增更新游戏区域的函数
	void SaveButtonPositions(void);	  // 保存按钮原始位置
	void UpdateButtonPositions(void); // 更新按钮位置

	void SetGameProgress(int range);

	// 音频相关函数
	BOOL InitAudio();			// 初始化音频系统
	BOOL PlayBackgroundMusic(); // 播放背景音乐
	BOOL StopBackgroundMusic(); // 停止背景音乐
	BOOL PlayClickSound();		// 播放普通点击音效
	BOOL PlayClickPicSound();	// 播放点击图片音效
	BOOL PlayEliminateSound();	// 播放消除音效
	void CleanupAudio();		// 清理音频资源

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); // 鼠标左键松开事件
	afx_msg void OnEnChangeEdit1();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};