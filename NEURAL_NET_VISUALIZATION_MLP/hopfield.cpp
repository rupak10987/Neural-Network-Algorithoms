#include<iostream>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include"MATRIX_SCRATCH.h"
#include"GRAPHICAL_VIEW_NEURO.h"
#include<math.h>
void print_matrix(float **A,int r,int c,const char* name);
int main()
{
    srand(time(0));
    int num_of_pattern_set;
    std::cout<<"input num of set patterns: ";
    std::cin>>num_of_pattern_set;
    int num_of_nodes;
    std::cout<<"input num of elements in each patterns: ";
    std::cin>>num_of_nodes;

    float ***INS;
    float **miu;
    INS= new float**[num_of_pattern_set];
    for(int i=0;i<num_of_pattern_set;i++)
    {
        INS[i]=new float*[1];
        miu=new float*[1];
        INS[i][0]=new float[num_of_nodes];
        miu[0]=new float[num_of_nodes];
    }

    for(int i=0;i<num_of_pattern_set;i++)
    {
        for(int j=0;j<num_of_nodes;j++)
        {
            std::cout<<"S"<<i<<" E"<<j<<" = ";
            std::cin>>INS[i][0][j];

        }std::cout<<"\n\n";
    }
    //miu
        for(int j=0;j<num_of_nodes;j++)
        {
            std::cout<<"Miu-"<<" E"<<j<<" = ";
            std::cin>>miu[0][j];

        }std::cout<<"\n\n";

    //calculate weights
float **WEIGHT;
float **prev_WEIGHT;
WEIGHT=new float*[num_of_nodes];
prev_WEIGHT=new float*[num_of_nodes];
for(int i=0;i<num_of_nodes;i++)
{
    WEIGHT[i]=new float[num_of_nodes];
    prev_WEIGHT[i]=new float[num_of_nodes];
}
//initialize both weight and prev weight to 0s
for(int i=0;i<num_of_nodes;i++)
    for(int j=0;j<num_of_nodes;j++)
{
    WEIGHT[i][j]=0;
    prev_WEIGHT[i][j]=0;
}

for(int i=0;i<num_of_pattern_set;i++)//calculate weights
{
//make a transpose of the ins
float **T_INS;
T_INS=new float*[num_of_nodes];
for(int j=0;j<num_of_nodes;j++ )
{
    T_INS[j]=new float[1];
}
MATRIX_OPS::MATRIX_Transpose(INS[i],1,num_of_nodes,T_INS);
//W=INS_t * INS
print_matrix(T_INS,num_of_nodes,1,"inverse X");
MATRIX_OPS::MATRIX_MUL(T_INS,num_of_nodes,1,INS[i],1,num_of_nodes,prev_WEIGHT);
print_matrix(prev_WEIGHT,num_of_nodes,num_of_nodes,"WEIGHT");
MATRIX_OPS::MATRIX_ADD_SAME(WEIGHT,num_of_nodes,num_of_nodes,prev_WEIGHT,num_of_nodes,num_of_nodes,WEIGHT);
}
print_matrix(WEIGHT,num_of_nodes,num_of_nodes,"Total WEIGHT");
//calculate new miu
float **new_miu;
new_miu=new float*[1];
new_miu[0]=new float[num_of_nodes];
while(true)
{
    std::cout<<"hello";
    MATRIX_OPS::MATRIX_MUL(miu,1,num_of_nodes,WEIGHT,num_of_nodes,num_of_nodes,new_miu);
    // hard threshold hobe
    MATRIX_OPS::MATRIX_HARD_THRESHOLD_TRANSFORM(new_miu,1,num_of_nodes,0);
    MATRIX_OPS::MATRIX_SUB_SAME(new_miu,1,num_of_nodes,miu,1,num_of_nodes,miu);
    int check=0;
    for(int k=0;k<num_of_nodes;k++)
    {
      check+=miu[0][k];
    }
    if(check==0)
    {
    break;
    }
    else
    {
    for(int k=0;k<num_of_nodes;k++)
    {
        miu[0][k]=new_miu[0][k];
    }
    print_matrix(miu,1,num_of_nodes,"miu(t+1)");
    }

}
    /*int gd = DETECT, gm;
    initwindow(500, 500);
    getch();
    closegraph();*/
}


void print_matrix(float **A,int r,int c,const char* name)
{
    std::cout<<name<<" : \n";
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            std::cout<<A[i][j]<<" ";
        }std::cout<<"\n";
    }
}
