#pragma once

#include "global.h"
#include<iostream>
#include<stack>

using namespace std;

class CGameLogic
{
public:
    int** GameMap;     //��ͼ��Ϣ
    int rows;          //��ͼ��
    int cols;          //��ͼ��
    int picNum;        //��ͼ������ͼƬ�ĸ���
private:
    stack<Vertex> verList;  //���ڱ�������ͼƬ������·��
    
public:
    ~CGameLogic();
    int GetElement(int nRow, int nCol);
    void InitMap(int nRows, int nCols, int nPicNum);  //�൱�ڶ�GameMap���г�ʼ��
    bool IsLink(Vertex V1, Vertex V2);
    void Clear(Vertex V1, Vertex V2);
    void ClearVerList();
    stack<Vertex> GetVerList();
    bool isBlank();
    bool GetPrompt(Vertex& v1, Vertex& v2); //�ڵ�ͼ�в����������������ĵ㣬 ��v1��v2����
    void ResetMap();
private:
    bool LinkInRow(Vertex V1, Vertex V2);
    bool LinkInCol(Vertex V1, Vertex V2);
    bool LinkOneCorner(Vertex V1, Vertex V2);
    bool LinkTwoCorner(Vertex V1, Vertex V2);
    bool LinkY(int Row1, int Row2, int Col);
    bool LinkX(int Row, int Col1, int Col2);

};