#define _VERTICES_H
#include "matrix.h"
#define VOLUME_VERTICE 10000
#define VOLUME_FACE 10000
#define VOLUME_EDGE 20000
#define VOLUME_GROUP 100
// 全部是用来定义存放点、面、变、组数组的大小，顾名思义
#define PENBOLDFACE 8//用法setpensize(_),用来画点的大小
#define PENLIGHT 1//用来画线
#define PENMIDDLE 3//用来画轴
#define DELTAL 0.1//按键一次镜头移动一次的距离
#define GRAIN 0.05//鼠标识别点的颗粒度，调大即识别范围大

//typedef struct ListNode{
//    int generalkey;
//    struct ListNode *next;
//} listnode;
typedef struct{
    int v1;
    int v2;
    int key;
} edge;
typedef struct Crystal{
    edge* e;
    struct Crystal *next;
} crystaline;
typedef struct{
    double x;
    double y;
    double z;
    int key;
    crystaline *header;
} vertice;
typedef struct{
    double x;
    double y;
} R_2;
typedef struct vplist{
    vertice *vp;
    struct vplist *next;
} *VCollection;
struct groupunity{
    int vertice;
    int edge;
    int face;
};
struct GS{
    matrix rotation;
    matrix centre;
    double size;
    matrix sigma;
};
// 以上定义类型详见word文档
vertice* newvertice(double x,double y,double z);
vertice* KeyToVertice(int key);
int RegisterVertice(vertice p);
int join(int v1,int v2);
void RegisterEdge(edge p);//平时不直接用这个函数
void GenerateUnity(edge *e,crystaline **L);//生成联通的边集
VCollection ExtractVertices(crystaline *L);//从联通的边集中提取点集
int inList(edge *e,crystaline *L);
void FlushToDisk(char *s);//全部保存，s文件名
void LoadFromFile(char *s,int lastcount);//读取文件,s文件名，lastcount已经注册的点数量,open时是0,import时不是
void camcentralize();
int RegisterGroup(int i);//生成组的状态信息，像中心坐标、轴的朝向之类
double distance(R_2 p1,R_2 p2);//鼠标控制
// 点的流程：新建点(newvertice)->注册点(registervertice)->删除点(deletevertice)
//边的创建流程：一种是直接使用join(v1,v2)，必须是注册过的两点；一种是scanface直接把一个面的一圈边都读进来顺便连好
// 两种方法生成的边都是直接注册好的；如果删除就按它在数组里的指针来删除(deleteedge)
//------------------------
void DeleteVertice(int vk);
void DeleteEdge(int ek);
//------------------------
void kernel(char *s);//主程序的主函数
void consolecmd(void);
int cmd(void);//调用控制台，返回0就代表调用结束
void mousecontrol(int x,int y,int event,int key);//鼠标回调函数，整个程序里只能有一个，所以集成了
void display(void);//demoGui最下面的那个
void RotControl(int key,int event);
void MovControl(int key);
void KeyboardEventProcess(int key, int event);
void CharEventProcess(char ch);
//------------------------
//for console panel uses;
//functions newcone, newcylinder may have minor loading issues
void newcube(double a);
void newcone(int n,double R,double h);
void newsphere(double R);//no implementations
void newarc(double theta,int n,double R);//also this
void newface(int n,double R,double h);
void newclinder(int n,double R,double h);
void import(char *s);
//------------------------
void initextern(void);//used in function "kernel()", to set the variables to initial status
void showvertice(int view,vertice v);
//view和projection函数里的view变量功能一样，
//把一个点投影之后画出来，view选择视图
// ---------------------------------
//视图：1正视图；2：上视图；3：左视图；0：自由视图
//布局位置（按象限）:1:左；2：正；3：上
//用UTF-8编辑
// 涉及全局的函数就这些，还有一些矩阵的函数在matrix.h中做了简略的注释
