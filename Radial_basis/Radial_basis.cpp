#include<iostream>
#include<time.h>
#include"MATRIX_SCRATCH.h"
int main()
{
    srand(time(0));
    int num_of_features;
    std::cout<<"enter num of features :";
    std::cin>>num_of_features;
    int num_of_centroids;
    std::cout<<"enter num of centroids: ";
    std::cin>>num_of_centroids;
    float **c;
    c=new float*[num_of_centroids];
    for(int i=0;i<num_of_centroids;i++)
    {
        c[i]=new float[num_of_features];
    }
//input centers
for(int i=0;i<num_of_centroids;i++)
{
    for(int j=0;j<num_of_features;j++)
    {
        std::cout<<"enter y"<<i<<j<<" : ";
        std::cin>>c[i][j];
    }
}
//initialize the network
float ***INOUTS;
INOUTS=new float**[3];
INOUTS[1]=new float*[1];//row vector
INOUTS[1][0]=new float[num_of_centroids];
INOUTS[2]=new float*[1];
INOUTS[2][0]=new float[2];//only two output nodes;
//input pattern
INOUTS[0]=new float*[1];
INOUTS[0][0]=new float[num_of_features];
for(int i=0;i<num_of_features;i++)
{
    std::cout<<"IN "<<i<<" :";
    std::cin>>INOUTS[0][0][i];
}

float ***WEIGHTS;
WEIGHTS=new float**[2];
//input to hidden
WEIGHTS[0]=new float*[num_of_features];
std::cout<<"WEIGHT MATRIX OF INPUT-HIDDEN\n";
for(int i=0;i<num_of_features;i++)
{
    WEIGHTS[0][i]=new float[num_of_centroids];
    for(int j=0;j<num_of_centroids;j++)
    {
        WEIGHTS[0][i][j]=((rand()%100)+1.0)/10.0;
        std::cout<<WEIGHTS[0][i][j]<<"\t";
    }
    std::cout<<"\n";
}
//hidden to output
std::cout<<"WEIGHT MATRIX OF HIDDEN-OUTPUT\n";
WEIGHTS[1]=new float*[num_of_centroids];
for(int i=0;i<num_of_centroids;i++)
{
    WEIGHTS[1][i]=new float[2];
    for(int j=0;j<2;j++)
    {
        WEIGHTS[1][i][j]=((rand()%100)+1.0)/10.0;
        std::cout<<WEIGHTS[1][i][j]<<"\t";
    }
    std::cout<<"\n";
}
float **sigma;
sigma=new float*[2];
sigma[0]=new float[1];
sigma[1]=new float[1];
float expected[2];
expected[0]=1;
expected[1]=0;
//set f(hidden layer outs)
MATRIX_OPS::CALC_DISTANCE(INOUTS[0],1,num_of_features,WEIGHTS[0],num_of_features,num_of_centroids,INOUTS[1]);
MATRIX_OPS::MATRIX_GAUSSIAN_TRANSFORM(INOUTS[1],1,num_of_centroids,1);
while(true)
{
  //hidden to out
  float k=10;
  float miu=0.2;
MATRIX_OPS::MATRIX_MUL(INOUTS[1],1,num_of_centroids,WEIGHTS[0],num_of_centroids,2,INOUTS[2]);
MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[2],1,2,k);

//calculate ep
float ep=pow(INOUTS[2][0][0]-expected[0],2)+pow(INOUTS[2][0][1]-expected[1],2);
ep/=2;
if(ep<=0.01)
    break;
//backbass
float** T_saved_delw;
//calculate sigma
for(int j=0; j<2; j++)
    {
        sigma[j][0]=k*INOUTS[2][0][j]*(1-INOUTS[2][0][j])*(expected[j]-INOUTS[2][0][j]);
    }
//claculate transpose of weight matric
float **TRANSPOSED;
TRANSPOSED=new float*[2];
for(int r=0; r<2; r++)
    TRANSPOSED[r]=new float[num_of_centroids];
MATRIX_OPS::MATRIX_Transpose(WEIGHTS[1],num_of_centroids,2,TRANSPOSED);

//STEP3: find delW=miu*sig*op
            float **delw;//same dimension as transposed
            delw=new float*[2];
            T_saved_delw=new float*[num_of_centroids];
            for(int j=0; j<2; j++)
            {
                delw[j]=new float[num_of_centroids];
            }
            for(int j=0; j<num_of_centroids; j++)
            {
                T_saved_delw[j]= new float[2];
            }
            MATRIX_OPS::MATRIX_MUL(sigma,2,1
                                   ,INOUTS[1],1,num_of_centroids,delw);////////////
            MATRIX_OPS::MATRIX_Transpose(delw,2
                                         ,num_of_centroids
                                         ,T_saved_delw);//saving delw as transposed
            MATRIX_OPS::MATRIX_Constant_Multiply(delw,2
                                                 ,num_of_centroids,miu);

//step4
 MATRIX_OPS::MATRIX_ADD(TRANSPOSED,2,num_of_centroids
                                   ,delw,2,num_of_centroids,TRANSPOSED);

            //STEP5: retranspose the weight matric
            MATRIX_OPS::MATRIX_Transpose(TRANSPOSED,2
                                         ,num_of_centroids,WEIGHTS[1]);

//visualize updated weights
std::cout<<"updated weight\n";
for(int i=0;i<num_of_centroids;i++)
{
  for(int j=0;j<2;j++)
{
    std::cout<<WEIGHTS[1][i][j]<<"\t";
}
std::cout<<"\n";
}
}

}
