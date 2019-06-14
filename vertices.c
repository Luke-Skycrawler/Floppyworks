#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "include/vertices.h"
#include "include/datastructure.h"
//static listnode *findlast(listnode *header);
char currentstorage[50]="",target[50]="";
struct groupunity lastcount[VOLUME_GROUP];
int vtkey=0,egkey=0,fck=0;
int GroupIndex=0;
vertice vt_i[VOLUME_VERTICE];
edge eg_i[VOLUME_EDGE];
crystaline *face_i[VOLUME_FACE],*group[VOLUME_GROUP];
VCollection groupVC[VOLUME_GROUP];
/***************************** 全局变量表
char currentstorage[50]=""存储的位置
struct groupunity lastcount[VOLUME_GROUP]; 读这一组时，前面已经注册的点、线、面的数量，表明应该放到数组的哪个位置
int vtkey=0,egkey=0,fck=0;现在注册到几号；对应点、边、面
int GroupIndex=0;现在注册的组数
vertice vt_i[VOLUME_VERTICE];所有的点的存放方式
edge eg_i[VOLUME_EDGE];边
crystaline *face_i[VOLUME_FACE],*group[VOLUME_GROUP];面的存放，链表，放边，crystaline；组的边的存放...
必要时可以用extractvertices
VCollection groupVC[VOLUME_GROUP];没怎么用上
 *****************************/
//-----------------------------
extern struct GS GroupStatus[VOLUME_GROUP];//组的状态：中心，轴，范围
extern int selectedgroup,rotatebit,selectedvertex,vertexview,FREE;
//当前操作组；画图时用的一个位；当前选中的点的序号；当前选中的视图
extern double proportion,cx,dcx,cy,dcy;
//-----------------------------
static void disposelist(crystaline *L);
static void add(edge e,crystaline **L);
static void AddEdgeToVertice(edge e,int key);
//----------------------
static int collected(int v,VCollection L);
//VCollection ExtractVertices(crystaline *L);
int scanface(FILE *input,int lg);//输入面；从文件中读入时用到的模块,全部包办
void RegisterFace(crystaline *L);
void ComboJoin(int v1,int v2,crystaline **L);
static void scanedge(FILE *input);
//------------------------
//void newfacecopy(crystaline *face,double h);
//------------------------
//void GenerateUnity(edge *e,crystaline *L);
//edge *KeyToEdge(int key);
//static void AddEdgeToVertice(edge e,int key);
//int inList(edge *e,crystaline *L);
//static void disposelist(crystaline *L);
//static void addtounity(edge *e,crystaline *L);
//------------------------
//void showvertice(vertice v);
//-------------------6.5:fixed file output bug
//--------------------31
void initextern(void){
    int i;
    vertice blank={
        0,0,0,0,NULL
    };
    vtkey=1;egkey=GroupIndex=fck=lastcount[0].vertice=0;
    for(i=0;i<VOLUME_VERTICE;i++)vt_i[i]=blank;
    rotatebit=0;selectedgroup=selectedvertex=vertexview=-1;
    strcpy(currentstorage,"");
}
void import(char *s){
    // if(GroupIndex)
    //     LoadFromFile(s,lastcount[GroupIndex].vertice);
    // else LoadFromFile(s,1);
    LoadFromFile(s,lastcount[GroupIndex].vertice);
}
//---------------------5.30
void newface(int n,double R,double h){
    double phi;
    crystaline *L=NULL;
    int i,v=0,v0=0;
    phi=M_PI/n;
    for(i=1;i<n*2;i+=2){
        v=RegisterVertice(*newvertice(cos(phi*i),sin(phi*i),-h/2));
        if(v0)ComboJoin(v-1,v,&L);
            else v0=v;
    }
    ComboJoin(v,v0,&L);
    RegisterFace(L);
}
void newcylinder(int n,double R,double h){
    int i;
    crystaline *L;
    newface(n,R,h);
    newface(n,R,-h);
    for(i=0;i<n;i++)join(vtkey-1-i,vtkey-n-1-i);
    groupchore(R,h);
}
void groupchore(double R,double h){
    GenerateUnity(eg_i+egkey-1,group+GroupIndex);
    groupVC[GroupIndex]=ExtractVertices(group[GroupIndex]);
    lastcount[GroupIndex].edge=egkey;
    lastcount[GroupIndex].face=fck;
    RegisterGroup(GroupIndex);
    GroupStatus[GroupIndex].sigma->a[0][0]=R;
    GroupStatus[GroupIndex].sigma->a[1][0]=R;
    GroupStatus[GroupIndex].sigma->a[2][0]=h;
    if(GroupIndex==0)lastcount[0].vertice=0;
    GroupIndex++;
    lastcount[GroupIndex].vertice=vtkey-1;
}
void newcone(int n,double R,double h){
    int i;
    crystaline *L;
    newface(n,R,h);
    RegisterVertice(*newvertice(0,0,h/2));
    for(i=0;i<n;i++)join(vtkey-1,vtkey-i-2);
    groupchore(R,h);
}
//-----deleted stuffs causes bugs//update:bugs are dead
void DeleteVertice(int vk){
    crystaline *p;
    for(p=vt_i[vk].header;p;p=p->next)
        DeleteEdge(p->e->key);
    disposelist(vt_i[vk].header);
    vt_i[vk].key=0;
}
void DeleteEdge(int ek){
    // crystaline *p;
    if(ek>=0)eg_i[ek].key=-1;
}
//------------------------5.22
int scanface(FILE *input,int lastgroup){
//(done)all scanf should be replaced by fscanf,consider adding a FILE *
    int error,v0,v,vold;
    char c='f';
    crystaline *L=NULL;
    do{
        fscanf(input,"%d%d",&v0,&vold);
        ComboJoin(v0+lastgroup,vold+lastgroup,&L);
        while(fscanf(input,"%d",&v)==1){
            ComboJoin(vold+lastgroup,v+lastgroup,&L);
            vold=v;
        }
        ComboJoin(v+lastgroup,v0+lastgroup,&L);
        RegisterFace(L);
        L=NULL;
    }
    while(fscanf(input,"%c",&c)==1&&c=='f');
    if(feof(input))return 0;
    else return 1;
}
void RegisterFace(crystaline *L){
    face_i[fck++]=L;
}
void ComboJoin(int v1,int v2,crystaline **L){
    edge *ep;
    join(v1,v2);
    ep=calloc(1,sizeof(edge));
    ep->v1=v1;ep->v2=v2;
    add(*ep,L);
}
//------------------------
static int collected(int v,VCollection L){
    VCollection p;
    for(p=L;p;p=p->next)
        if(p->vp->key==v)return 1;
    return 0;
}
VCollection ExtractVertices(crystaline *L){
    crystaline *p;
    VCollection VC=NULL,nvc;
    for(p=L;p;p=p->next){
        if(!collected(p->e->v1,VC)){
            nvc=calloc(1,sizeof(struct vplist));
            nvc->vp=KeyToVertice(p->e->v1);
            nvc->next=VC;
            VC=nvc;
        }
        if(!collected(p->e->v2,VC)){
            nvc=calloc(1,sizeof(struct vplist));
            nvc->vp=KeyToVertice(p->e->v2);
            nvc->next=VC;
            VC=nvc;
        }
    }
    return VC;
}
//---------------------------
void FlushToDisk(char *outputs){
    int i,j,k,fcount=0;
    FILE *output;
    crystaline *p;
    char s[30];
    output=fopen(outputs,"w");
    for(j=0;j<GroupIndex;j++){
        fprintf(output,"g Object%03d\n",j+1);
        for(i=lastcount[j].vertice+1;i<lastcount[j+1].vertice+1;i++)
            if(vt_i[i].key==i)
                fprintf(output,"v %8.6f %8.6f %8.6f\n",vt_i[i].x,vt_i[i].y,vt_i[i].z);
        if(j)k=lastcount[j-1].face;else k=0;
        for(i=k;i<lastcount[j].face;i++){
            memset(s,0,30);
            sprintf(s,"f");
            for(p=face_i[i];p;p=p->next)
                if(p->e->key!=-1){
                    fcount++;
                    // sprintf(strlen(s)+s," %d",p->e->v1-lastcount[j].vertice);
                    sprintf(strlen(s)+s," %d",p->e->v1);
                }
            if(fcount>2)fprintf(output,"%s\n",s);
            fcount=0;
        }
    }
        //-------------------
        //     for(i=0;i<egkey;i++)
        //        fprintf(output,"l %d %d\n",eg_i[i].v1,eg_i[i].v2);
        //-------------------
    fclose(output);
}
static double maxchore(double x,double y,double z,double a,double b,double c){
    x=fabs(x)>fabs(y)?fabs(x):fabs(y);
    x=x>fabs(z)?x:fabs(z);
    x=x>fabs(a)?x:fabs(a);
    x=x>fabs(b)?x:fabs(b);
    x=x>fabs(c)?x:fabs(c);
    return x;
}
void LoadFromFile(char *inputs,int lg){
    char c,*s[31];
    double x,y,z,mx=0,my=0,mz=0,ix,iy,iz,sup;
    FILE *input;
    int v1,v2,e=0,i;
    input=fopen(inputs,"r");
    if(input==NULL)return;
    // vtkey=1;egkey=0;lastcount[0].vertice=0;
    do{
        mx=my=mz=ix=iy=iz=sup=0;
        fgets(s,30,input);
        do{
            c=fgetc(input);
            if(c=='v'||c=='V'){
                e=fscanf(input,"%lf%lf%lf\n",&x,&y,&z);
                if(x>mx)mx=x; if(x<ix)ix=x;
                if(y>my)my=y; if(y<iy)iy=y;//a bug was found here
                if(z>mz)mz=z; if(z<iz)iz=z;
                if(e==EOF)break;
                RegisterVertice(*newvertice(x,y,z));
            }
        }
        while(c=='v'||c=='V');
        while(c=='\n')c=fgetc(input);
        if(c=='f'||c=='F')c='g'*scanface(input,lg);//important line
        // if(c=='f'||c=='F')if(lg)c='g'*scanface(input,lastcount[GroupIndex].vertice);
        //     else c='g'*scanface(input,0);
        // if(c=='l'||c=='L')scanedge(input);
        // while((c=='l'||c=='L')&&e!=EOF){
        //     e=fscanf(input,"%d%d\n%c",&v1,&v2,&c);
        //     join(v1,v2);
        // }
        group[GroupIndex]=NULL;
        if(vtkey+1-lastcount[GroupIndex].vertice>100||vtkey+1==lastcount[GroupIndex].vertice){
        //    group[GroupIndex]=NULL;/*awaited to be completed*/
        }
        else{
            GenerateUnity(eg_i+egkey-1,group+GroupIndex);
            groupVC[GroupIndex]=ExtractVertices(group[GroupIndex]);
        }
        lastcount[GroupIndex].edge=egkey;
        lastcount[GroupIndex].face=fck;
        RegisterGroup(GroupIndex);
        GroupStatus[GroupIndex].sigma->a[0][0]=0.5*(mx-ix);
        GroupStatus[GroupIndex].sigma->a[1][0]=0.5*(my-iy);
        GroupStatus[GroupIndex].sigma->a[2][0]=0.5*(mz-iz);
        GroupStatus[GroupIndex].centre->a[2][0]=0.5*(mz+iz);
        GroupStatus[GroupIndex].centre->a[0][0]=0.5*(mx+ix);
        GroupStatus[GroupIndex].centre->a[1][0]=0.5*(my+iy);
        // mx=(mx-ix)/2; my=(my-iy)/2; mz=(mz-iz)/2;
        sup=maxchore(mx,my,mz,ix,iy,iz);
        if(sup*proportion>dcy){
            proportion=dcy/sup;
            if(FREE==0){
                cx=cx-dcx/2;
                cy*=2;
                FREE=1;
            }
        }
        GroupIndex++;
        lastcount[GroupIndex].vertice=vtkey-1;
    }
    while((c=='g'||c=='G')&&!(feof(input)));
    fclose(input);
    camcentralize();
}
static void scanedge(FILE *input){
    char c='l';
    int e=1,v1,v2;
    while(e!=EOF&&(c=='l'||c=='L')){
        fscanf(input,"%d%d\n",&v1,&v2);
        join(v1,v2);
        e=fscanf(input,"%c",&c);
    }
}
//--------------primitive version
vertice *KeyToVertice(int key){
    // if(vt_i[key].key!=0)
    return vt_i+key;
}
//--------------------------
void RegisterEdge(edge p){
    eg_i[p.key]=p;
}
int RegisterVertice(vertice v){
    v.key=vtkey;
    vt_i[vtkey++]=v;
    return vtkey-1;
}
//-------------------
vertice* newvertice(double x,double y,double z){
    vertice *p;
    p=(vertice*)calloc(1,sizeof(vertice));
    p->x=x;
    p->y=y;
    p->z=z;
    p->header=NULL;
    return p;
}
//-----------------------
static void add(edge e,crystaline **L){
    crystaline *p;
    p=calloc(1,sizeof(crystaline));
    p->e=calloc(1,sizeof(edge));
    *(p->e)=e;
    p->next=*L;
    *L=p;
}
int inList(edge *e,crystaline *L){
    crystaline *p;
    p=L;
    while(p){
        if(p->e->key==e->key)return 1;
        p=p->next;
    }
    return 0;
}
static void disposelist(crystaline *L){
    if(L==NULL)return;
    if(L->next==NULL)free(L);
        else disposelist(L->next);
}
//---------------------------
// edge *KeyToEdge(int key){
//     return eg_i+key;
// }
//crystaline *newsolid(char *type);
//---------------------
//crystaline *newsolid(char *t){
//    crystaline *L;
//    L=malloc(sizeof(crystaline));
//
//}
//listnode *FindHeader(int verticekey){
//    vertice *v;
//    v=KeyToVertice(verticekey);
//    return v->header;
//}
static void AddEdgeToVertice(edge e,int vcode){
    vertice *v;
    v=KeyToVertice(vcode);
    add(e,&(v->header));
}
int join(int v1,int v2){
//cool
    edge e;
    crystaline *p1;
    int joined=0;
    if(KeyToVertice(v1)->header&&KeyToVertice(v2)->header){
        for(p1=KeyToVertice(v1)->header;p1->next;p1=p1->next)
            if(p1->e->v2==v2||p1->e->v1==v2)break;
        if(p1->e->v2==v2||p1->e->v1==v2) joined=1;
    }
    if(joined)return -1;
    e.key=egkey++;
    e.v1=v1;e.v2=v2;
    RegisterEdge(e);
    AddEdgeToVertice(e,v1);
    AddEdgeToVertice(e,v2);
    return 0;
}
void GenerateUnity(edge *e,crystaline **L){
//----O(E^2)algorithm
    crystaline *p;
    disposelist(*L);
    pushin(e);
    do{
        do e=popout();
        while(e&&inList(e,*L));
//        if(e)showedge(*e);
        if(e==NULL)break;
        add(*e,L);
        for(p=KeyToVertice(e->v1)->header;p;p=p->next) if(!inList(p->e,*L)) pushin(p->e);
        for(p=KeyToVertice(e->v2)->header;p;p=p->next) if(!inList(p->e,*L)) pushin(p->e);
    }
    while(1);
}
//--------------------
// int main(void){
//     int i,j,k;
//     for(i=-1;i<2;i+=2)
//         for(j=-1;j<2;j+=2)
//             for(k=-1;k<2;k+=2){
//                 RegisterVertice(*newvertice(i,j,k));
//                 // if(vtkey)connect(vtkey-1,vtkey);
//             }
//    for(i=0;i<8;i++)showvertice(vt_i[i]);
//    for(i=0;i<4;i++){
//        join(i,i+4);
//        join(2*i,2*i+1);
//    }
//    join(0,2);join(1,3);
//    join(4,6);join(5,7);
//    for(i=0;i<12;i++)showedge(eg_i[i]);
//    return 0;
// }
// void showvertice(vertice v){
//    printf("%f %f %f\n",v.x,v.y,v.z);
// }
// void showedge(edge e){
//    printf("//");
//    showvertice(*KeyToVertice(e.v1));
//    showvertice(*KeyToVertice(e.v2));
//    printf("//");
// }
