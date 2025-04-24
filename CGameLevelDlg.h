#pragma once
#include "afxdialogex.h"

// CGameLevelDlg 对话框

class CGameLevelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameLevelDlg)

public:
	CGameLevelDlg(CWnd *pParent = nullptr); // 标准构造函数
	virtual ~CGameLevelDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_LEVEL_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持
	CDC m_dcMem;									 // 内存DC
	CBitmap m_bmpBackground;						 // 背景位图

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnEasy();
	afx_msg void OnBnClickedBtnMed();
	afx_msg void OnBnClickedBtnHard();

public:
	void SetLevel(int level);
	void InitBackground(void);
	BOOL OnInitDialog();
	void OnPaint();
};
