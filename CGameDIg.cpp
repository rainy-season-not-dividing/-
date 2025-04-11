// CGameDIg.cpp: 实现文件
//

#include "pch.h"
#include "欢乐连连看.h"
#include "afxdialogex.h"
#include "CGameDIg.h"

// 正确定义类的静态成员变量
CString CGameDIg::BGPath;
CString CGameDIg::ElementPath; // 元素图片路径
CString CGameDIg::MaskPath;    // 掩码图片路径

// CGameDIg 对话框

IMPLEMENT_DYNAMIC(CGameDIg, CDialogEx)

CGameDIg::CGameDIg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_GAME_DIALOG, pParent)
{
    // 资源路径
    BGPath = _T("res/fruit_bg.bmp");
    ElementPath = _T("res/fruit_element.bmp");
    MaskPath = _T("res/fruit_mask.bmp");

    m_GameRegionTop.y = MAP_TOP;  // 50
    m_GameRegionTop.x = MAP_LEFT; // 20
    m_sizeElem.cx = PIC_WIDTH;    // 40
    m_sizeElem.cy = PIC_HEIGHT;   // 40

    m_GameRegion.top = MAP_TOP;
    m_GameRegion.left = MAP_LEFT;
    m_GameRegion.right = MAP_LEFT + PIC_WIDTH * Cols;
    m_GameRegion.bottom = MAP_TOP + PIC_HEIGHT * Rows;

    playing = false;
    firstSelect = false;
    GameMode = 0;
}

CGameDIg::~CGameDIg()
{
}

void CGameDIg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameDIg, CDialogEx)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BIN_START, &CGameDIg::OnClickedBtnStart)
ON_BN_CLICKED(IDC_BIN_STOP, &CGameDIg::OnClickedBtnStop)
ON_BN_CLICKED(IDC_BIN_PROMPT, &CGameDIg::OnClickedBtnPrompt)
ON_BN_CLICKED(IDC_BIN_RESET, &CGameDIg::OnClickedBtnReset)
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CGameDIg 消息处理程序

/*
void CGameDIg::InitBackground(void)
{
    //加载背景，留着后面更新地图的使用
    HANDLE Backbmp = ::LoadImageW(NULL, BGPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    CClientDC dc(this);

    m_dcBG.CreateCompatibleDC(&dc);
    m_dcBG.SelectObject(Backbmp);

    //加载内存
    m_dcMem.CreateCompatibleDC(&dc);
    CBitmap bmpMem;

    m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);

    //处理窗口
    CRect rtWin;
    CRect rtClient;
    this->GetWindowRect(rtWin);
    this->GetClientRect(rtClient);
    int nSpanWidth = rtWin.Width() - rtClient.Width();
    int nSpanHeight = rtWin.Height() - rtClient.Height();

    MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);
    CenterWindow();
}
*/
void CGameDIg::InitBackground(void)
{
    // 加载背景，留着后面更新地图的使用
    HANDLE Backbmp = ::LoadImageW(NULL, BGPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    CBitmap *pOldBitmap = nullptr; // 提前声明 pOldBitmap
    if (Backbmp == NULL)
    {
        DWORD error = GetLastError();
        CString errorMsg;
        errorMsg.Format(_T("Failed to load background image. Error code: %d"), error);
        AfxMessageBox(errorMsg);
        return;
    }
    CClientDC dc(this);
    if (!m_dcBG.CreateCompatibleDC(&dc))
    {
        AfxMessageBox(_T("Failed to create compatible DC for background."));
        ::DeleteObject(Backbmp);
        return;
    }
    if (Backbmp != NULL)
    {
        HBITMAP hBitmap = static_cast<HBITMAP>(Backbmp);
        pOldBitmap = m_dcBG.SelectObject(static_cast<CBitmap *>(CBitmap::FromHandle(hBitmap)));
        if (pOldBitmap == NULL)
        {
            AfxMessageBox(_T("Failed to select background bitmap into DC."));
            ::DeleteObject(Backbmp);
            m_dcBG.DeleteDC();
            return;
        }
    }
    else
    {
        AfxMessageBox(_T("Background bitmap handle is NULL."));
        return;
    }
    if (pOldBitmap == NULL)
    {
        AfxMessageBox(_T("Failed to select background bitmap into DC."));
        ::DeleteObject(Backbmp);
        m_dcBG.DeleteDC();
        return;
    }
    // 加载内存
    if (!m_dcMem.CreateCompatibleDC(&dc))
    {
        AfxMessageBox(_T("Failed to create compatible memory DC."));
        m_dcBG.SelectObject(pOldBitmap);
        m_dcBG.DeleteDC();
        ::DeleteObject(Backbmp);
        return;
    }
    CBitmap bmpMem;
    if (!bmpMem.CreateCompatibleBitmap(&dc, 800, 600))
    {
        AfxMessageBox(_T("Failed to create compatible bitmap for memory DC."));
        m_dcMem.DeleteDC();
        m_dcBG.SelectObject(pOldBitmap);
        m_dcBG.DeleteDC();
        ::DeleteObject(Backbmp);
        return;
    }
    if (m_dcMem.SelectObject(&bmpMem) == NULL)
    {
        AfxMessageBox(_T("Failed to select bitmap into memory DC."));
        bmpMem.DeleteObject();
        m_dcMem.DeleteDC();
        m_dcBG.SelectObject(pOldBitmap);
        m_dcBG.DeleteDC();
        ::DeleteObject(Backbmp);
        return;
    }
    if (!m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY))
    {
        AfxMessageBox(_T("Failed to copy background to memory DC."));
        bmpMem.DeleteObject();
        m_dcMem.DeleteDC();
        m_dcBG.SelectObject(pOldBitmap);
        m_dcBG.DeleteDC();
        ::DeleteObject(Backbmp);
        return;
    }
    // 处理窗口
    CRect rtWin;
    CRect rtClient;
    this->GetWindowRect(rtWin);
    this->GetClientRect(rtClient);
    int nSpanWidth = rtWin.Width() - rtClient.Width();
    int nSpanHeight = rtWin.Height() - rtClient.Height();
    MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);
    CenterWindow();
}

void CGameDIg::OnPaint()
{

    if (IsWindowVisible() && IsWindowEnabled())
    {
        CPaintDC dc(this);
        // 检查内存 DC 是否有效
        if (m_dcMem.GetSafeHdc() == NULL)
        {
            AfxMessageBox(_T("内存 DC 无效"));
            return;
        }
        // 执行 BitBlt 操作
        if (!dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY))
        {
            AfxMessageBox(_T("Failed to copy memory DC to screen DC."));
        }
    }
    else
    {
        // 处理窗口不可见或不可用的情况，比如记录日志或进行提示
        TRACE(_T("窗口不可见或不可用，无法进行 DC 复制操作\n"));
        return;
    }

    /*
     CPaintDC dc(this); // device context for painting
    // 执行 BitBlt 操作
    if (!dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY)) {
        AfxMessageBox(_T("Failed to copy memory DC to screen DC."));
    }
    */
}

void CGameDIg::SetGameMode(int mode)
{
    this->GameMode = mode;
}

void CGameDIg::InitMode(int mode)
{
    if (mode == 1)
    {
        this->SetWindowTextW(_T("欢乐连连看--基本模式"));
    }
    else if (mode == 0)
        this->SetWindowTextW(_T("欢乐连连看--体闲模式"));
    else
        this->SetWindowTextW(_T("欢乐连连看--关卡模式"));
}

// 开始游戏按钮
void CGameDIg::OnClickedBtnStart()
{
    // 初始化地图，地图用一个二维数组保存
    bool status = m_GameC.StartGame(Rows, Cols, PicNum);
    if (status) // 初始化地图成功
    {
        // 设置状态
        playing = true;
        firstSelect = true;

        // 设置按钮
        SetButton(FALSE, TRUE, TRUE, TRUE);
        UpdateMap(); // 展示地图信息
    }
    else
        AfxMessageBox(L"行列和图片个数不匹配，无法绘制地图！", MB_OKCANCEL);

    InvalidateRect(FALSE);
}

// 暂停游戏按钮
void CGameDIg::OnClickedBtnStop()
{
    // TODO: 在此添加控件通知处理程序代码
}

// 提示按钮
void CGameDIg::OnClickedBtnPrompt()
{
    // TODO: 在此添加控件通知处理程序代码
}

// 重拍按钮
void CGameDIg::OnClickedBtnReset()
{
    // TODO: 在此添加控件通知处理程序代码
}

// 按钮状态变化
void CGameDIg::SetButton(bool start, bool stop, bool prompt, bool reset)
{
    this->GetDlgItem(IDC_BIN_START)->EnableWindow(start);
    this->GetDlgItem(IDC_BIN_STOP)->EnableWindow(stop);
    this->GetDlgItem(IDC_BIN_PROMPT)->EnableWindow(prompt);
    this->GetDlgItem(IDC_BIN_RESET)->EnableWindow(reset);
}

void CGameDIg::InitElement(CString ElementPath, CString MaskPath)
{
    CClientDC dc(this);
    HANDLE hBmp = ::LoadImageW(NULL, ElementPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_dcEle.CreateCompatibleDC(&dc);
    m_dcEle.SelectObject(hBmp);
    HANDLE hMask = ::LoadImageW(NULL, MaskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_dcMask.CreateCompatibleDC(&dc);
    m_dcMask.SelectObject(hMask);
}

BOOL CGameDIg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon
    // 让弹出窗口作为应用程序窗口显示
    this->ModifyStyleEx(0, WS_EX_APPWINDOW);
    // 设置背景
    InitBackground();
    // 设置模式
    InitMode(this->GameMode);
    // 设置游戏窗口按钮的初始状态
    SetButton(TRUE, FALSE, FALSE, FALSE);

    // 初始化加载水果元素和掩码
    InitElement(ElementPath, MaskPath);
    return TRUE; // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CGameDIg::UpdateMap()
{
    // 读取地图的信息，并且在界面上显示
    int nTop = MAP_TOP;
    int nLeft = MAP_LEFT;
    int nElemW = PIC_WIDTH;
    int nElemH = PIC_HEIGHT;
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            int nElemVal = m_GameC.GetElement(i, j);
            m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, nElemVal * nElemH, SRCPAINT);
            m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcEle, 0, nElemVal * nElemH, SRCAND);
        }
    }
}

void CGameDIg::DrawTipFrame(int nRow, int nCol)
{
    CClientDC dc(this);
    CBrush brush(RGB(233, 43, 43));
    CRect rtTipFrame;
    rtTipFrame.left = m_GameRegionTop.x + nCol * m_sizeElem.cx;
    rtTipFrame.top = m_GameRegionTop.y + nRow * m_sizeElem.cy;
    rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
    rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
    dc.FrameRect(rtTipFrame, &brush);
}

void CGameDIg::DrawTipLine(stack<Vertex> verList)
{
    CClientDC dc(this);
    CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
    CPen *pOldPen = dc.SelectObject(&penLine);
    Vertex vTop;
    CPoint cp;
    if (!verList.empty())
    {
        vTop = verList.top();
        cp.x = m_GameRegionTop.x + vTop.col * m_sizeElem.cx + m_sizeElem.cx / 2;
        cp.y = m_GameRegionTop.y + vTop.row * m_sizeElem.cy + m_sizeElem.cy / 2;
        dc.MoveTo(cp);
        verList.pop();
        while (!verList.empty())
        {
            vTop = verList.top();
            cp.x = m_GameRegionTop.x + vTop.col * m_sizeElem.cx + m_sizeElem.cx / 2;
            cp.y = m_GameRegionTop.y + vTop.row * m_sizeElem.cy + m_sizeElem.cy / 2;
            dc.LineTo(cp);
            verList.pop();
        }
    }
    dc.SelectObject(pOldPen);
}

void CGameDIg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (!playing)
        return;
    if (point.y < m_GameRegion.top || point.y > m_GameRegion.bottom || point.x < m_GameRegion.left || point.x > m_GameRegion.right)
    {
        // 如果没有在地图的区域内，则不管
        return CDialogEx::OnLButtonUp(nFlags, point);
    }
    // 计算当前点的坐标
    int nRow = (point.y - m_GameRegionTop.y) / m_sizeElem.cy;
    int nCol = (point.x - m_GameRegionTop.x) / m_sizeElem.cx;
    if (nRow > Rows - 1 || nCol > Cols - 1)
    {
        return CDialogEx::OnLButtonUp(nFlags, point);
    }
    if (m_GameC.GetElement(nRow, nCol) == BLANK)
    {
        // 如果这个区域没有图片，则返回
        return CDialogEx::OnLButtonUp(nFlags, point);
    }

    if (firstSelect)
    {
        // 如果是此次点击是第一个点，则画边框，并进行设置
        DrawTipFrame(nRow, nCol);
        m_GameC.SetFirstPoint(nRow, nCol);
    }
    else
    {
        // 如果此次点击是第二个点，怎画边框，并进行设置
        DrawTipFrame(nRow, nCol);
        m_GameC.SetSecondPoint(nRow, nCol);
        // 同时判断两个点是否相连
        stack<Vertex> verList;
        bool bSuc = m_GameC.Link(verList);
        if (bSuc)
        {
            // 如果相连，则画连接线路，
            DrawTipLine(verList);
            // 重新加载地图背景，并更新最新地图
            m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);
            UpdateMap();
        }
        Sleep(200);
        InvalidateRect(FALSE);
        if (bSuc && m_GameC.isWin())
        {
            // 设置按钮
            SetButton(TRUE, FALSE, FALSE, FALSE);
            playing = false;

            // 确保界面已完全更新后再显示消息框
            UpdateWindow();
            MessageBox(TEXT("恭喜你！获胜！"));
        }
    }
    firstSelect = !firstSelect; // 第一次和第二次点击切换标志
    CDialogEx::OnLButtonUp(nFlags, point);
}
