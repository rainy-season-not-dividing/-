#pragma once
#include "afxdialogex.h"
#include "CGameControl.h"


// CGameDIg 对话框

class CGameDIg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDIg)

public:
	CGameDIg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDIg();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

private:
	CPoint m_GameRegionTop;		//窗体地图左上角坐标
	CSize m_sizeElem;			// 每个图片的大小
	CRect m_GameRegion;			//地图所在的矩形框
	int GameMode;	//标识游戏模式

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;		//系统图标
	CDC m_dcMem;	//内存DC
	CDC m_dcBG;		// 背景
	CDC m_dcEle;	//元素
	CDC m_dcMask;	//掩码

	static CString BGPath;    // 背景图片路径
	static CString ElementPath; // 元素图片路径
	static CString MaskPath;  // 掩码图片路径

	// 创建一个CGameControl对象
	 CGameControl m_GameC;
	bool playing;             // 游戏是否进行中
	bool firstSelect;         // true表示prepared for第一个点

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnPaint();
	afx_msg void OnClickedBtnStart();
	afx_msg void OnClickedBtnStop();
	afx_msg void OnClickedBtnPrompt();
	afx_msg void OnClickedBtnReset();
	
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
	
	
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	//鼠标左键松开事件
};