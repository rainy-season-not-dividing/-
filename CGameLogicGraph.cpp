#include "pch.h"
#include "CGameLogicGraph.h"
#include <time.h>

CGameLogicGraph::CGameLogicGraph()
{
    GameMap = nullptr;
    rows = 0;
    cols = 0;
    picNum = 0;
}

CGameLogicGraph::~CGameLogicGraph()
{
    if (GameMap != nullptr)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] GameMap[i];
        }
        delete[] GameMap;
    }
}

int CGameLogicGraph::GetElement(int nRow, int nCol)
{
    if (nRow < 0 || nRow >= rows || nCol < 0 || nCol >= cols)
        return BLANK;
    return GameMap[nRow][nCol];
}

// 获取节点在图中的索引
int CGameLogicGraph::GetNodeIndex(int row, int col)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return -1;
    return row * cols + col;
}

// 初始化地图
void CGameLogicGraph::InitMap(int nRows, int nCols, int nPicNum)
{
    // 清理旧数据
    if (GameMap != nullptr)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] GameMap[i];
        }
        delete[] GameMap;
    }
    
    // 更新参数
    rows = nRows;
    cols = nCols;
    picNum = nPicNum;
    
    // 分配内存
    GameMap = new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        GameMap[i] = new int[cols];
        memset(GameMap[i], NULL, sizeof(int) * cols);
    }
    
    // 填充图片
    int nRepeatNum = (rows * cols) / picNum;
    int nCount = 0;
    for (int i = 0; i < picNum; i++)
    {
        for (int j = 0; j < nRepeatNum; j++) {
            GameMap[nCount / cols][nCount % cols] = i;
            nCount++;
        }
    }
    
    // 随机打乱图片位置
    srand((int)time(NULL));
    int nVertexNum = rows * cols;
    for (int i = 0; i < nVertexNum; i++)
    {
        int nIndex1 = rand() % nVertexNum;
        int nIndex2 = rand() % nVertexNum;
        // 交换两个位置的图片
        int nTmp = GameMap[nIndex1 / cols][nIndex1 % cols];
        GameMap[nIndex1 / cols][nIndex1 % cols] = GameMap[nIndex2 / cols][nIndex2 % cols];
        GameMap[nIndex2 / cols][nIndex2 % cols] = nTmp;
    }
    
    // 构建图结构
    BuildGraph();
}

// 构建图结构
void CGameLogicGraph::BuildGraph()
{
    // 清空原有图
    graphNodes.clear();
    
    // 创建所有节点
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            GraphNode node(i, j, GameMap[i][j]);
            graphNodes.push_back(node);
        }
    }
    
    // 连接所有相邻节点（四个方向）
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // 向右连接
            if (j + 1 < cols)
                AddEdge(i, j, i, j + 1);
            
            // 向下连接
            if (i + 1 < rows)
                AddEdge(i, j, i + 1, j);
        }
    }
}

// 添加边
void CGameLogicGraph::AddEdge(int startRow, int startCol, int endRow, int endCol, int weight)
{
    int startIndex = GetNodeIndex(startRow, startCol);
    int endIndex = GetNodeIndex(endRow, endCol);
    
    if (startIndex == -1 || endIndex == -1)
        return;
    
    // 双向连接
    graphNodes[startIndex].edges.push_back(Edge(endIndex, weight));
    graphNodes[endIndex].edges.push_back(Edge(startIndex, weight));
}

// 判断两个点是否可以连接
bool CGameLogicGraph::IsLink(Vertex V1, Vertex V2)
{
    // 判断两个点是否有效
    if (V1.row < 0 || V1.row >= rows || V1.col < 0 || V1.col >= cols ||
        V2.row < 0 || V2.row >= rows || V2.col < 0 || V2.col >= cols)
        return false;
    
    // 判断是否是同一个点
    if (V1.row == V2.row && V1.col == V2.col)
        return false;
    
    // 判断两个点的图片是否相同
    if (GameMap[V1.row][V1.col] != GameMap[V2.row][V2.col])
        return false;
    
    // 清空路径列表
    ClearVerList();
    verList.push(V1);
    
    // 使用图结构寻找路径
    return FindPath(V1, V2);
}

// 寻找连接路径
bool CGameLogicGraph::FindPath(Vertex V1, Vertex V2)
{
    int startNodeIndex = GetNodeIndex(V1.row, V1.col);
    int endNodeIndex = GetNodeIndex(V2.row, V2.col);
    
    if (startNodeIndex == -1 || endNodeIndex == -1)
        return false;
    
    // 使用BFS寻找路径
    vector<int> parent(rows * cols, -1);
    if (BFS(startNodeIndex, endNodeIndex, parent))
    {
        // 重建路径并存入verList
        ReconstructPath(startNodeIndex, endNodeIndex, parent);
        return true;
    }
    
    return false;
}

// 广度优先搜索寻找路径
bool CGameLogicGraph::BFS(int startNodeIndex, int endNodeIndex, vector<int>& parent)
{
    // 初始化访问标记
    vector<bool> visited(rows * cols, false);
    queue<int> q;
    
    // 将起点加入队列
    q.push(startNodeIndex);
    visited[startNodeIndex] = true;
    
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        
        // 找到终点
        if (current == endNodeIndex)
            return true;
        
        // 遍历当前节点的所有边
        for (const Edge& edge : graphNodes[current].edges)
        {
            int next = edge.destVertex;
            
            // 如果是空白位置或者是终点，并且未访问过
            if ((GameMap[graphNodes[next].row][graphNodes[next].col] == BLANK || next == endNodeIndex) && !visited[next])
            {
                // 记录路径
                parent[next] = current;
                visited[next] = true;
                q.push(next);
            }
        }
    }
    
    return false;
}

// 重建路径
void CGameLogicGraph::ReconstructPath(int startNodeIndex, int endNodeIndex, vector<int>& parent)
{
    // 从终点回溯到起点
    vector<int> path;
    for (int at = endNodeIndex; at != -1; at = parent[at])
    {
        path.push_back(at);
    }
    
    // 反转路径从起点到终点
    reverse(path.begin(), path.end());
    
    // 转换为Vertex结构并加入verList
    for (size_t i = 1; i < path.size(); i++)
    {
        int row = graphNodes[path[i]].row;
        int col = graphNodes[path[i]].col;
        Vertex v = {row, col, GameMap[row][col]};
        verList.push(v);
    }
}

// 计算路径中的转折次数
int CGameLogicGraph::GetTurns(vector<int>& path)
{
    if (path.size() < 3)
        return 0;
    
    int turns = 0;
    for (size_t i = 1; i < path.size() - 1; i++)
    {
        int prevRow = graphNodes[path[i-1]].row;
        int prevCol = graphNodes[path[i-1]].col;
        int currRow = graphNodes[path[i]].row;
        int currCol = graphNodes[path[i]].col;
        int nextRow = graphNodes[path[i+1]].row;
        int nextCol = graphNodes[path[i+1]].col;
        
        // 判断是否有转折（行列变化方向改变）
        bool horizontalMove1 = (prevRow == currRow);
        bool horizontalMove2 = (currRow == nextRow);
        
        if (horizontalMove1 != horizontalMove2)
            turns++;
    }
    
    return turns;
}

// 消除两个点
void CGameLogicGraph::Clear(Vertex V1, Vertex V2)
{
    // 清除地图上的两个点
    GameMap[V1.row][V1.col] = BLANK;
    GameMap[V2.row][V2.col] = BLANK;
    
    // 更新图结构中节点的值
    int index1 = GetNodeIndex(V1.row, V1.col);
    int index2 = GetNodeIndex(V2.row, V2.col);
    
    if (index1 != -1)
        graphNodes[index1].value = BLANK;
    
    if (index2 != -1)
        graphNodes[index2].value = BLANK;
}

// 清空路径列表
void CGameLogicGraph::ClearVerList()
{
    stack<Vertex>().swap(verList);
}

// 获取路径列表副本
stack<Vertex> CGameLogicGraph::GetVerList()
{
    return stack<Vertex>(verList);
}

// 判断地图是否已清空
bool CGameLogicGraph::isBlank()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (GameMap[i][j] != BLANK)
                return false;
        }
    }
    return true;
}

// 寻找可消除的一对点
bool CGameLogicGraph::GetPrompt(Vertex& V1, Vertex& V2)
{
    for (int row1 = 0; row1 < rows; row1++)
    {
        for (int col1 = 0; col1 < cols; col1++)
        {
            int firstVal = GameMap[row1][col1];
            if (firstVal == BLANK)
                continue;
            
            V1.row = row1;
            V1.col = col1;
            V1.info = firstVal;
            
            for (int row2 = 0; row2 < rows; row2++)
            {
                for (int col2 = 0; col2 < cols; col2++)
                {
                    // 跳过相同位置
                    if (row1 == row2 && col1 == col2)
                        continue;
                    
                    int secondVal = GameMap[row2][col2];
                    if (secondVal == BLANK || secondVal != firstVal)
                        continue;
                    
                    V2.row = row2;
                    V2.col = col2;
                    V2.info = secondVal;
                    
                    if (IsLink(V1, V2))
                        return true;
                }
            }
        }
    }
    
    return false;
}

// 重新排列地图
void CGameLogicGraph::ResetMap()
{
    // 收集所有非空图片
    vector<int> pictures;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (GameMap[i][j] != BLANK)
                pictures.push_back(GameMap[i][j]);
        }
    }
    
    // 随机打乱顺序
    random_shuffle(pictures.begin(), pictures.end());
    
    // 重新填充地图
    int index = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (GameMap[i][j] != BLANK && index < pictures.size())
            {
                GameMap[i][j] = pictures[index++];
                graphNodes[GetNodeIndex(i, j)].value = GameMap[i][j];
            }
        }
    }
}

// 清空地图
void CGameLogicGraph::ClearMap()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            GameMap[i][j] = BLANK;
            int nodeIndex = GetNodeIndex(i, j);
            if (nodeIndex != -1)
                graphNodes[nodeIndex].value = BLANK;
        }
    }
} 