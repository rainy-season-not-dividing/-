#include "pch.h"
#include "CGameControlGraph.h"

CGameControlGraph::CGameControlGraph()
{
    // 构造函数，初始化成员变量
}

int CGameControlGraph::GetElement(int nRow, int nCol)
{
    return m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControlGraph::StartGame(int rows, int cols, int picNum)
{
    // 参数合理性检查
    if ((rows * cols) % (picNum * 2) != 0)
        return false;
    
    // 初始化地图
    m_GameLogic.InitMap(rows, cols, picNum);
    return true;
}

void CGameControlGraph::SetFirstPoint(int nRow, int nCol)
{
    selFirst.row = nRow;
    selFirst.col = nCol;
    selFirst.info = m_GameLogic.GetElement(nRow, nCol);
}

void CGameControlGraph::SetSecondPoint(int nRow, int nCol)
{
    selSecond.row = nRow;
    selSecond.col = nCol;
    selSecond.info = m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControlGraph::Link(stack<Vertex>& verList)
{
    // 判断两点是否可以连接
    if (m_GameLogic.IsLink(selFirst, selSecond))
    {
        // 消除两点
        m_GameLogic.Clear(selFirst, selSecond);
        
        // 获取连接路径
        verList = m_GameLogic.GetVerList();
        
        return true;
    }
    
    return false;
}

bool CGameControlGraph::isWin()
{
    // 判断游戏是否获胜
    return m_GameLogic.isBlank();
}

void CGameControlGraph::SetFirstHelpPoint(int nRow, int nCol)
{
    helpFirst.row = nRow;
    helpFirst.col = nCol;
    helpFirst.info = m_GameLogic.GetElement(nRow, nCol);
}

void CGameControlGraph::SetSecondHelpPoint(int nRow, int nCol)
{
    helpSecond.row = nRow;
    helpSecond.col = nCol;
    helpSecond.info = m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControlGraph::GetPrompt(stack<Vertex>& verList)
{
    // 寻找可连接的一对点
    Vertex v1, v2;
    if (m_GameLogic.GetPrompt(v1, v2))
    {
        // 设置提示点
        SetFirstHelpPoint(v1.row, v1.col);
        SetSecondHelpPoint(v2.row, v2.col);
        
        // 获取连接路径
        verList = m_GameLogic.GetVerList();
        
        return true;
    }
    
    return false;
}

void CGameControlGraph::ResetMap()
{
    // 重新排列地图
    m_GameLogic.ResetMap();
}

void CGameControlGraph::ClearMap()
{
    // 清空地图
    m_GameLogic.ClearMap();
} 