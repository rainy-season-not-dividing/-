
// 欢乐连连看Dlg.h: 头文件
//

#pragma once



// C欢乐连连看Dlg 对话框
class C欢乐连连看Dlg : public CDialogEx
{
	// 构造
public:
	C欢乐连连看Dlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;
	CDC m_dcMem;	// 内存DC

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// 绘制背景
	void InitBackground(void);
	DECLARE_MESSAGE_MAP()

public:
	// 基本模式按钮
	afx_msg void OnBnClickedBinBasic();
	// 体闲模式按钮
	afx_msg void OnBnClickedBinRelax();
	afx_msg void OnBnClickedBinLevel();
	afx_msg void OnBnClickedBinRank();
	afx_msg void OnBnClickedBinSetting();
	// 帮助按钮
	afx_msg void OnBnClickedBinHelp();
};