﻿// 欢乐连连看Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "欢乐连连看.h"
#include "欢乐连连看Dlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "CGameDIg.h"
#include "CGameLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序"关于"菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_ABOUTBOX
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// C欢乐连连看Dlg 对话框

C欢乐连连看Dlg::C欢乐连连看Dlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C欢乐连连看Dlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C欢乐连连看Dlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BIN_BASIC, &C欢乐连连看Dlg::OnBnClickedBinBasic)
ON_BN_CLICKED(IDC_BIN_RELAX, &C欢乐连连看Dlg::OnBnClickedBinRelax)
ON_BN_CLICKED(IDC_BIN_LEVEL, &C欢乐连连看Dlg::OnBnClickedBinLevel)
ON_BN_CLICKED(IDC_BIN_RANK, &C欢乐连连看Dlg::OnBnClickedBinRank)
ON_BN_CLICKED(IDC_BIN_SETTING, &C欢乐连连看Dlg::OnBnClickedBinSetting)
ON_BN_CLICKED(IDC_BIN_HELP, &C欢乐连连看Dlg::OnBnClickedBinHelp)
END_MESSAGE_MAP()

// C欢乐连连看Dlg 消息处理程序

BOOL C欢乐连连看Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将"关于..."菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);	 // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// 初始化窗口背景
	InitBackground();

	// TODO: 在此添加额外的初始化代码

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void C欢乐连连看Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C欢乐连连看Dlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		// 从位图内存中拷贝内容到视频内存，进行显示
		dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
	}
}

// 当用户拖动最小化窗口时系统调用此函数取得光标
// 显示。
HCURSOR C欢乐连连看Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C欢乐连连看Dlg::InitBackground(void)
{
	// 加载背景
	CBitmap bmpMain;
	bmpMain.LoadBitmap(IDB_MAIN);

	// 获取当前对话框视频内存
	CClientDC dc(this);
	// 创建与视频内存兼容的内存DC
	m_dcMem.CreateCompatibleDC(&dc);
	// 将位图资源入选DC
	m_dcMem.SelectObject(bmpMain);

	// 调整窗口大小
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	this->GetClientRect(rtClient);
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);
	CenterWindow();
}

// 基本模式按钮
void C欢乐连连看Dlg::OnBnClickedBinBasic()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);

	// 显示游戏界面，模拟显示
	CGameDIg dlg;
	dlg.SetGameMode(0);
	dlg.DoModal();

	// 展示主界面
	this->ShowWindow(SW_SHOW);
}

// 体闲模式按钮
void C欢乐连连看Dlg::OnBnClickedBinRelax()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);

	// 显示游戏界面，模拟显示
	CGameDIg dlg;
	dlg.SetGameMode(1);
	dlg.DoModal();

	// 展示主界面
	this->ShowWindow(SW_SHOW);
}

// 关卡模式按钮
void C欢乐连连看Dlg::OnBnClickedBinLevel()
{
	// 隐藏主界面
	this->ShowWindow(SW_HIDE);

	// 创建并显示关卡选择对话框
	CGameLevelDlg levelDlg;
	levelDlg.DoModal();

	// 展示主界面
	this->ShowWindow(SW_SHOW);
}

// 排行榜按钮
void C欢乐连连看Dlg::OnBnClickedBinRank()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 设置按钮
void C欢乐连连看Dlg::OnBnClickedBinSetting()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 帮助按钮
void C欢乐连连看Dlg::OnBnClickedBinHelp()
{
	// TODO: 在此添加控件通知处理程序代码
}