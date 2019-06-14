#include <string.h>
#include <stdio.h>
#include "include/vertices.h"
extern char currentstorage[50],target[50];
int saved=0;
//-----------------------
static char command[][20]={"new.vertex","new.face","connect","new.cube","new.cylinder",
"new.cone","save as","open","import","exit","save","help"};
//-----------------------
extern int initbit;
int cmd(void){
    char s[50]=" ";
    crystaline *L;
    int i,j,n,v1,v2,count=0;
    double x,y,z;
    do{
        if(*s!='/n'&&*s)printf("$");
        i=getchar();
        if(i==38){
            puts(s);
        }
        else{
            s[0]=i;
            gets(s+1);
        }
        if(*s=='.')return 0;
        for(i=0;i<12;i++)
            if(strcmp(s,command[i])==0)break;
        if(i<7&&initbit)continue;
        switch(i){
            case 0://new.v
                printf("arguments x y z\n>");
                scanf("%lf%lf%lf",&x,&y,&z);
                RegisterVertice(*newvertice(x,y,z));
                break;
            case 1:
                printf("arguments n R\n>");
                scanf("%d%lf",&n,&x);
                newface(n,x,0);
                break;
            case 2:
                printf("arguments v1 v2\n>");
                scanf("%d%d",&v1,&v2);
                join(v1,v2);
                break;
            case 4:
                printf("arguments n R h\n>");
                scanf("%d%lf%lf",&n,&x,&y);
                newcylinder(n,x,y);
                break;
            case 5:
                printf("arguments n R h\n>");
                scanf("%d%lf%lf",&n,&x,&y);
                newcone(n,x,y);
                break;
            case 8://import
                //scanface function needs a little bit of fixing
                if(!initbit){
                    do{
                        printf("opening ...(file.obj)\n>");
                        gets(s);
                    }
                    while(strcmp(s+strlen(s)-4,".obj"));
                    import(s);
                    return 0;
                    break;
                }
            case 7://open
                if(!initbit){
                    printf("error:running files\n");
                    break;
                }
                else {
                    do{
                        printf("opening ...(file.obj)\n>");
                        gets(s);
                    }
                    while(strcmp(s+strlen(s)-4,".obj"));
                    strcpy(currentstorage,s);
                    saved=-1;
                    kernel("");
                    import(s);
                    initbit=0;
                    return 0;
                }
            case 3:
                if(initbit){
                    kernel("8V.obj");return 0;
                }
                else import("8V.obj");
                break;
            case 9:printf("Closing...\n");return 0;//exit
            case 10:
                if(*currentstorage&&saved>=0){
                    FlushToDisk(currentstorage);
                    saved=1;
                    break;
                }//else goes to save as
            case 6://save as
                do{
                    printf("saving file as ...(filename.obj)\n>");
                    gets(s);
                }
                while(strcmp(s+strlen(s)-4,".obj"));
                strcpy(currentstorage,s);
                FlushToDisk(currentstorage);
                saved=1;
                break;
            default:
                if(count++>3)printf("...help");
                else break;//maybe some help
            case 11:for(j=0;j<11;j++)printf(">%s\n",command[j]);
        }
        if(i<12)count=0;
    }
    while(!feof((stdin)));
    return 0;
}
