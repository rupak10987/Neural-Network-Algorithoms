#include<iostream>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
struct POINTSS
{
    float x1;
    float x2;
};
float euclid_dist(float x1,float x2,float y1,float y2);
void MATRIX_DISTANCE_CALC(float** MAT_A
                          ,int num_of_row_A
                          ,int num_of_col_A
                          ,float** MAT_B
                          ,int num_of_row_B
                          ,int num_of_col_B
                          ,float** result);
int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(1000, 500);
    srand(time(0));


    int num_of_layers=2;
    int num_of_nodes_per_layer[2]= {2,10}; //input node 2 and network nodes 10
    //data set
    POINTSS point[100];
    for(int i=0; i<100; i++)
    {
        int choice=rand()%2;//2 clusters for now
        switch(choice)
        {
        case 0:
        {
            point[i].x1=10+rand()%10;
            point[i].x2=10+rand()%10;
            putpixel(point[i].x1,point[i].x2,YELLOW);
            break;
        }

        case 1:
        {
            point[i].x1=25+rand()%10;
            point[i].x2=25+rand()%10;
            putpixel(point[i].x1,point[i].x2,GREEN);
            break;
        }
        }
    }
    //input matrix initialization
    float **INS;
    INS=new float*[1];//a row vector like [1,1,1,1]
    INS[0]=new float[num_of_nodes_per_layer[0]];
    //weight matric initialization
    float **WEIGHTS;
    float **temp_weights;
    WEIGHTS=new float*[num_of_nodes_per_layer[0]];//2 row
    temp_weights=new float*[num_of_nodes_per_layer[0]];//2 row
    WEIGHTS[0]=new float[num_of_nodes_per_layer[1]];//10 collumns
    WEIGHTS[1]=new float[num_of_nodes_per_layer[1]];//10 collumns
    temp_weights[0]=new float[num_of_nodes_per_layer[1]];//10 collumns
    temp_weights[1]=new float[num_of_nodes_per_layer[1]];//10 collumns


    //assign random weights
    for(int j=0; j<num_of_nodes_per_layer[1]; j++)
    {
       for(int k=0; k<num_of_nodes_per_layer[0]; k++)
        {
            WEIGHTS[k][j]=5+rand()%50;//fill vertically
        }
        //putpixel(WEIGHTS[0][j],WEIGHTS[1][j],RED);
    }

    //main algo
    float neighbor_radius=30;
    float miu=0.5;
    for(int i=0;i<100;i++)//iterate for every pattern
    {
    INS[0][0]=point[i].x1;
    INS[0][1]=point[i].x2;
    //calculate distance
    float **store_result;
    store_result=new float*[0];
    store_result[0]=new float[num_of_nodes_per_layer[1]];
    MATRIX_DISTANCE_CALC(INS
                        ,1
                        ,num_of_nodes_per_layer[0]
                        ,WEIGHTS,num_of_nodes_per_layer[0]
                        ,num_of_nodes_per_layer[1]
                        ,store_result);
    //find the winner node based on the least distance
    float min_dist=100000;
    int min_d_index=0;
    for(int j=0;j<num_of_nodes_per_layer[1];j++)
    {
        if(min_dist>store_result[0][j])
        {
            min_dist=store_result[0][j];
            min_d_index=j;
        }
    }

    //update weights of the winner node and nodes that are in neighborhood
    cleardevice();
    for(int j=0; j<num_of_nodes_per_layer[1]; j++)
    {
       for(int k=0; k<num_of_nodes_per_layer[0]; k++)
        {
            if(euclid_dist(WEIGHTS[0][min_d_index],WEIGHTS[1][min_d_index],WEIGHTS[0][j],WEIGHTS[1][j])<=neighbor_radius)
            WEIGHTS[k][j]= WEIGHTS[k][j]+miu*(INS[0][k]-WEIGHTS[k][j]);
        }


        putpixel(WEIGHTS[0][j],WEIGHTS[1][j],RED);

    }swapbuffers();delay(100);
    neighbor_radius-=0.8;
    miu-=0.001;
    }

    getch();
    closegraph();

}

float euclid_dist(float x1,float x2,float y1,float y2)
{
    return pow(pow(x1-y1,2)+pow(x2-y2,2),2);
}

void MATRIX_DISTANCE_CALC(float** MAT_A
                          ,int num_of_row_A
                          ,int num_of_col_A
                          ,float** MAT_B
                          ,int num_of_row_B
                          ,int num_of_col_B
                          ,float** result)
{
    if(num_of_col_A!=num_of_row_B)
    {
        return;
    }

    //initialize result to 0;
    for(int i=0; i<num_of_row_A; i++)
    {
        for(int j=0; j<num_of_col_B; j++)
        {
            result[i][j]=0;
        }
    }
    //multiplication
    for(int i = 0; i < num_of_row_A; ++i)
        for(int j = 0; j < num_of_col_B; ++j)
        {
            for(int k = 0; k < num_of_col_A; ++k)
            {
                result[i][j] += pow(MAT_A[i][k] - MAT_B[k][j],2);
            }
            pow(result[i][j],0.5);
        }
}
