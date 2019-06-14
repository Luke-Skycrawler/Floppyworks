#include <stdio.h>
#include "include/vertices.h"
#include "include/datastructure.h"
static int initial=1,f,l;
static edge *a[volume];
edge* popout(void){
    int k;
    if(initial)return NULL;
    	else if(l==f) initial=1;
    k=f,f=(f+1)%volume;
    return a[k];
}
int pushin(edge* x){
    if(initial) {
        initial=0;f=0;l=-1;
    }
    else if((l+1)%volume==f)return -1;
    a[l=(l+1)%volume]=x;
    return 0;
}
