#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "include\graphics.h"
#include "include\genlib.h"
#include "conio.h"
#include <math.h>
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
//-------------------------
#include "include/vertices.h"
#include "include/matrix.h"
#include "include/imgui.h"

#define NOMP -100
#define DELTATHETA (M_PI/24)
//-------------------------
struct GS
// {
    // matrix rotation;
    // matrix centre;
    // double size;
    // matrix sigma;
// }
 GroupStatus[VOLUME_GROUP];
//-------------------------
int selectedgroup=-1,rotatebit=0,selectedvertex=-1,vertexview=-1;
//-----------
// 不重复介绍
extern double cx,cy,dcx,dcy;
// 屏幕尺寸，cx,cy，自由视图中心；dcx,显示时左右两个视图中心的距离；dcy同理
extern pressedbutton mousepressed;
extern int updated,initbit,consolebit;
// initbit=1时代表处在初始状态；0代表已经打开过kernel
//-------------------------
double proportion=1;
static matrix RotationVector,CameraPosition;
static matrix xaxis,yaxis,zaxis;
static R_2 buffer[VOLUME_VERTICE];
/*
static double proportion=1;视图大小
static matrix RotationVector,CameraPosition;摄像机角度、位置
static matrix xaxis,yaxis,zaxis;标准xyz轴
static R_2 buffer[VOLUME_VERTICE];缓存数组，记录了所有点在屏幕上显示的位置
 */
//-------------------------
extern int GroupIndex,vtkey,egkey;
extern struct groupunity lastcount[VOLUME_GROUP];
extern vertice vt_i[VOLUME_VERTICE];
extern edge eg_i[VOLUME_EDGE];
extern crystaline *face_i[VOLUME_FACE],*group[VOLUME_GROUP];
extern VCollection groupVC[VOLUME_GROUP];
extern char currentstorage[50],target[50];
//-------------------------
//void display(double n);
static R_2 projection(int class,matrix r);//返回把点投影在屏幕的位置;class投影的视图，0是自由
matrix vector(vertice v);//把点换成向量方便运算
matrix spin(matrix axis,double theta);//返回旋转操作对应的矩阵
//-------------------------display interfaces
static void drawvertice(R_2 c);
static void drawedge(edge *e);
void DisplayCrystal(int g);
void display_all(int FREE);
void drawaxis(int g,int a);
void dotdash(double dx,double dy);
double bound(int g);
int inbound(int g,R_2 mouse,int view);
static double minimum(double x,double y);
//-------------------------
// static crystaline *listcry;
// static VCollection listvertice;
//-------------------------
matrix column(int i,matrix A);
void SpinGroup(int groupnumber,int axis,double theta);
void MoveGroup(int groupnumber,double dx,double dy,double dz);
void magnify(int groupnumber,double size);
//--------------------------
static void showedge(int view,edge *e);
//-------------------------6.13
void camcentralize(){
//probably all set
    int i,j;
    double sup[3],inf[3];
    // MoveGroup(GroupIndex,-cx,-cy,-cz);
    // for(i=0;i<3;i++)
    //     GroupStatus[GroupIndex].centre->a[i][0]=0;
    for(i=0;i<3;i++)
        sup[i]=inf[i]=0;

    for(i=0;i<GroupIndex;i++)
        for(j=0;j<3;j++){
            if(GroupStatus[i].centre->a[j][0]+GroupStatus[i].sigma->a[j][0]>sup[j])
                sup[j]=GroupStatus[i].centre->a[j][0]+GroupStatus[i].sigma->a[j][0];
            if(GroupStatus[i].centre->a[j][0]-GroupStatus[i].sigma->a[j][0]<inf[j])
                inf[j]=GroupStatus[i].centre->a[j][0]-GroupStatus[i].sigma->a[j][0];
        }
    for(i=0;i<3;i++)
        CameraPosition->a[i][0]=-0.5*(sup[i]+inf[i]);//-0.5 is wield
}
//------------------------------
double distance(R_2 c,R_2 mouse){
    return sqrt(pow(c.x-mouse.x,2)+pow(c.y-mouse.y,2));
}
int inbound(int g,R_2 mouse,int view){
    R_2 c;
    double d;
    c=projection(view,GroupStatus[g].centre);
    d=distance(c,mouse);
    if(d<=bound(g))return 1;
        else return 0;
}
//double bound(int g){
//    R_2 centre;
//    matrix axis;
//    double l,s[3]={0,0,0};
//    int i,j;
//    axis=cross(transverse(RotationVector),GroupStatus[g].rotation);
//    for(i=0;i<3;i++){
//        l=GroupStatus[g].sigma->a[i][0]*proportion;
//        for(j=0;j<3;j++)s[j]+=fabs(l*axis->a[j][i]);
//    }
//    for(i=0;i<3;i++)if(s[i]>s[0])s[0]=s[i];
//    return s[0];
//}
double bound(int g){
    double s[3];
    int i;
    for(i=0;i<3;i++)s[i]=GroupStatus[g].sigma->a[i][0];
    return s[0]+s[1]+s[2]-minimum(minimum(s[1],s[2]),s[0]);
}
static double minimum(double x,double y){
    return x<y?x:y;
}
void magnify(int g,double size){
    int j;
    for(j=lastcount[g].vertice+1;j<lastcount[g+1].vertice+1;j++){
        vt_i[j].x=GroupStatus[g].centre->a[0][0]+size*(vt_i[j].x-GroupStatus[g].centre->a[0][0]);
        vt_i[j].y=GroupStatus[g].centre->a[1][0]+size*(vt_i[j].y-GroupStatus[g].centre->a[1][0]);
        vt_i[j].z=GroupStatus[g].centre->a[2][0]+size*(vt_i[j].z-GroupStatus[g].centre->a[2][0]);
    }
    duplicate(size,GroupStatus[g].sigma);
}
void MoveGroup(int g,double dx,double dy,double dz){
    int j;
    GroupStatus[g].centre->a[0][0]+=dx;
    GroupStatus[g].centre->a[1][0]+=dy;
    GroupStatus[g].centre->a[2][0]+=dz;
    for(j=lastcount[g].vertice+1;j<lastcount[g+1].vertice+1;j++){
        vt_i[j].x+=dx;
        vt_i[j].y+=dy;
        vt_i[j].z+=dz;
    }
}
void SpinGroup(int g,int axis,double theta){//axis:x=0;
    matrix r,M,d;
    int j,k;
    M=spin(column(axis,GroupStatus[g].rotation),theta);
    for(j=lastcount[g].vertice+1;j<lastcount[g+1].vertice+1;j++){
        r=vector(vt_i[j]);
        for(k=0;k<3;k++)
            r->a[k][0]-=GroupStatus[g].centre->a[k][0];
        *r=*cross(M,r);//notice
        vt_i[j].x=r->a[0][0]+GroupStatus[g].centre->a[0][0];
        vt_i[j].y=r->a[1][0]+GroupStatus[g].centre->a[1][0];
        vt_i[j].z=r->a[2][0]+GroupStatus[g].centre->a[2][0];
    }
    // a=column(axis,GroupStatus[g].rotation);
    d=cross(M,GroupStatus[g].rotation);
    *GroupStatus[g].rotation=*d;//notice
//    drawaxis(g);
    rotatebit=axis+1;
    disposematrix(r);
    disposematrix(M);
    free(d);
    // disposematrix(d);
}
matrix column(int x,matrix A){
    matrix B;
    int i;
    B=newmatrix(3,1);
    for(i=0;i<3;i++)
        B->a[i][0]=A->a[i][x];
    return B;
}
int RegisterGroup(int i){
    int j;
    GroupStatus[i].rotation=newmatrix(3,3);
    GroupStatus[i].sigma=newmatrix(3,1);
    GroupStatus[i].size=1;
    GroupStatus[i].centre=newmatrix(3,1);
    for(j=0;j<3;j++)GroupStatus[i].rotation->a[j][j]=1;
    return 0;
}
void drawaxis(int g,int a){
//--------------lots of bugs fixed
    R_2 centre;
    // vertice v;
    matrix axis;
    double l;
    int i;
    // v.x=GroupStatus[g].centre->a[0][0];
    // v.y=GroupStatus[g].centre->a[1][0];
    // v.z=GroupStatus[g].centre->a[2][0];
    centre=projection(0,GroupStatus[g].centre);
    axis=cross(transverse(RotationVector),GroupStatus[g].rotation);
    for(i=0;i<3;i++){
        MovePen(centre.x+cx,cy+centre.y);
        l=0.5*GroupStatus[g].sigma->a[i][0]*proportion;
        SetPenSize(PENMIDDLE);
        if(i==a)
            SetPenColor("Blue");
            else SetPenColor("Dark Gray");
        dotdash(axis->a[0][i]*l,axis->a[2][i]*l);
        MovePen(centre.x+cx,cy+centre.y);
        dotdash(-axis->a[0][i]*l,-axis->a[2][i]*l);
    }
    disposematrix(axis);
}
void dotdash(double dx,double dy){
    double lx,ly;
    if(dx==0)
        if(dy==0)return;
    else {
        ly=dy>0?0.1:-0.1;
        dy=fabs(dy);
        while(dy>0){
            DrawLine(0,ly);
            MovePen(GetCurrentX(),GetCurrentY()+ly);
            dy-=fabs(ly);
        }
        return;
    }
    // SetPenColor("Blue");
    // SetPenSize(PENMIDDLE);
    lx=dx/sqrt(dx*dx+dy*dy)*0.1;
    ly=dy/sqrt(dx*dx+dy*dy)*0.1;
    dx=fabs(dx);
    while(dx>0){
        DrawLine(lx,ly);
        MovePen(GetCurrentX()+lx,GetCurrentY()+ly);
        dx-=fabs(lx);
    }
}
//-----------------------------
void display_all(int FREE){
//very primitive
    int i,j;
    // j,k;
    // for(i=0;i<GroupIndex;i++)
    //     // if(group[i])
    //         DisplayCrystal(i);
    SetPenSize(PENBOLDFACE);
    SetPenColor("Red");
    for(i=1;i<vtkey;i++)showvertice(0,vt_i[i]);
    SetPenSize(PENLIGHT);
    SetPenColor("Black");
    for(i=0;i<egkey;i++)if(eg_i[i].key==i)drawedge(eg_i+i);
    if(FREE)return;
    for(j=1;j<4;j++){
        SetPenSize(PENBOLDFACE);
        SetPenColor("Red");
        for(i=1;i<vtkey;i++)showvertice(j,vt_i[i]);
        SetPenSize(PENLIGHT);
        SetPenColor("Black");
        for(i=0;i<egkey;i++)if(eg_i[i].key==i)showedge(j,eg_i+i);
    }
    //     else {
    //         for(j=lastcount[i].vertice+1;j<lastcount[i+1].vertice+1;j++)
    //             drawvertice(projection(0,vector(vt_i[j])));
    //         if(i)k=lastcount[i-1].edge;else k=0;
    //         for(j=k;j<lastcount[i].edge;j++)drawedge(eg_i+j);
    // }
    // DisplayCrystal(listcry,listvertice);
}
// void DisplayCrystal(crystaline *L,VCollection VC){
//     crystaline *p;
//     VCollection v;
//     for(p=L;p;p=p->next)drawedge(p->e);
//     for(v=VC;v;v=v->next)drawvertice(projection(0,vector(*v->vp)));
// }
void DisplayCrystal(int g){
//------new version, with buffer
    crystaline *p;
    VCollection v;
    int key,j,k;
    // if(group[g])
    if(group[g])
    {
        SetPenSize(PENBOLDFACE);
        SetPenColor("Red");
        for(v=groupVC[g];v;v=v->next){
            key=v->vp->key;
            if(key==0)
                continue;
            buffer[key]=projection(0,vector(vt_i[key]));
            drawvertice(buffer[key]);
        }
        SetPenSize(PENLIGHT);
        SetPenColor("Black");
        for(p=group[g];p;p=p->next)drawedge(p->e);
    }
    else {
        SetPenSize(PENBOLDFACE);
        SetPenColor("Red");
        for(j=lastcount[g].vertice+1;j<lastcount[g+1].vertice+1;j++)
            if(vt_i[j].key==j){
                buffer[j]=projection(0,vector(vt_i[j]));
                drawvertice(buffer[j]);
            }
        if(g)k=lastcount[g-1].edge;else k=0;
        SetPenSize(PENLIGHT);
        SetPenColor("Black");
        for(j=k;j<lastcount[g].edge;j++)drawedge(eg_i+j);
    }
}
// void RotControl(int key,int event);
// void MovControl(int key);
//-------------------------
static void drawvertice(R_2 c){
//-------could use some optimization, such as prepare the color and size just once
    MovePen(cx+c.x,cy+c.y);
    DrawLine(0,0);
}
static void showedge(int view,edge *e){
    static R_2 v1,v2;
    matrix d1,d2;
    if(e->key==-1)return;
    d1=vector(vt_i[e->v1]);
    d2=vector(vt_i[e->v2]);
    v1=projection(view,d1);
    v2=projection(view,d2);
    MovePen(v2.x+cx,v2.y+cy);
    DrawLine(v1.x-v2.x,v1.y-v2.y);
    disposematrix(d1);
    disposematrix(d2);
}
static void drawedge(edge *e){
    if(e->key==-1)return;
    MovePen(buffer[e->v2].x+cx,buffer[e->v2].y+cy);
    DrawLine(buffer[e->v1].x-buffer[e->v2].x,buffer[e->v1].y-buffer[e->v2].y);
}
static R_2 projection(int class,matrix rbar){
//---------primitive version, equavilient to the map along y axis
// 0 :free, use camera;1:V;2:H;3:W
    R_2* f;
    matrix r;
    f=malloc(sizeof(R_2));
//---------------------
    if(class){
        switch(class){
            case 1:f->x=rbar->a[0][0]-dcx;f->y=rbar->a[2][0]+dcy;break;
            case 2:f->x=rbar->a[0][0]-dcx;f->y=rbar->a[1][0];break;
            case 3:f->x=-rbar->a[1][0];f->y=rbar->a[2][0]+dcy;break;
        }
        return *f;
    }
    r=cross(transverse(RotationVector),rbar);
    r->a[0][0]+=CameraPosition->a[0][0];
    // r->a[1][0]+=CameraPosition->a[1][0];
    r->a[2][0]+=CameraPosition->a[2][0];
    f->x=r->a[0][0]*proportion;
    f->y=r->a[2][0]*proportion;
//---------------------
    return *f;
    disposematrix(r);
}
matrix vector(vertice v){
    matrix r;
    r=newmatrix(3,1);
    r->a[0][0]=v.x;
    r->a[1][0]=v.y;
    r->a[2][0]=v.z;
    return r;
}
matrix spin(matrix axis,double theta){
    matrix M;
    int i;
    double alpha,beta,c,b,a;
    normalize(axis);
    c=axis->a[2][0];b=axis->a[1][0];a=axis->a[0][0];
    alpha=sin(theta);
    beta=cos(theta);
    M=cross(axis,transverse(axis));
    duplicate(1-beta,M);
    for(i=0;i<3;i++)M->a[i][i]+=beta;
    M->a[0][1]+=c*alpha;M->a[1][0]-=c*alpha;
    M->a[0][2]-=b*alpha;M->a[2][0]+=b*alpha;
    M->a[1][2]+=a*alpha;M->a[2][1]-=a*alpha;
//    p=cross(M,r);
//    return p;
    return M;
}
//------------------------
static int stabbedview(R_2 mp){
//-1 not selected any view, bugs...fixed
    int i,j,view;
    if(mp.x<-1.5*dcx||mp.y>dcy*1.5)return -1;
    i=(mp.x>-dcx/2);
    j=(mp.y>dcy/2);
    if(i&&j)view=3;
        else if(i&&!j)view=0;
            else if(!i&&j)view=1;
                else if(!i&&!j)view=2;
    return view;
}
void mousecontrol(int x,int y,int button,int event){
//whole-scale variable: vertexview,selectedvertex;
    int view=0,i,j;
    static int formerselect=0;
    static R_2 formermp={0,0};
    double d;
    R_2 mp;
    uiGetMouse(x,y,button,event);
    display();
    if(initbit)return;

    mp.x=InchesX(x)-cx;
    mp.y=GetWindowHeight()-InchesY(y)-cy;
    //----------------------------
        if(event==ROLL_UP&&initbit==0)proportion+=0.1;
        if(event==ROLL_DOWN&&initbit==0)proportion-=0.1;
        if(button==RIGHT_BUTTON&&event==KEY_DOWN){
            mousepressed.rightbutton+=1;
            formerselect=0;
        }
        if(button==MIDDLE_BUTTON)
            if(event==BUTTON_DOWN){
                mousepressed.middlebutton=1;
                formermp=mp;
            }
                else if(event==BUTTON_UP){
                    mousepressed.middlebutton=0;
                    formermp.x=formermp.y=NOMP;
                }
        if(button==LEFT_BUTTON)mousepressed.middlebutton=0;
    if(mousepressed.middlebutton&&event==MOUSEMOVE&&stabbedview(mp)==0){
            CameraPosition->a[0][0]-=(formermp.x-mp.x)/proportion;
            CameraPosition->a[2][0]-=(formermp.y-mp.y)/proportion;
            formermp=mp;
    }
    //----------------------------
        else if(event!=BUTTON_DOWN) return;
    selectedvertex=vertexview=-1;

    view=stabbedview(mp);
    if(view<1)return;

    for(i=0;i<GroupIndex;i++)
       if(inbound(i,mp,view)){
            for(j=lastcount[i].vertice+1;j<lastcount[i+1].vertice+1;j++)
                if(vt_i[j].key==j&&(d=distance(projection(view,vector(vt_i[j])),mp))<GRAIN){
                    selectedvertex=j;
                    vertexview=view;
                    break;
                }
       }
    //-------------------
    if(selectedvertex==-1){
        formerselect=0;
        mousepressed.rightbutton=0;
    }
        else if(mousepressed.rightbutton)
            if(formerselect){
                if(formerselect==selectedvertex){
                    formerselect=0;
                    mousepressed.rightbutton=0;
                }
                else {
                    if(mousepressed.rightbutton==1)
                        join(formerselect,selectedvertex);
                    else mousepressed.rightbutton=1;
                    formerselect=selectedvertex;
                }
            }
            else{
                formerselect=selectedvertex;
            }
    //--------------------
}
void RotControl(int key,int event){
//    matrix xaxis,zaxis,yaxis;
//    uiGetKeyboard(key,event);
//    display();
//    if(initbit)return;
    KeyboardEventProcess(key,event);
//
//    if(event==KEY_UP&&mousepressed.control==1&&key==VK_CONTROL)
//        mousepressed.control=0;
    if(event!=KEY_DOWN)return;
    updated=0;rotatebit=0;//severe problem has been found
    switch(key){
//        case VK_CONTROL:
//            if(event==KEY_DOWN)mousepressed.control=1;break;
        case VK_LEFT:
            *RotationVector=*cross(RotationVector,spin(zaxis,-DELTATHETA));break;
        case VK_RIGHT:
            *RotationVector=*cross(RotationVector,spin(zaxis,DELTATHETA));break;
        case VK_DOWN:
            *RotationVector=*cross(RotationVector,spin(xaxis,DELTATHETA));break;
        case VK_UP:
            *RotationVector=*cross(RotationVector,spin(xaxis,-DELTATHETA));break;
        case VK_ESCAPE:
            if(*currentstorage) FlushToDisk(currentstorage);
                else FlushToDisk("data1.obj");
            ExitGraphics();
            break;
        case 187:
            proportion*=(128.0/125.0);break;
        case 189:
            proportion/=(128.0/125.0);break;
        case VK_DELETE:
            DeleteVertice(selectedvertex);
            selectedvertex=vertexview=-1;
            break;
        default: updated=1;break;
    }
}
void MovControl(int key){
    CharEventProcess((char)key);
    updated=0;
    rotatebit=0;
    switch(key){
    case 'q':case 'Q':
        *RotationVector=*cross(RotationVector,spin(yaxis,-DELTATHETA));break;
    case 'e':case 'E':
        *RotationVector=*cross(RotationVector,spin(yaxis,DELTATHETA));break;
    case 'w':case 'W':
        CameraPosition->a[2][0]-=DELTAL/proportion;
        break;
    case 's':case 'S':
        CameraPosition->a[2][0]+=DELTAL/proportion;break;
    case 'a':case 'A':
        CameraPosition->a[0][0]+=DELTAL/proportion;break;
    case 'd':case 'D':
        CameraPosition->a[0][0]-=DELTAL/proportion;break;
    default:updated=1;break;
    }
    if(GroupIndex)if(selectedgroup==-1)selectedgroup=0;
    if(selectedgroup>=0)switch(key){
        case 'u':magnify(selectedgroup,0.8);break;
        case 'o':magnify(selectedgroup,1.2);break;
        case 'l':MoveGroup(selectedgroup,DELTAL,0,0);break;
        case 'j':MoveGroup(selectedgroup,-DELTAL,0,0);break;
        case 'i':MoveGroup(selectedgroup,0,0,DELTAL);break;
        case 'k':MoveGroup(selectedgroup,0,0,-DELTAL);break;
        case ' ':
    //        DisplayClear();display_all();
            // drawaxis(selectedgroup);
            break;
        case 't':SpinGroup(selectedgroup,0,-DELTATHETA);break;
        case 'g':SpinGroup(selectedgroup,0,DELTATHETA);break;
        case 'h':SpinGroup(selectedgroup,2,DELTATHETA);break;
        case 'f':SpinGroup(selectedgroup,2,-DELTATHETA);break;
        case 'r':SpinGroup(selectedgroup,1,DELTATHETA);break;
        case 'y':SpinGroup(selectedgroup,1,-DELTATHETA);break;
        case ',':case '<':if(selectedgroup)selectedgroup--;break;
        case '.':case '>':if(selectedgroup<GroupIndex-1)selectedgroup++;break;
        default: updated=1;break;
    }
}
//------------------------
void kernel(char *s)
{
    int i,j,k;
    CameraPosition=newmatrix(3,1);
    RotationVector=newmatrix(3,3);
    for(i=0;i<3;i++)
        RotationVector->a[i][i]=1;
    xaxis=newmatrix(3,1);xaxis->a[0][0]=1;
    zaxis=newmatrix(3,1);zaxis->a[2][0]=1;
    yaxis=newmatrix(3,1);yaxis->a[1][0]=1;
    InitGraphics();
    initextern();
//    cx = GetWindowWidth()/2;
//    cy = GetWindowHeight()/2;
    MovePen(cx,cy);
//    drawvertice(cx,cy);
//------------------------------
//     for(i=-1;i<2;i+=2)
//         for(j=-1;j<2;j+=2)
//             for(k=-1;k<2;k+=2){
//                 RegisterVertice(*newvertice(i,j,k));
//                 // if(vtkey)connect(vtkey-1,vtkey);
//             }
//    for(i=0;i<4;i++){
//        join(i,i+4);
//        join(2*i,2*i+1);
//    }
//    join(0,2);join(1,3);
//    join(4,6);join(5,7);
//------------------------------
//    for(i=1;i<5;i++)
//        for(j=1;j<5;j++)
//            if(j!=i)join(i,j);
    if(*s)LoadFromFile(s,0);
    // GenerateUnity(eg_i,&listcry);
    // listvertice=ExtractVertices(listcry);
//    DisplayCrystal(L);
//    display_all();
//    MovePen(cx,cy);
//    dotdash(5,0);
    registerKeyboardEvent(RotControl);
    registerCharEvent(MovControl);
    // registerMouseEvent(mousecontrol);
//-------------------------------
    return 0;
}
void showvertice(int view,vertice v){
    matrix d;
    if(!v.key)return;
    d=vector(v);
    buffer[v.key]=projection(view,d);
    drawvertice(buffer[v.key]);
    //disposematrix(d);
}
