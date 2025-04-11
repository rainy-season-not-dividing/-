#include "pch.h"
#include "CGameLogic.h"

CGameLogic::~CGameLogic()
{
    for (int i = 0; i < rows; i++)
    {
        delete[] GameMap[i];
    }
    delete[]GameMap;
}

int CGameLogic::GetElement(int nRow, int nCol)
{
    return GameMap[nRow][nCol];
}

//这里不做参数合理性判断，判断放在前面进行
void CGameLogic::InitMap(int nRows, int nCols, int nPicNum)
{
    //为二维数组开辟空间
    this->GameMap = new int* [nRows];
    for (int i = 0; i < nRows; i++)
    {
        GameMap[i] = new int[nCols];
        memset(GameMap[i], NULL, sizeof(int) * nCols);
    }
    //用图片编号填充地图
    int nRepeatNum = (nRows * nCols) / nPicNum;
    int nCount = 0;
    for (int i = 0; i < nPicNum; i++)
    {
        for (int j = 0; j < nRepeatNum; j++) {
            GameMap[nCount / nCols][nCount % nCols] = i;
            nCount++;
        }
    }
    //将图片随机打散
    srand((int)time(NULL));
    int nVertexNum = nRows * nCols;
    for (int i = 0; i < nVertexNum; i++)
    {
        int nIndex1 = rand() % nVertexNum;
        int nIndex2 = rand() % nVertexNum;
        //两个下表交换
        int nTmp = GameMap[nIndex1 / nCols][nIndex1 % nCols];
        GameMap[nIndex1 / nCols][nIndex1 % nCols] = GameMap[nIndex2 / nCols][nIndex2 % nCols];
        GameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
    }
}

//判断在Row行上，从Col1到Col2是否相连
bool CGameLogic::LinkX(int Row, int Col1, int Col2)
{
    if (Col1 > Col2)
    { //两个列上选择Col1为较小的列数
        int temp = Col1;
        Col1 = Col2;
        Col2 = temp;
    }
    for (int i = Col1 + 1; i <= Col2; i++)
    {
        //在同一列判断是否联通
        if (i == Col2)return true;
        if (GameMap[Row][i] != BLANK)return false;
    }
    return false;
}

//判断V1和V2在同一个Row上是否相连
bool CGameLogic::LinkInRow(Vertex V1, Vertex V2)
{
    int Row = V1.row;
    int Col1 = V1.col;
    int Col2 = V2.col;
    return LinkX(Row, Col1, Col2);
}

//判断在Col列上，从Row1到Row2是否相连
bool CGameLogic::LinkY(int Row1, int Row2, int Col)
{
    if (Row1 > Row2)
    { //两个行上选择Row1为较小的行数
        int temp = Row1;
        Row1 = Row2;
        Row2 = temp;
    }
    for (int i = Row1 + 1; i <= Row2; i++)
    {
        //在同一行判断是否联通
        if (i == Row2) return true;
        if (GameMap[i][Col] != BLANK)return false;
    }
    return false;
}

//判断V1和V2在同一个Col上是否相连
bool CGameLogic::LinkInCol(Vertex V1, Vertex V2)
{
    //思路和LinkInRow()完全一致
    int Row1 = V1.row;
    int Row2 = V2.row;
    int Col = V1.col;
    return LinkY(Row1, Row2, Col);
}

//判断V1和V2在经过一个拐角相连
bool CGameLogic::LinkOneCorner(Vertex V1, Vertex V2)
{
    if (GameMap[V1.row][V2.col] == BLANK)
    {
        if (LinkX(V1.row, V1.col, V2.col) && LinkY(V1.row, V2.row, V2.col))
        {
            Vertex V = { V1.row,V2.col,BLANK };
            verList.push(V);
            return true;
        }
    }
    if (GameMap[V2.row][V1.col] == BLANK)
    {
        if (LinkX(V2.row, V1.col, V2.col) && LinkY(V1.row, V2.row, V1.col))
        {
            Vertex V = { V2.row,V1.col,BLANK };
            verList.push(V);
            return true;
        }
    }
    return false;
}

//判断V1和V2在经过两个拐角相连
bool CGameLogic::LinkTwoCorner(Vertex V1, Vertex V2)
{
    for (int nCol = 0; nCol < Cols; nCol++)
    {
        if (GameMap[V1.row][nCol] == BLANK && GameMap[V2.row][nCol] == BLANK)
        {
            if (LinkY(V1.row, V2.row, nCol))
            {
                if (LinkX(V1.row, V1.col, nCol) && LinkX(V2.row, V2.col, nCol))
                {
                    Vertex vx1 = { V1.row,nCol,BLANK };
                    Vertex vx2 = { V2.row,nCol,BLANK };
                    verList.push(vx1);
                    verList.push(vx2);
                    return true;
                }
            }
        }
    }
    for (int nRow = 0; nRow < Rows; nRow++)
    {
        if (GameMap[nRow][V1.col] == BLANK && GameMap[nRow][V2.col] == BLANK)
        {
            if (LinkX(nRow, V1.col, V2.col))
            {
                if (LinkY(nRow, V1.row, V1.col) && LinkY(nRow, V2.row, V2.col))
                {
                    Vertex vx1 = { nRow,V1.col,BLANK };
                    Vertex vx2 = { nRow,V2.col,BLANK };
                    verList.push(vx1);
                    verList.push(vx2);
                    return true;
                }
            }
        }
    }
    return false;
}

//在地图pGameMap中判断，V1和V2是否相连
bool CGameLogic::IsLink(Vertex V1, Vertex V2)
{
    if (V1.row == V2.row && V1.col == V2.col)
    {
        //两个点坐标一致
        return false;
    }
    if (GetElement(V1.row, V1.col) != GetElement(V2.row, V2.col))
    {
        //两个点在地图中，所呈现的图片不一致
        return false;
    }

    ClearVerList();
    verList.push(V1);

    if (V1.row == V2.row)
    {
        if (LinkInRow(V1, V2))
        {
            verList.push(V2);
            return true;
        }
        else if (V1.row == 0)
        {
            //边界情况，第0行，则增加外边框的点作为路径
            verList.push({ V1.row - 1,V1.col,0 });
            verList.push({ V2.row - 1,V2.col,0 });
            verList.push(V2);
            return true;
        }
        else if (V1.row == Rows - 1)
        {
            //边界情况，第Rows-1行，则增加外边框的点作为路径
            verList.push({ V1.row + 1,V1.col,0 });
            verList.push({ V2.row + 1,V2.col,0 });
            verList.push(V2);
            return true;
        }
    }

    if (V1.col == V2.col)
    {
        if (LinkInCol(V1, V2))
        {
            verList.push(V2);
            return true;
        }
        else if (V1.col == 0)
        {
            //边界情况，第0列，则增加外边框的点作为路径
            verList.push({ V1.row ,V1.col - 1,0 });
            verList.push({ V2.row ,V2.col - 1,0 });
            verList.push(V2);
            return true;
        }
        else if (V1.col == Cols - 1)
        {
            //边界情况，第Cols-1列，则增加外边框的点作为路径
            verList.push({ V1.row,V1.col + 1,0 });
            verList.push({ V2.row,V2.col + 1,0 });
            verList.push(V2);
            return true;
        }
    }

    if (LinkOneCorner(V1, V2))
    {
        verList.push(V2);
        return true;
    }

    if (LinkTwoCorner(V1, V2))
    {
        verList.push(V2);
        return true;
    }

    return false;
}

//在地图GameMap中去掉，V1和V2两个图片
void CGameLogic::Clear(Vertex V1, Vertex V2)
{
    GameMap[V1.row][V1.col] = BLANK;
    GameMap[V2.row][V2.col] = BLANK;
}

//判断所有的图片是否已经全部消除
bool CGameLogic::isBlank()
{
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            if (this->GameMap[i][j] != BLANK)
            {
                return false;
            }
        }
    }
    return true;
}

//将堆栈verList的信息copy给copyList
stack<Vertex> CGameLogic::GetVerList()
{
    stack<Vertex> copyList = stack<Vertex>(verList);
    return copyList;
}

//清空堆栈操作,因为C++本身没有提供清空操作，因此编写函数完成清空操作
void CGameLogic::ClearVerList()
{
    //利用swap操作速度比pop快2/3
    stack<Vertex>().swap(verList);
}
