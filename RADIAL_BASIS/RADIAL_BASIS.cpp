#include<iostream>
#include<graphics.h>
#include<time.h>
#include"MATRIX_SCRATCH.h"
#include"GRAPHICAL_VIEW_NEURO.h"
int main()
{
    srand(time(0));
//initializations
    int num_of_features;
    int num_of_cetroids;
    int num_of_class;
    std::cout<<"ENTER NUM OF FEATURES: ";
    std::cin>>num_of_features;
    std::cout<<"ENTER NUM OF CENTROIDS: ";
    std::cin>>num_of_cetroids;
    std::cout<<"ENTER NUM OF CLASSES: ";
    std::cin>>num_of_class;
    float ***INOUTS;
    INOUTS=new float**[3];//3 layers
//row vectors like [i1,i2,i3]
    INOUTS[0]=new float*[1];
    INOUTS[1]=new float*[1];
    INOUTS[2]=new float*[1];
    INOUTS[0][0]=new float[num_of_features];//input layer
    INOUTS[1][0]=new float[num_of_cetroids];//hidden layer
    INOUTS[2][0]=new float[num_of_class];//out layer
    for(int i=0; i<num_of_features; i++)
    {
        std::cout<<"Input"<<i<<" :";
        std::cin>>INOUTS[0][0][i];
    }
    float expected[num_of_class];
    for(int i=0; i<num_of_class; i++)
    {
        std::cout<<"EXPECTED"<<i<<" :";
        std::cin>>expected[i];
    }
    float ***WEIGHTS;
    WEIGHTS=new float**[2];//two sets of weights
    WEIGHTS[0]=new float*[num_of_features];
    std::cout<<"WEIGHT MATRIX OF IN-->HIDDEN\n";
    for(int i=0; i<num_of_features; i++)
    {
        WEIGHTS[0][i]=new float[num_of_cetroids];
    }
    for(int i=0; i<num_of_cetroids; i++)
    {
        for(int j=0; j<num_of_features; j++)
        {
            std::cout<<"CENTROID"<<i<<"-"<<j<<" :";
            std::cin>>WEIGHTS[0][j][i];
        }
    }
    WEIGHTS[1]=new float*[num_of_cetroids];
    std::cout<<"WEIGHT MATRIX OF HIDDEN-->OUT\n";
    for(int i=0; i<num_of_cetroids; i++)
    {
        WEIGHTS[0][i]=new float[num_of_class];
        for(int j=0; j<num_of_class; j++)
        {
            WEIGHTS[0][i][j]=(1+rand()%100)/100.0;
            std::cout<<WEIGHTS[0][i][j]<<"\t";
        }
        std::cout<<"\n";
    }

//forward pass IN->HIDDEN
    MATRIX_OPS::DISTANCE_SQUARED_CALCULATION(INOUTS[0],1,num_of_features,WEIGHTS[0],num_of_features,num_of_cetroids,INOUTS[1]);
    MATRIX_OPS::MATRIX_GAUSIAN_TRANSFORM(INOUTS[1],1,num_of_cetroids);
    float **sigmas;
    sigmas=new float*[num_of_class];//collumn vec
    for(int i=0; i<num_of_class; i++)
    {
        sigmas[i]=new float[1];
    }
//learn
    int k=5;
    int miu=0.9;
    int iteration=0;
    while(true)
    {
        std::cout<<"ITERATION :"<<iteration<<"\n";
//forward pass for hidden->out
        MATRIX_OPS::MATRIX_MUL(INOUTS[1],1,num_of_cetroids,WEIGHTS[1],num_of_cetroids,num_of_class,INOUTS[2]);
    std::cout<<"hello\n";
        MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[2],1,num_of_class,k);
        float Ep=0;
        for(int i=0; i<num_of_class; i++)
        {
            std::cout<<"OUT"<<i<<" :"<<INOUTS[2][0][i]<<"\n";
            Ep+=pow(INOUTS[2][0][i]-expected[i],2);
        }
        Ep/=2;
        std::cout<<"Ep="<<Ep<<"\n";
        if(Ep<0.01)
            break;
//back propagate out-->hidden
//step 1: calculate sigma
        for(int i=0; i<num_of_class; i++)
        {
            sigmas[i][0]=k*INOUTS[2][0][i]*(1-INOUTS[2][0][i])*(expected[i]-INOUTS[2][0][i]);
        }

//STEP2: calculate transpose of the weight matric that is to be updated
        float **TRANSPOSED;
        TRANSPOSED=new float*[num_of_class];
        for(int r=0; r<num_of_class; r++)
            TRANSPOSED[r]=new float[num_of_cetroids];
        MATRIX_OPS::MATRIX_Transpose(WEIGHTS[1],num_of_cetroids,num_of_class,TRANSPOSED);

//STEP3: find delW = miu*sigma*opi
        float **delw;//same dimension as transposed
        delw=new float*[num_of_class];
        for(int j=0; j<num_of_class; j++)
        {
            delw[j]=new float[num_of_cetroids];
        }
        MATRIX_OPS::MATRIX_MUL(sigmas,num_of_class,1,INOUTS[1],1,num_of_cetroids,delw);
        MATRIX_OPS::MATRIX_Constant_Multiply(delw,num_of_class,num_of_cetroids,miu);

//STEP4:weights(t+1)=T(weights(t))+delw
        MATRIX_OPS::MATRIX_ADD(TRANSPOSED,num_of_class,num_of_cetroids,delw,num_of_class,num_of_cetroids,TRANSPOSED);

//STEP5: retranspose the weight matric
        MATRIX_OPS::MATRIX_Transpose(TRANSPOSED,num_of_class,num_of_cetroids,WEIGHTS[1]);
//visualize updated weight ofhidden-->out
std::cout<<"ÜPDATED WEIGHTS of HIDDEN-->OUT\n";
        for(int i=0; i<num_of_cetroids; i++)
        {
            for(int j=0; j<num_of_class; j++)
            {
                std::cout<<WEIGHTS[1][i][j]<<"\t";
            }std::cout<<std::endl;
        }
        iteration++;
        if(iteration>=5000)
            break;
    }
return 0;
}
