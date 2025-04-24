#pragma once
#include <afxstr.h>
#include "global.h" // ����global.h�Ի�ȡ�궨��

class ParamSetting
{
private:
    static ParamSetting *instance; // ����ʵ��

    // ˽�й��캯������ֹ�ⲿ����ʵ��
    ParamSetting();

public:
    static CString BGPath;         // ����ͼƬ·��
    static CString ElementPath;    // Ԫ��ͼƬ·��
    static CString MaskPath;       // ����ͼƬ·��
    static CString ClickSoundPath; // ���ͼƬ������
    static CString ClearSoundPath; // ����ͼƬ������
    static CString BGMSoundPath;   // ��������

    // ��Ҫʹ�ú�������Ϊ������
    static int mapRows; // ��ͼ��
    static int mapCols; // ��ͼ��
    static int picNum;  // ��ͼ������ͼƬ����

    static bool BGMusicOn; // �Ƿ�����������
    static int ThemeNo;    // ��ǰ���������

    // ��ȡ����ʵ��
    static ParamSetting *GetInstance();

    // ��ʼ��Ĭ�ϲ���
    void InitDefaultParams();

    // ��ȡ�����õ�ͼ����
    static int GetMapRow() { return mapRows; }
    static void SetMapRow(int rows) { mapRows = rows; }

    // ��ȡ�����õ�ͼ����
    static int GetMapCol() { return mapCols; }
    static void SetMapCol(int cols) { mapCols = cols; }

    // ��ȡ������ͼƬ����
    static int GetPicCount() { return picNum; }
    static void SetPicCount(int count) { picNum = count; }
};
