// CGameLevelDlg.cpp: 实现文件
//

#include "pch.h"
#include "欢乐连连看.h"
#include "afxdialogex.h"
#include "CGameLevelDlg.h"
#include "CGameDIg.h"

// CGameLevelDlg 对话框

IMPLEMENT_DYNAMIC(CGameLevelDlg, CDialogEx)

CGameLevelDlg::CGameLevelDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_LEVEL_DIALOG, pParent)
{
}

CGameLevelDlg::~CGameLevelDlg()
{
    // 清理资源
    if (m_dcMem.GetSafeHdc())
    {
        // 先选入一个空位图，再删除DC
        m_dcMem.SelectObject(CBitmap::FromHandle(NULL));
        m_dcMem.DeleteDC();
    }
    if (m_bmpBackground.GetSafeHandle())
    {
        m_bmpBackground.DeleteObject();
    }
}

void CGameLevelDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameLevelDlg, CDialogEx)
ON_BN_CLICKED(IDD_BTN_Easy, &CGameLevelDlg::OnBnClickedBtnEasy)
ON_BN_CLICKED(IDD_BTN_Med, &CGameLevelDlg::OnBnClickedBtnMed)
ON_BN_CLICKED(IDD_BTN_Hard, &CGameLevelDlg::OnBnClickedBtnHard)
END_MESSAGE_MAP()

// CGameLevelDlg 消息处理程序

void CGameLevelDlg::OnBnClickedBtnEasy()
{
    // TODO: 在此添加控件通知处理程序代码
    SetLevel(2); // 关卡模式简单
}

void CGameLevelDlg::OnBnClickedBtnMed()
{
    // TODO: 在此添加控件通知处理程序代码
    SetLevel(3); // 关卡模式中等
}

void CGameLevelDlg::OnBnClickedBtnHard()
{
    // TODO: 在此添加控件通知处理程序代码
    SetLevel(4); // 关卡模式困难
}

void CGameLevelDlg::SetLevel(int level)
{
    // 隐藏关卡选择对话框
    this->ShowWindow(SW_HIDE);

    // 创建游戏对话框并设置游戏模式
    CGameDIg dlg;
    dlg.SetGameMode(level);

    // 模态显示游戏对话框
    dlg.DoModal();

    // 游戏结束后关闭关卡选择对话框
    EndDialog(IDOK);
}

// 初始化背景函数
void CGameLevelDlg::InitBackground(void)
{
    // 加载背景
    if (!m_bmpBackground.LoadBitmapW(IDB_LEVEL))
    {
        AfxMessageBox(_T("加载背景图片资源失败！"));
        return;
    }

    // 获取当前对话框视频内存
    CClientDC dc(this);
    // 创建与视频内存兼容的内存DC
    if (!m_dcMem.CreateCompatibleDC(&dc))
    {
        AfxMessageBox(_T("创建内存设备上下文失败！"));
        return;
    }
    // 将位图资源选入DC
    CBitmap *pOldBitmap = m_dcMem.SelectObject(&m_bmpBackground);
    if (pOldBitmap == NULL)
    {
        AfxMessageBox(_T("选择位图到内存设备上下文失败！"));
        m_dcMem.DeleteDC();
        return;
    }

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

BOOL CGameLevelDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: 在此添加额外的初始化
    InitBackground();

    return TRUE; // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CGameLevelDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
    dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}
