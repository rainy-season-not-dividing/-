#pragma once
#include "CGameLogic.h"

class CGameControl
{
public:
    Vertex selFirst;  //ѡ��ĵ�һ����
    Vertex selSecond; //ѡ��ĵڶ�����
    CGameLogic m_GameLogic;  //ʵ�־�����߼�
public:
    CGameControl();  //�������Ĺ��캯��
    int GetElement(int nRow, int nCol);  //��ȡ��ͼ�е�����
    bool StartGame(int rows, int cols, int picNum);//��ʼ��Ϸ����CGameDlg�����
    void SetFirstPoint(int nRow, int nCol);
    void SetSecondPoint(int nRow, int nCol);
    bool Link(stack<Vertex>& verList);  //�ж�selFirst��selSecond�Ƿ�����,����·������verList��
    bool isWin();  //�ж���Ϸ�Ƿ����
};