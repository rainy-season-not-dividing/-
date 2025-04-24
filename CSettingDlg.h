#pragma once
#include "afxdialogex.h"

// CGameSettingDlg 对话框
class CGameSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameSettingDlg)

public:
	CGameSettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameSettingDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// 控件变量
	CEdit m_editMapRows;       // 地图行数编辑框
	CEdit m_editMapCols;       // 地图列数编辑框
	CEdit m_editPicCount;      // 图片个数编辑框
	CComboBox m_comboTheme;    // 主题下拉框
	BOOL m_bBGMusicOn;         // 背景音乐开关

public:
	// 获取设置的值
	int GetMapRows() const { return m_nMapRows; }
	int GetMapCols() const { return m_nMapCols; }
	int GetPicCount() const { return m_nPicCount; }
	int GetThemeIndex() const { return m_nThemeIndex; }
	BOOL IsBGMusicOn() const { return m_bBGMusicOn; }

	// 设置初始值
	void SetMapRows(int rows) { m_nMapRows = rows; }
	void SetMapCols(int cols) { m_nMapCols = cols; }
	void SetPicCount(int count) { m_nPicCount = count; }
	void SetThemeIndex(int index) { m_nThemeIndex = index; }
	void SetBGMusicOn(BOOL on) { m_bBGMusicOn = on; }

private:
	int m_nMapRows;     // 地图行数
	int m_nMapCols;     // 地图列数
	int m_nPicCount;    // 图片个数
	int m_nThemeIndex;  // 主题索引

public:
	afx_msg void OnBnClickedRadioBgmOn();
	afx_msg void OnBnClickedRadioBgmOff();
	afx_msg void OnCbnSelchangeComboTheme();
	afx_msg void OnEnChangeEditRows();
	afx_msg void OnEnChangeEditCols();
	afx_msg void OnEnChangeEditPicCount();
	afx_msg void OnBnClickedOk();
};