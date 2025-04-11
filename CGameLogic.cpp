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

//���ﲻ�������������жϣ��жϷ���ǰ�����
void CGameLogic::InitMap(int nRows, int nCols, int nPicNum)
{
    //Ϊ��ά���鿪�ٿռ�
    this->GameMap = new int* [nRows];
    for (int i = 0; i < nRows; i++)
    {
        GameMap[i] = new int[nCols];
        memset(GameMap[i], NULL, sizeof(int) * nCols);
    }
    //��ͼƬ�������ͼ
    int nRepeatNum = (nRows * nCols) / nPicNum;
    int nCount = 0;
    for (int i = 0; i < nPicNum; i++)
    {
        for (int j = 0; j < nRepeatNum; j++) {
            GameMap[nCount / nCols][nCount % nCols] = i;
            nCount++;
        }
    }
    //��ͼƬ�����ɢ
    srand((int)time(NULL));
    int nVertexNum = nRows * nCols;
    for (int i = 0; i < nVertexNum; i++)
    {
        int nIndex1 = rand() % nVertexNum;
        int nIndex2 = rand() % nVertexNum;
        //�����±���
        int nTmp = GameMap[nIndex1 / nCols][nIndex1 % nCols];
        GameMap[nIndex1 / nCols][nIndex1 % nCols] = GameMap[nIndex2 / nCols][nIndex2 % nCols];
        GameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
    }
}

//�ж���Row���ϣ���Col1��Col2�Ƿ�����
bool CGameLogic::LinkX(int Row, int Col1, int Col2)
{
    if (Col1 > Col2)
    { //��������ѡ��Col1Ϊ��С������
        int temp = Col1;
        Col1 = Col2;
        Col2 = temp;
    }
    for (int i = Col1 + 1; i <= Col2; i++)
    {
        //��ͬһ���ж��Ƿ���ͨ
        if (i == Col2)return true;
        if (GameMap[Row][i] != BLANK)return false;
    }
    return false;
}

//�ж�V1��V2��ͬһ��Row���Ƿ�����
bool CGameLogic::LinkInRow(Vertex V1, Vertex V2)
{
    int Row = V1.row;
    int Col1 = V1.col;
    int Col2 = V2.col;
    return LinkX(Row, Col1, Col2);
}

//�ж���Col���ϣ���Row1��Row2�Ƿ�����
bool CGameLogic::LinkY(int Row1, int Row2, int Col)
{
    if (Row1 > Row2)
    { //��������ѡ��Row1Ϊ��С������
        int temp = Row1;
        Row1 = Row2;
        Row2 = temp;
    }
    for (int i = Row1 + 1; i <= Row2; i++)
    {
        //��ͬһ���ж��Ƿ���ͨ
        if (i == Row2) return true;
        if (GameMap[i][Col] != BLANK)return false;
    }
    return false;
}

//�ж�V1��V2��ͬһ��Col���Ƿ�����
bool CGameLogic::LinkInCol(Vertex V1, Vertex V2)
{
    //˼·��LinkInRow()��ȫһ��
    int Row1 = V1.row;
    int Row2 = V2.row;
    int Col = V1.col;
    return LinkY(Row1, Row2, Col);
}

//�ж�V1��V2�ھ���һ���ս�����
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

//�ж�V1��V2�ھ��������ս�����
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

//�ڵ�ͼpGameMap���жϣ�V1��V2�Ƿ�����
bool CGameLogic::IsLink(Vertex V1, Vertex V2)
{
    if (V1.row == V2.row && V1.col == V2.col)
    {
        //����������һ��
        return false;
    }
    if (GetElement(V1.row, V1.col) != GetElement(V2.row, V2.col))
    {
        //�������ڵ�ͼ�У������ֵ�ͼƬ��һ��
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
            //�߽��������0�У���������߿�ĵ���Ϊ·��
            verList.push({ V1.row - 1,V1.col,0 });
            verList.push({ V2.row - 1,V2.col,0 });
            verList.push(V2);
            return true;
        }
        else if (V1.row == Rows - 1)
        {
            //�߽��������Rows-1�У���������߿�ĵ���Ϊ·��
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
            //�߽��������0�У���������߿�ĵ���Ϊ·��
            verList.push({ V1.row ,V1.col - 1,0 });
            verList.push({ V2.row ,V2.col - 1,0 });
            verList.push(V2);
            return true;
        }
        else if (V1.col == Cols - 1)
        {
            //�߽��������Cols-1�У���������߿�ĵ���Ϊ·��
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

//�ڵ�ͼGameMap��ȥ����V1��V2����ͼƬ
void CGameLogic::Clear(Vertex V1, Vertex V2)
{
    GameMap[V1.row][V1.col] = BLANK;
    GameMap[V2.row][V2.col] = BLANK;
}

//�ж����е�ͼƬ�Ƿ��Ѿ�ȫ������
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

//����ջverList����Ϣcopy��copyList
stack<Vertex> CGameLogic::GetVerList()
{
    stack<Vertex> copyList = stack<Vertex>(verList);
    return copyList;
}

//��ն�ջ����,��ΪC++����û���ṩ��ղ�������˱�д���������ղ���
void CGameLogic::ClearVerList()
{
    //����swap�����ٶȱ�pop��2/3
    stack<Vertex>().swap(verList);
}
