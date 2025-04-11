#include "pch.h"
#include "CGameControl.h"

CGameControl::CGameControl() {
    // 构造函数体为空
}

int CGameControl::GetElement(int nRow, int nCol)
{
    return m_GameLogic.GetElement(nRow, nCol);
}

bool CGameControl::StartGame(int rows, int cols, int picNum)
{
    //首先判断3个参数是否合理，不合理，返回false
    //进行地图初始化，填充m_pGameMap返回true
    if ((rows * cols) % (picNum * 2) != 0)
        return false;        //这里进行了参数合理性判断
    else
        m_GameLogic.InitMap(rows, cols, picNum);
    return true;
}

void CGameControl::SetFirstPoint(int nRow, int nCol)
{
    selFirst.row = nRow;
    selFirst.col = nCol;
}

void CGameControl::SetSecondPoint(int nRow, int nCol)
{
    selSecond.row = nRow;
    selSecond.col = nCol;
}

bool CGameControl::Link(stack<Vertex>& verList)
{
    if (m_GameLogic.IsLink(selFirst, selSecond))
    {
        m_GameLogic.Clear(selFirst, selSecond);
        verList = m_GameLogic.GetVerList();
        return true;
    }
    return false;
}

bool CGameControl::isWin()
{
    return m_GameLogic.isBlank();
}

