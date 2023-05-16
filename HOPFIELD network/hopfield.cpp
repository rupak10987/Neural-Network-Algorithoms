#include<iostream>
#include<time.h>
#include<conio.h>
#include<graphics.h>
#include"MATRIX_SCRATCH.h"
#include"file.h"
#include<math.h>
void print_matrix(float **A,int r,int c,const char* name);
void draw_miu(float**miu,int num_of_nodes);
int main()
{
    srand(time(0));
    int gd = DETECT, gm;
    initwindow(700, 700);

    //initializations
    int num_of_pattern_set;
    std::cout<<"input num of set patterns: ";
    std::cin>>num_of_pattern_set;
    int num_of_nodes;
    std::cout<<"input num of elements in each patterns: ";
    std::cin>>num_of_nodes;


    //pattern sets dimensions to be remembered
    float ***INS;
    float **miu;
    INS= new float**[num_of_pattern_set];
    for(int i=0; i<num_of_pattern_set; i++)
    {
        INS[i]=new float*[1];
        miu=new float*[1];
        INS[i][0]=new float[num_of_nodes];
        miu[0]=new float[num_of_nodes];
    }



    std::cout<<"MANUALLY INPUT[M]/INPUT FROMFILE-GRAPHICS[F] : ";
    char c;
    std::cin>>c;



    if(c=='M')
    {
        for(int i=0; i<num_of_pattern_set; i++)
        {
            for(int j=0; j<num_of_nodes; j++)
            {
                std::cout<<"S"<<i<<" E"<<j<<" = ";
                std::cin>>INS[i][0][j];

            }
            std::cout<<"\n\n";
        }
        for(int j=0; j<num_of_nodes; j++)
        {
            std::cout<<"Miu-"<<" E"<<j<<" = ";
            std::cin>>miu[0][j];

        }
        std::cout<<"\n\n";
    }

    //image input
    else
    {
        for(int i=0; i<num_of_pattern_set; i++)
        {
            read_binary_file_for_hopfield::fetch_ins(INS[i],i,100);
        }
        read_binary_file_for_hopfield::fetch_miu(miu,100);
    }


//WEIGHT MATRIX INITIALIZATION
    float **WEIGHT;
    float **prev_WEIGHT;
    WEIGHT=new float*[num_of_nodes];
    prev_WEIGHT=new float*[num_of_nodes];
    for(int i=0; i<num_of_nodes; i++)
    {
        WEIGHT[i]=new float[num_of_nodes];
        prev_WEIGHT[i]=new float[num_of_nodes];
    }

//initialize both weight and prev_weight to 0s
    for(int i=0; i<num_of_nodes; i++)
        for(int j=0; j<num_of_nodes; j++)
        {
            WEIGHT[i][j]=0;
            prev_WEIGHT[i][j]=0;
        }

    for(int i=0; i<num_of_pattern_set; i++) //calculate weights
    {
//make a transpose of the INS
        float **T_INS;
        T_INS=new float*[num_of_nodes];
        for(int j=0; j<num_of_nodes; j++ )
        {
            T_INS[j]=new float[1];
        }

//T[INS]
        MATRIX_OPS::MATRIX_Transpose(INS[i],1,num_of_nodes,T_INS);
//W=INS_t * INS
        if(c=='M')//if choice was manual input(cumbersome)
        print_matrix(T_INS,num_of_nodes,1,"inverse X");
//W1=T[INS]*INS
        MATRIX_OPS::MATRIX_MUL(T_INS,num_of_nodes,1,INS[i],1,num_of_nodes,prev_WEIGHT);
        if(c=='M')//if choice was manual input(cumbersome)
        print_matrix(prev_WEIGHT,num_of_nodes,num_of_nodes,"WEIGHT");
//Wn=Wn-1+Wn-2+.....+W0
        MATRIX_OPS::MATRIX_ADD_SAME(WEIGHT,num_of_nodes,num_of_nodes,prev_WEIGHT,num_of_nodes,num_of_nodes,WEIGHT);
    }
    if(c=='M')//if choice was manual input(cumbersome)
    print_matrix(WEIGHT,num_of_nodes,num_of_nodes,"Total WEIGHT");


//calculate new miu
    float **new_miu;
    new_miu=new float*[1];
    new_miu[0]=new float[num_of_nodes];
    int num_of_update=0;

    while(true)
    {
        num_of_update++;

    //miu(t+1)=miu(t)*Wn
        MATRIX_OPS::MATRIX_MUL(miu,1,num_of_nodes,WEIGHT,num_of_nodes,num_of_nodes,new_miu);
        if(c=='M')//if choice was manual input(cumbersome)
        print_matrix(new_miu,1,num_of_nodes,"miu(t+1)");

    // hard threshold Fh(miu(t))
        MATRIX_OPS::MATRIX_HARD_THRESHOLD_TRANSFORM(new_miu,1,num_of_nodes,0);
        if(c=='M')//if choice was manual input(cumbersome)
        print_matrix(new_miu,1,num_of_nodes,"threshold miu(t+1)");
        if(c!='M')
        draw_miu(new_miu,num_of_nodes);
    //check if new and prev miu are same or not
        MATRIX_OPS::MATRIX_SUB_SAME(new_miu,1,num_of_nodes,miu,1,num_of_nodes,miu);
        //if(c=='M')//if choice was manual input(cumbersome)
        print_matrix(miu,1,num_of_nodes," miu(t+1)- miu(t)");
        bool check=false;
        for(int k=0; k<num_of_nodes; k++)
        {
            if(miu[0][k]!=0)
            {
             check=true;
             break;
            }
        }
        if(check==false)
        {
            break;
        }
        else
        {
            for(int k=0; k<num_of_nodes; k++)
            {
                miu[0][k]=new_miu[0][k];
            }
            if(c=='M')//if choice was manual input(cumbersome)
            print_matrix(miu,1,num_of_nodes,"miu(t+1)");
        }

    }
    getch();
    closegraph();
    return 0;
}


void print_matrix(float **A,int r,int c,const char* name)
{
    std::cout<<name<<" : \n";
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            std::cout<<A[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

void draw_miu(float**miu,int num_of_nodes)
{
    for(int itr=0; itr<num_of_nodes; itr++)
    {
        int x=(itr)%10;
        int y=(int)(itr/10);
        for(int m=x*10; m<x*10+10; m++)
            for(int n=y*10; n<y*10+10; n++)
            {
                if(miu[0][itr]==1)
                    putpixel(m,n+350,YELLOW);
            }

    }
    swapbuffers();
}
