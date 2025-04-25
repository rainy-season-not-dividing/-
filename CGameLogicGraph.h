#pragma once

#include "global.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <list>

using namespace std;

// 图结构中的边类
class Edge {
public:
    int destVertex;    // 目标顶点
    int weight;        // 权重（可用于表示连接的复杂度）

    Edge(int dest, int w = 1) : destVertex(dest), weight(w) {}
};

// 图结构中的节点类
class GraphNode {
public:
    int row;           // 节点在游戏地图中的行
    int col;           // 节点在游戏地图中的列
    int value;         // 节点的值（图片ID或BLANK）
    list<Edge> edges;  // 该节点的边列表

    GraphNode(int r = 0, int c = 0, int v = BLANK) : row(r), col(c), value(v) {}
};

class CGameLogicGraph
{
public:
    vector<GraphNode> graphNodes;  // 图的节点集合
    int rows;          // 地图行数
    int cols;          // 地图列数
    int picNum;        // 地图包含的图片的个数
    int** GameMap;     // 二维数组形式的地图（便于索引）

private:
    stack<Vertex> verList;  // 用于保存两个图片相连的路径
    
public:
    CGameLogicGraph();
    ~CGameLogicGraph();
    
    // 基础操作
    int GetElement(int nRow, int nCol);
    void InitMap(int nRows, int nCols, int nPicNum);
    bool IsLink(Vertex V1, Vertex V2);
    void Clear(Vertex V1, Vertex V2);
    void ClearVerList();
    stack<Vertex> GetVerList();
    bool isBlank();
    bool GetPrompt(Vertex& v1, Vertex& v2);
    void ResetMap();
    void ClearMap();

private:
    // 图结构特有的方法
    int GetNodeIndex(int row, int col);
    void BuildGraph();
    void AddEdge(int startRow, int startCol, int endRow, int endCol, int weight = 1);
    bool FindPath(Vertex V1, Vertex V2);
    bool BFS(int startNodeIndex, int endNodeIndex, vector<int>& parent);
    void ReconstructPath(int startNodeIndex, int endNodeIndex, vector<int>& parent);
    int GetTurns(vector<int>& path);
}; 