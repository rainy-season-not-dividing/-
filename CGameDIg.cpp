// CGameDIg.cpp: 实现文件

#include "pch.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "欢乐连连看.h"
#include "afxdialogex.h"
#include "CGameDIg.h"
#include "ParamSetting.h"
#include "CSettingDlg.h"

// 正确定义类的静态成员变量
CString CGameDIg::BGPath;
CString CGameDIg::ElementPath; // 元素图片路径
CString CGameDIg::MaskPath;    // 掩码图片路径

// CGameDIg 对话框

IMPLEMENT_DYNAMIC(CGameDIg, CDialogEx)

CGameDIg::CGameDIg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_GAME_DIALOG, pParent)
{
    // 确保ParamSetting已初始化
    ParamSetting::GetInstance();

    // 资源路径
    BGPath = ParamSetting::BGPath;
    ElementPath = ParamSetting::ElementPath;
    MaskPath = ParamSetting::MaskPath;

    // 音频资源路径
    m_strBGMusicPath = ParamSetting::BGMSoundPath;
    m_strClickSoundPath = ParamSetting::ClickSoundPath;
    m_strClickPicSoundPath = ParamSetting::ClearSoundPath;
    m_strEliminateSoundPath = ParamSetting::ClearSoundPath;
    m_bPlayingBGMusic = FALSE;

    m_GameRegionTop.y = MAP_TOP;  // 50
    m_GameRegionTop.x = MAP_LEFT; // 20
    m_sizeElem.cx = PIC_WIDTH;    // 40
    m_sizeElem.cy = PIC_HEIGHT;   // 40

    m_GameRegion.top = MAP_TOP;
    m_GameRegion.left = MAP_LEFT;
    m_GameRegion.right = MAP_LEFT + PIC_WIDTH * ParamSetting::GetMapCol();
    m_GameRegion.bottom = MAP_TOP + PIC_HEIGHT * ParamSetting::GetMapRow();

    playing = false;
    firstSelect = false;
    GameMode = 0;
}

CGameDIg::~CGameDIg()
{
    // 清理音频资源
    CleanupAudio();
}

void CGameDIg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PRB_TIME, GameProgress); // 将滚动条和空间绑定在一起
}

BEGIN_MESSAGE_MAP(CGameDIg, CDialogEx)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BIN_START, &CGameDIg::OnClickedBtnStart)
ON_BN_CLICKED(IDC_BIN_STOP, &CGameDIg::OnClickedBtnStop)
ON_BN_CLICKED(IDC_BIN_PROMPT, &CGameDIg::OnClickedBtnPrompt)
ON_BN_CLICKED(IDC_BIN_RESET, &CGameDIg::OnClickedBtnReset)
ON_BN_CLICKED(IDC_BIN_SETTING, &CGameDIg::OnBnClickedBinSetting)
ON_WM_LBUTTONUP()
ON_EN_CHANGE(IDC_EDIT_TIME, &CGameDIg::OnEnChangeEdit1)

ON_WM_TIMER()
END_MESSAGE_MAP()

// CGameDIg 消息处理程序

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

    // 获取位图信息
    BITMAP bmp;
    CBitmap *pBitmap = CBitmap::FromHandle((HBITMAP)Backbmp);
    pBitmap->GetBitmap(&bmp);
    int origWidth = bmp.bmWidth;
    int origHeight = bmp.bmHeight;

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
    if (!bmpMem.CreateCompatibleBitmap(&dc, GAMEWND_WIDTH, GAMEWND_HEIGHT))
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

    // 使用StretchBlt拉伸背景图片以填充整个窗口
    if (!m_dcMem.StretchBlt(0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, &m_dcBG, 0, 0, origWidth, origHeight, SRCCOPY))
    {
        AfxMessageBox(_T("Failed to stretch background to memory DC."));
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
    MoveWindow(0, 0, GAMEWND_WIDTH + nSpanWidth, GAMEWND_HEIGHT + nSpanHeight);
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
        // 执行 BitBlt 操作，不用修改为StretchBlt，因为m_dcMem已经是拉伸后的大小
        if (!dc.BitBlt(0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, &m_dcMem, 0, 0, SRCCOPY))
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
}

void CGameDIg::SetGameMode(int mode)
{
    this->GameMode = mode;
}

void CGameDIg::InitMode(int mode)
{
    CProgressCtrl *pProgressCtrl = (CProgressCtrl *)GetDlgItem(IDC_PRB_TIME);
    CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_TIME);
    if (mode == 0)
    {
        this->SetWindowTextW(_T("欢乐连连看--基本模式"));
        pEdit->EnableWindow(FALSE);
    }
    else if (mode == 1)
    {
        this->SetWindowTextW(_T("欢乐连连看--体闲模式"));
        pProgressCtrl->ShowWindow(SW_HIDE);
        pEdit->ShowWindow(SW_HIDE); // 体闲模式，隐藏控件
    }
    else
    {
        this->SetWindowTextW(_T("欢乐连连看--关卡模式"));
        pEdit->EnableWindow(FALSE);
    }
}

// 开始游戏按钮
void CGameDIg::OnClickedBtnStart()
{
    // 先确保所有定时器已停止
    KillTimer(1);

    // 播放点击音效
    PlayClickSound();

    // 初始化地图，地图用一个二维数组保存
    bool status = m_GameC.StartGame(ParamSetting::GetMapRow(), ParamSetting::GetMapCol(), ParamSetting::GetPicCount());
    if (status) // 初始化地图成功
    {
        // 清除旧的界面元素
        RefreshBackground();
        
        // 设置状态
        playing = true;
        firstSelect = true;

        // 设置按钮 - 开始按钮禁用，其他按钮启用
        SetButton(FALSE, TRUE, TRUE, TRUE);
        
        // 展示地图信息
        UpdateMap();

        // 根据关卡模式设置滚动条和定时器
        switch (this->GameMode)
        {
        case 0: // 休闲模式
        case 2: // 简单模式
            SetGameProgress(EASY);
            break;
        case 3: // 中等难度
            SetGameProgress(MED);
            break;
        case 4: // 困难模式
            SetGameProgress(HARD);
            break;
        default:
            SetGameProgress(EASY);
            break;
        }
        
        // 更新界面
        InvalidateRect(NULL, TRUE);
    }
    else
    {
        // 初始化失败，显示错误消息
        MessageBox(_T("行列和图片个数不匹配，无法绘制地图！"), _T("初始化失败"), MB_OK | MB_ICONWARNING);
    }
}

// 暂停游戏按钮
void CGameDIg::OnClickedBtnStop()
{
    // 播放点击音效
    PlayClickSound();

    if (playing)
    {
        this->GetDlgItem(IDC_BIN_STOP)->SetWindowText(L"重新开始");
        playing = false;
    }
    else
    {
        this->GetDlgItem(IDC_BIN_STOP)->SetWindowText(L"暂停游戏");
        playing = true;
    }
}

// 提示按钮
void CGameDIg::OnClickedBtnPrompt()
{
    // 播放点击音效
    PlayClickSound();

    if (!playing)
        return;
    stack<Vertex> verList;
    bool success = m_GameC.GetPrompt(verList);
    if (success)
    {
        // 重新加载地图背景，并更新最新地图
        RefreshBackground();
        UpdateMap();
        // 画线
        DrawTipFrame(m_GameC.helpFirst.row, m_GameC.helpFirst.col);
        DrawTipFrame(m_GameC.helpSecond.row, m_GameC.helpSecond.col);
        DrawTipLine(verList);
    }
}

// 重排按钮
void CGameDIg::OnClickedBtnReset()
{
    // 播放点击音效
    PlayClickSound();

    if (!playing)
        return;
    m_GameC.ResetMap();
    firstSelect = true;
    RefreshBackground();
    UpdateMap();
    InvalidateRect(FALSE);
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

    // 保存按钮的初始位置
    // SaveButtonPositions();

    // 初始化加载水果元素和掩码
    InitElement(ElementPath, MaskPath);

    // 初始化音频系统
    InitAudio();

    // 播放背景音乐
    PlayBackgroundMusic();

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

    int rows = ParamSetting::GetMapRow();
    int cols = ParamSetting::GetMapCol();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
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
    
    int rows = ParamSetting::GetMapRow();
    int cols = ParamSetting::GetMapCol();
    
    if (nRow > rows - 1 || nCol > cols - 1)
    {
        return CDialogEx::OnLButtonUp(nFlags, point);
    }
    if (m_GameC.GetElement(nRow, nCol) == BLANK)
    {
        // 如果这个区域没有图片，则返回
        return CDialogEx::OnLButtonUp(nFlags, point);
    }

    // 播放点击图片音效
    PlayClickPicSound();

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
            // 如果相连，则画连接线路
            DrawTipLine(verList);

            // 播放消除音效
            PlayEliminateSound();

            // 重新加载地图背景，并更新最新地图
            RefreshBackground();
            UpdateMap();
            
            // 检查游戏是否获胜
            bool isWin = m_GameC.isWin();
            
            // 调试输出
            TRACE(_T("游戏胜利状态: %d\n"), isWin ? 1 : 0);
            
            // 如果游戏胜利
            if (isWin)
            {
                // 停止定时器
                KillTimer(1);
                
                // 设置按钮状态 - 只允许开始按钮可用
                SetButton(TRUE, FALSE, FALSE, FALSE);
                
                // 设置游戏状态为非进行中
                playing = false;
                
                // 更新界面，确保所有变化都已显示
                InvalidateRect(NULL, TRUE);
                UpdateWindow();
                
                // 显示胜利消息
                MessageBox(_T("恭喜你！获胜！"), _T("游戏胜利"), MB_OK | MB_ICONINFORMATION);
                
                // 确保消息框之后UI正确刷新
                return;
            }
        }
        
        // 等待200毫秒，让玩家能看清连线
        Sleep(200);
        
        // 刷新屏幕
        InvalidateRect(FALSE);
    }
    
    // 切换第一次和第二次点击的标志
    firstSelect = !firstSelect;
    
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CGameDIg::OnEnChangeEdit1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志"或"运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

void CGameDIg::SetGameProgress(int range)
{
    GameProgress.SetRange(0, range);
    GameProgress.SetStep(-1);
    GameProgress.SetPos(range);
    TimeCount = range;
    this->SetTimer(1, 1000, NULL);
}

void CGameDIg::OnTimer(UINT_PTR nIDEvent)
{
    // 只处理定时器1的事件
    if (nIDEvent != 1) 
    {
        CDialogEx::OnTimer(nIDEvent);
        return;
    }
    
    // 如果游戏不在进行中，停止定时器并返回
    if (!playing)
    {
        KillTimer(1);
        CDialogEx::OnTimer(nIDEvent);
        return;
    }
    
    // 更新时间显示
    CString str;
    str.Format(_T("%d"), TimeCount - 1);
    SetDlgItemText(IDC_EDIT_TIME, str);
    UpdateData(false);
    
    // 递减时间计数
    TimeCount--;
    GameProgress.StepIt();

    // 如果时间到
    if (TimeCount == 0)
    {
        // 停止定时器
        KillTimer(1);
        
        // 清空地图
        m_GameC.ClearMap();
        RefreshBackground();
        UpdateMap();
        
        // 刷新界面
        InvalidateRect(NULL, TRUE);
        UpdateWindow();
        
        // 显示失败消息
        MessageBox(_T("时间到！游戏失败！"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
        
        // 设置按钮状态 - 只允许开始按钮可用
        SetButton(TRUE, FALSE, FALSE, FALSE);
        
        // 设置游戏状态为非进行中
        playing = false;
    }
    
    CDialogEx::OnTimer(nIDEvent);
}

// 音频相关函数实现
BOOL CGameDIg::InitAudio()
{
    // 确保音频文件存在
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // 检查背景音乐文件
    hFind = FindFirstFile(m_strBGMusicPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        // 文件不存在，但不阻止程序运行
        TRACE(_T("背景音乐文件不存在: %s\n"), m_strBGMusicPath);
    }
    else
    {
        FindClose(hFind);
    }

    // 检查点击音效文件
    hFind = FindFirstFile(m_strClickSoundPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("点击音效文件不存在: %s\n"), m_strClickSoundPath);
    }
    else
    {
        FindClose(hFind);
    }

    // 检查点击图片音效文件
    hFind = FindFirstFile(m_strClickPicSoundPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("点击图片音效文件不存在: %s\n"), m_strClickPicSoundPath);
    }
    else
    {
        FindClose(hFind);
    }

    // 检查消除音效文件
    hFind = FindFirstFile(m_strEliminateSoundPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("消除音效文件不存在: %s\n"), m_strEliminateSoundPath);
    }
    else
    {
        FindClose(hFind);
    }

    return TRUE;
}

BOOL CGameDIg::PlayBackgroundMusic()
{
    // 如果已经在播放，先停止
    if (m_bPlayingBGMusic)
    {
        StopBackgroundMusic();
    }

    // 使用MCI命令播放背景音乐(循环播放)
    CString strCommand;
    strCommand.Format(_T("open \"%s\" type mpegvideo alias BGMusic"), m_strBGMusicPath);

    if (mciSendString(strCommand, NULL, 0, NULL) != 0)
    {
        TRACE(_T("播放背景音乐失败: %s\n"), m_strBGMusicPath);
        return FALSE;
    }

    // 设置循环播放
    mciSendString(_T("play BGMusic repeat"), NULL, 0, NULL);
    m_bPlayingBGMusic = TRUE;

    return TRUE;
}

BOOL CGameDIg::StopBackgroundMusic()
{
    if (m_bPlayingBGMusic)
    {
        mciSendString(_T("stop BGMusic"), NULL, 0, NULL);
        mciSendString(_T("close BGMusic"), NULL, 0, NULL);
        m_bPlayingBGMusic = FALSE;
    }

    return TRUE;
}

BOOL CGameDIg::PlayClickSound()
{
    // 使用PlaySound API播放WAV音效
    PlaySound(m_strClickSoundPath, NULL, SND_FILENAME | SND_ASYNC);
    return TRUE;
}

BOOL CGameDIg::PlayClickPicSound()
{
    // 使用PlaySound API播放WAV音效
    PlaySound(m_strClickPicSoundPath, NULL, SND_FILENAME | SND_ASYNC);
    return TRUE;
}

BOOL CGameDIg::PlayEliminateSound()
{
    // 使用PlaySound API播放WAV音效
    PlaySound(m_strEliminateSoundPath, NULL, SND_FILENAME | SND_ASYNC);
    return TRUE;
}

void CGameDIg::CleanupAudio()
{
    // 停止背景音乐
    StopBackgroundMusic();
}

// 添加一个方法，用于刷新背景
void CGameDIg::RefreshBackground()
{
    // 获取位图信息
    BITMAP bmp;
    HBITMAP hBmp = (HBITMAP)::GetCurrentObject(m_dcBG.GetSafeHdc(), OBJ_BITMAP);
    CBitmap *pBitmap = CBitmap::FromHandle(hBmp);
    pBitmap->GetBitmap(&bmp);
    int origWidth = bmp.bmWidth;
    int origHeight = bmp.bmHeight;

    // 使用StretchBlt拉伸背景图片以填充整个窗口
    m_dcMem.StretchBlt(0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, &m_dcBG, 0, 0, origWidth, origHeight, SRCCOPY);
}

// 设置按钮点击事件
void CGameDIg::OnBnClickedBinSetting()
{
    // 添加测试消息框
    MessageBox(_T("设置按钮被点击！"), _T("调试信息"), MB_OK);

    // 播放点击音效
    PlayClickSound();

    // 创建设置对话框
    CGameSettingDlg dlg;

    // 设置初始值 - 使用ParamSetting类
    dlg.SetMapRows(ParamSetting::GetMapRow());
    dlg.SetMapCols(ParamSetting::GetMapCol());
    dlg.SetPicCount(ParamSetting::GetPicCount());
    dlg.SetThemeIndex(ParamSetting::ThemeNo);
    dlg.SetBGMusicOn(ParamSetting::BGMusicOn);

    // 显示对话框
    if (dlg.DoModal() == IDOK)
    {
        // 应用新的设置

        // 获取游戏当前状态
        bool wasPlaying = playing;

        // 如果正在游戏，先暂停
        if (wasPlaying)
        {
            // 暂停游戏
            playing = false;
            KillTimer(1);
        }

        // 更新BGPath、ElementPath和MaskPath
        BGPath = ParamSetting::BGPath;
        ElementPath = ParamSetting::ElementPath;
        MaskPath = ParamSetting::MaskPath;

        // 更新音频路径
        m_strBGMusicPath = ParamSetting::BGMSoundPath;
        m_strClickSoundPath = ParamSetting::ClickSoundPath;
        m_strClickPicSoundPath = ParamSetting::ClearSoundPath;

        // 重新初始化背景和元素
        InitBackground();
        InitElement(ElementPath, MaskPath);

        // 更新游戏区域
        m_GameRegion.right = MAP_LEFT + PIC_WIDTH * ParamSetting::GetMapCol();
        m_GameRegion.bottom = MAP_TOP + PIC_HEIGHT * ParamSetting::GetMapRow();

        // 处理背景音乐
        if (ParamSetting::BGMusicOn)
        {
            if (!m_bPlayingBGMusic)
                PlayBackgroundMusic();
        }
        else
        {
            if (m_bPlayingBGMusic)
                StopBackgroundMusic();
        }

        // 如果之前在游戏中，重新开始游戏
        if (wasPlaying)
        {
            // 重新开始游戏
            OnClickedBtnStart();
        }
        else
        {
            // 刷新界面
            InvalidateRect(NULL, TRUE);
        }
    }
}