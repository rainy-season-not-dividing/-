// ���嶥��ṹ��
typedef struct tagVertex {
    int row;            // ��
    int col;            // ��
    int info;           // ֵΪBLANK��ʾ��ǰ��û��ͼƬ
}Vertex;

// �궨��
#define GAMEWND_WIDTH    800            //��Ϸ������
#define GAMEWND_HEIGHT   600            //��Ϸ����߶�
#define MAP_TOP          50             //��ͼ������Ϸ�����Ϸ��ļ��
#define MAP_LEFT         20             //��ͼ������Ϸ�������ļ��
#define PIC_WIDTH        40             //ͼƬ�Ŀ��
#define PIC_HEIGHT       40             //ͼƬ�ĸ߶�
#define Rows             4              //��ͼ������
#define Cols             4              //��ͼ������
#define PicNum           4              //ͼƬ�ĸ���
#define BLANK            -1             //��ͼû��ͼƬ 