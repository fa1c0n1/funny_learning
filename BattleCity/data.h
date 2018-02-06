#ifndef _DATA_H
#define _DATA_H

#include "comm.h"

#include <time.h>

//��ͼ��ÿ���������
typedef enum {
	SIGN_EMPTY,
	SIGN_WALL0,       //��ǽ(���ɻ���)
	SIGN_WALL1,       //ˮ��ǽ(�ɻ���)
	SIGN_TANK_PA,     //���̹��A
	SIGN_TANK_PB,     //���̹��B 
	SIGN_TANK_E0,     //�о�̹��0
	SIGN_TANK_E1,     //�о�̹��1
	SIGN_BULLET,      //�ӵ�
	SIGN_GRASS,       //�ݵ�
	SIGN_RIVER        //����
} ObjType;

//����
typedef enum {
	DRT_UP,
	DRT_DOWN,
	DRT_LEFT,
	DRT_RIGHT,
	DRT_NONE
} Direction;

typedef ObjType TankType;

typedef struct {
	int nX;
	int nY;
} Point;

//��ͼ�ϵĶ���
typedef struct {
	uint bulValid : 1;     //�ӵ��Ƿ���Ч
	uint bDead : 1;        //̹���Ƿ�����
	Direction eDrt : 4;    //����
	ObjType eType : 8;     //��ǽ ��ǽ �ݵ� ���� ɳ�ص�, ̹��A ̹��B �о�̹�˵�
	uint nX : 8;          //�������Ͻǵĺ�����
	uint nY : 8;          //�������Ͻǵ�������
	ObjType bulOwner : 8; //�ӵ�����
	uint nBulletID : 16;  //�ӵ�ID
	uint nEnID : 16;  //�о�ID
} Object;

typedef Object Tank;
typedef Object Bullet;

typedef struct {
	uint nKey;         //��Ӧ�İ���
	int(*func)(void); //��Ӧ�ĺ���
	char szText[64];   //��Ӧ������˵��
} MenuItem;


//ǰ��ɫ
#define FG_BLUE             0x0001
#define FG_GREEN            0x0002
#define FG_TURQUOISE        0x0003
#define FG_RED              0x0004
#define FG_PURPLE           0x0005
#define FG_YELLOW           0x0006
#define FG_WHITE            0x0007
#define FG_GRAY             0x0008
#define FG_LIGHTBLUE        0x0009
#define FG_LIGHTGREEN       0x000A
#define FG_LIGHTTURQUOISE   0x000B
#define FG_LIGHTRED         0x000C
#define FG_LIGHTPURPLE      0x000D
#define FG_LIGHTYELLOW      0x000E
#define FG_HIGHWHITE        0x000F

//����ɫ
#define BG_BLUE             0x0010
#define BG_GREEN            0x0020
#define BG_TURQUOISE        0x0030
#define BG_RED              0x0040
#define BG_PURPLE           0x0050
#define BG_YELLOW           0x0060
#define BG_WHITE            0x0070
#define BG_GRAY             0x0080
#define BG_LIGHTBLUE        0x0090
#define BG_LIGHTGREEN       0x00A0
#define BG_LIGHTTURQUOISE   0x00B0
#define BG_LIGHTRED         0x00C0
#define BG_LIGHTPURPLE      0x00D0
#define BG_LIGHTYELLOW      0x00E0
#define BG_HIGHWHITE        0x00F0

//����ֵ
#define KEY_0               48
#define KEY_1               49
#define KEY_2               50
#define KEY_3               51
#define KEY_4               52
#define KEY_5               53
#define KEY_6               54
#define KEY_ENTER           13
#define KEY_EXIT            27
#define KEY_UP              72
#define KEY_DOWN            80
#define KEY_LEFT            75
#define KEY_RIGHT           77
#define KEY_DIRECTION       224

//BulletBox������
#define BOX_CAPACITY        100

//�о����������ֵ
#define ENEMY_NMAX           5

extern int g_Map[40][40];          //��ŵ�ͼ�ĸ�����
extern int g_TankStatus[4][3][3];  //̹�����������ĸ������״̬
extern Bullet *g_pBulletBox;       //�ӵ��ֿ�
extern Tank *g_pTankA;             //���A
extern Tank *g_pTankB;             //���B
extern Tank *g_pEnemies;           //�о�
extern int g_nEnNum;               //�о�����
extern ObjType g_eNodeType;        //��ʾ��ͼ�ϵ�����ͣ��༭��ͼʱ�����л�����

extern clock_t g_startTime;        

#endif  // _DATA_H