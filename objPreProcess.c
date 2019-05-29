#include <stdio.h>
#include <string.h>
#define MAXL 100
/**delete the character in pos p. Returns new pos*/
char* strdel(char* p);
/**filter vt, vn, and its residue in f,
 * keep all the rest as comments,
 * delete use
*/
int objFilter();

char* strdel(char* p){
    if(*p)strcpy(p,p+1);
    return p;
}
int objFilter(){
    char filename[MAXL],buffer[MAXL];
    scanf("%s",filename);
    FILE* fin=fopen(filename,"r+");
    if(!fin) return -1;
    FILE* fout=fopen("a.obj","w");
    while (!feof(fin)){
        fgets(buffer,MAXL,fin);
        switch (buffer[0]){
            case 'f':{
                char* p=NULL;
                while (p=strchr(buffer,'/')){
                    strdel(p);
                    while (*p>='0'&&*p<='9')strdel(p);
                }
                fputs(buffer,fout);
                break;
            }
            case 'u':
            case '\0':break;
            case 'v':
                switch (buffer[1]){
                    case 't':
                    case 'n':
                        continue;
                        break;
                }
            default: 
                fputs(buffer,fout);
                break;
        }
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
int main(){
    objFilter();
    return 0;
}