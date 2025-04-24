int CGameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // 设置窗口大小
    SetWindowPos(NULL, 0, 0, GAMEWND_WIDTH, GAMEWND_HEIGHT, SWP_NOMOVE | SWP_NOZORDER);

    // 创建游戏控制对象
    m_gameControl = new CGameControl();

    return 0;
}