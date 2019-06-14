#include <stdio.h>
#include <math.h>
//------------------
#include "include/matrix.h"
//------------------
//typedef double element;
//typedef struct mat{
//    int row;
//    int column;
//    element **a;
//} *matrix;
//
//matrix cross(matrix A,matrix B);
//matrix newmatrix(int r,int c);
//double measure(matrix A);
////----value is 0 if A is not a vector
//void normalize(matrix A);
//matrix transverse(matrix A);
////------------------------
//// int rank(matrix A);
//// matrix solve(matrix A,matrix A_bar);
//// element det(matrix A);
//// matrix reverse(matrix A);
//void duplicate(element k,matrix A);
//// matrix newcopy(matrix A);
////-------------------------
//void printmatrix(matrix A);
//void definematrix(matrix A);
//void dispose(matrix A);
//int disposematrix(matrix A);
//-------------------------
void printmatrix(matrix A){
//functional
    int i,j;
    for(i=0;i<A->row;i++){
        for(j=0;j<A->column;j++)
            printf("%5.2f",A->a[i][j]);
        printf("\n");
    }
}
void definematrix(matrix A){
//functional
    int i,j;
    for(i=0;i<A->row;i++)
        for(j=0;j<A->column;j++)
            scanf("%lf",&(A->a[i][j]));
}
double measure(matrix A){
//functional
    double phi;
    if(A->column==1){
        phi=cross(transverse(A),A)->a[0][0];
        return sqrt(phi);
    }
    else if(A->row==1)return measure(transverse(A));
    else return 0;
}
void normalize(matrix A){
//functional
    duplicate(1/measure(A),A);
    if(A->row==1&&A->column!=1)
        *A=*transverse(A);
}
// matrix newcopy(matrix A){
//     matrix X;
//     int i,j;
//     X=malloc(sizeof(struct ma));
//     X->column=A->column;
//     X->row=A->row;
//     X->a=malloc(sizeof(element)*A->column*A->row);
//     for(i=0;i<A->row;i++)
//         for(j=0;j<A->column;j++)
//             X->a[getindex(i,j,A)]=A->a[getindex(i,j,A)];
//     return X;
// }
matrix transverse(matrix A){
//--------probably a few "dispose" is needed,but functional
     int i,j;
     matrix AT;
     AT=newmatrix(A->column,A->row);
     for(i=0;i<A->row;i++)
         for(j=0;j<A->column;j++)
                 AT->a[j][i]=A->a[i][j];
     return AT;
}
void duplicate(element k,matrix A){
//functional
    int i,j;
    for(i=0;i<A->row;i++)
        for(j=0;j<A->column;j++)
            A->a[i][j]*=k;
}
matrix cross(matrix A,matrix B){
//functional
    int i,j,k;
    matrix C;
    if(A->column!=B->row)
        return;
    C=newmatrix(A->row,B->column);
    for(k=1;k<=C->column;k++)
        for(i=1;i<=C->row;i++)
            for(j=1;j<=A->column;j++)
                C->a[i-1][k-1]+=A->a[i-1][j-1]*B->a[j-1][k-1];
    return C;
}
matrix newmatrix(int r,int c){
//functional
    int i;
    matrix A;
    A=(matrix)malloc(sizeof(struct mat));
    A->column=c;
    A->row=r;
    A->a=(element**)calloc(r,sizeof(element*));
    for(i=0;i<r;i++)A->a[i]=(element*)calloc(c,sizeof(element));
    return A;
}
int disposematrix(matrix A){
    int i;
    for(i=0;i<A->row;i++)free(A->a[i]);
    free(A->a);
    free(A);
    return 0;
}
//---------------------
//int main(void){
//    matrix A,B;
//    A=newmatrix(3,1);
//    definematrix(A);
//    printmatrix(A);
//    printf("\n");
//    normalize(A);
//    printmatrix(A);
//    printf("%f",measure(A));
//    return 0;
//}
