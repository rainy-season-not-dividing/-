#pragma once
#include "CGameLogicGraph.h"

class CGameControlGraph
{
public:
    Vertex selFirst;      // 选择的第一个点
    Vertex selSecond;     // 选择的第二个点
    Vertex helpFirst;     // 提示的第一个点
    Vertex helpSecond;    // 提示的第二个点
    CGameLogicGraph m_GameLogic;  // 实现具体的图结构逻辑
    
public:
    CGameControlGraph();  // 构造函数
    
    // 游戏操作接口
    int GetElement(int nRow, int nCol);  // 获取地图中的数据
    bool StartGame(int rows, int cols, int picNum); // 开始游戏
    void SetFirstPoint(int nRow, int nCol);  // 设置第一个选中点
    void SetSecondPoint(int nRow, int nCol); // 设置第二个选中点
    bool Link(stack<Vertex>& verList);  // 判断两点是否相连
    bool isWin();  // 判断游戏是否结束
    
    // 辅助功能
    void SetFirstHelpPoint(int nRow, int nCol);  // 设置第一个提示点
    void SetSecondHelpPoint(int nRow, int nCol); // 设置第二个提示点
    bool GetPrompt(stack<Vertex>& verList);  // 获取提示
    void ResetMap();  // 重新排列地图
    void ClearMap();  // 清空地图
}; 