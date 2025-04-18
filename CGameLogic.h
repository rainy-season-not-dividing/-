#pragma once

#include "global.h"
#include<iostream>
#include<stack>

using namespace std;

class CGameLogic
{
public:
    int** GameMap;     //地图信息
    int rows;          //地图行
    int cols;          //地图列
    int picNum;        //地图包含的图片的个数
private:
    stack<Vertex> verList;  //用于保存两个图片相连的路径
    
public:
    ~CGameLogic();
    int GetElement(int nRow, int nCol);
    void InitMap(int nRows, int nCols, int nPicNum);  //相当于对GameMap进行初始化
    bool IsLink(Vertex V1, Vertex V2);
    void Clear(Vertex V1, Vertex V2);
    void ClearVerList();
    stack<Vertex> GetVerList();
    bool isBlank();
    bool GetPrompt(Vertex& v1, Vertex& v2); //在地图中查找两个可以相连的点， 用v1和v2返回
    void ResetMap();
private:
    bool LinkInRow(Vertex V1, Vertex V2);
    bool LinkInCol(Vertex V1, Vertex V2);
    bool LinkOneCorner(Vertex V1, Vertex V2);
    bool LinkTwoCorner(Vertex V1, Vertex V2);
    bool LinkY(int Row1, int Row2, int Col);
    bool LinkX(int Row, int Col1, int Col2);

};