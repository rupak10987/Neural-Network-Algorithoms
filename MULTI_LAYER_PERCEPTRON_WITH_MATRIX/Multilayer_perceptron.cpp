#include<iostream>
#include<time.h>
#include"MATRIX_SCRATCH.h"
#include<math.h>
int main()
{
    srand(time(0));
    int num_of_layers;
    int num_of_nodes_per_layer;
    std::cout<<"NUMBER OF ACTIVE LAYERS :";
    std::cin>>num_of_layers;
    std::cout<<"NUMBER OF NODES PER LAYER :";
    std::cin>>num_of_nodes_per_layer;
    //initializing in/out and weight matrices
    float ***INOUTS;
    float ***WEIGHTS;
    float expected[num_of_nodes_per_layer];
    INOUTS=new float**[num_of_layers+1];
    WEIGHTS=new float**[num_of_layers];
    for(int i=0; i<num_of_layers+1; i++)
    {
        INOUTS[i]=new float*[1];//a row vector like [1,1,1,1]
        INOUTS[i][0]=new float[num_of_nodes_per_layer];
    }

    for(int i=0; i<num_of_layers; i++)
    {
        WEIGHTS[i]=new float*[num_of_nodes_per_layer];
        for(int j=0; j<num_of_nodes_per_layer; j++)
        {
            WEIGHTS[i][j]=new float[num_of_nodes_per_layer];
        }
    }
    //network i/o
    for(int i=0; i<num_of_layers+1; i++)
    {
        for(int j=0; j<num_of_nodes_per_layer; j++)
        {
            if(i==0)
            {
                std::cout<<"INPUT "<<j<<" :";
                std::cin>>INOUTS[i][0][j];
            }
            else
                INOUTS[i][0][j]=0;
        }
    }
    for(int i=0; i<num_of_nodes_per_layer; i++)
    {
        std::cout<<"EXPECTED OUTPUT"<<i<<" : ";
        std::cin>>expected[i];
    }
//network weights
    char choice;
    std::cout<<"RANDOM WEIGHTS(R) / MANUALLY PUT WEIGHTS (M) :";
    std::cin>>choice;
    for(int i=0; i<num_of_layers; i++)
        for(int j=0; j<num_of_nodes_per_layer; j++)
            for(int k=0; k<num_of_nodes_per_layer; k++)
            {
                if(choice=='R')
                {
                    WEIGHTS[i][k][j]=(float)((rand()%100)+1)/100.00;
                    std::cout<<"weight of "<<i<<k<<"--"<<i+1<<j<<" : "<<WEIGHTS[i][k][j]<<std::endl;
                }
                else
                {
                    std::cout<<"weight of "<<i<<k<<"--"<<i+1<<j<<" : ";
                    std::cin>>WEIGHTS[i][k][j];
                }
            }
//visualizing the weight matrices;
    std::cout<<"\n\n";
    for(int i=0; i<num_of_layers; i++)
    {
        std::cout<<"W_MATRIC L"<<i<<"-L"<<i+1<<std::endl;
        for(int j=0; j<num_of_nodes_per_layer; j++)
        {
            for(int k=0; k<num_of_nodes_per_layer; k++)
            {
                std::cout<< WEIGHTS[i][j][k]<<"\t";
            }
            std::cout<<"\n";
        }
        std::cout<<"\n";
    }

    float ***sigmas;
    sigmas=new float**[num_of_layers];
    for(int i=0; i<num_of_layers; i++)
    {
        sigmas[i]=new float*[num_of_nodes_per_layer];//[collumn vector]
        for(int j=0; j<num_of_nodes_per_layer; j++)
        {
            sigmas[i][j]=new float[1];
            sigmas[i][j][0]=0;
        }
    }
//learn
    int MAX_ITER=1000;
    while(MAX_ITER>=0)
    {

        float k=5;
        float **delw;
        delw=new float*[num_of_nodes_per_layer];
        for(int j=0; j<num_of_nodes_per_layer; j++)
            delw[j]=new float[num_of_nodes_per_layer];

//forwardpass
        for(int i=0; i<num_of_layers; i++)
        {
//referecncing the function for teacher's favour:=
//void MATRIX_MUL(float** MAT_A,int num_of_row_A,int num_of_col_A,float** MAT_B,int num_of_row_B,int num_of_col_B,float** result)
            MATRIX_OPS::MATRIX_MUL(INOUTS[i],1,num_of_nodes_per_layer,WEIGHTS[i],num_of_nodes_per_layer,num_of_nodes_per_layer,INOUTS[i+1]);
            MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[i+1],1,num_of_nodes_per_layer,k);
        }
        float ep=0;
        for(int i=0; i<num_of_nodes_per_layer; i++)
        {
            ep+=pow(expected[i]-INOUTS[num_of_layers][0][i],2);
        }
        ep/=2;
        std::cout<<"Ep = "<<ep<<"\n\n\n\n";
        if(ep<=0.1)
            break;
//backward_propagation//adjusting weights
        for(int i=num_of_layers-1; i>=0; i--)
        {
            float miu=0.2;

            //STEP1 :calculate sigma for out layer and hidden layers
            if(i==num_of_layers-1)//sigma outlayer
            {
                for(int j=0; j<num_of_nodes_per_layer; j++)
                    sigmas[i][j][0]=k*INOUTS[i+1][0][j]*(1-INOUTS[i+1][0][j])*(expected[j]-INOUTS[i+1][0][j]);
            }
            else//sigma hidden layers
            {
                MATRIX_OPS::MATRIX_MUL(WEIGHTS[i+1],num_of_nodes_per_layer,num_of_nodes_per_layer,sigmas[i+1],num_of_nodes_per_layer,1,sigmas[i]);//sum of sigK*Wjk
                for(int j=0; j<num_of_nodes_per_layer; j++)
                    sigmas[i][j][0]=k*INOUTS[i+1][0][j]*(1-INOUTS[i+1][0][j])*sigmas[i][j][0];
            }
            //STEP2: calculate transpose of the weight matric that is to be updated
            MATRIX_OPS::MATRIX_Transpose(WEIGHTS[i],num_of_nodes_per_layer,num_of_nodes_per_layer);
            //STEP3: find delW=miu*sig*op
            MATRIX_OPS::MATRIX_MUL(sigmas[i],num_of_nodes_per_layer,1,INOUTS[i+1],1,num_of_nodes_per_layer,delw);
            MATRIX_OPS::MATRIX_Constant_Multiply(delw,num_of_nodes_per_layer,num_of_nodes_per_layer,miu);
            //STEP4:weights(t+1)=weights(t)+delw
            MATRIX_OPS::MATRIX_ADD(WEIGHTS[i],num_of_nodes_per_layer,num_of_nodes_per_layer,delw,num_of_nodes_per_layer,num_of_nodes_per_layer,WEIGHTS[i]);
            //STEP5: retranspose the weight matric
            MATRIX_OPS::MATRIX_Transpose(WEIGHTS[i],num_of_nodes_per_layer,num_of_nodes_per_layer);

            //visualize updated weigts to console
            std::cout<<"updated weight matric of L"<<i<<"-L"<<i+1<<" :\n";
            for(int j=0; j<num_of_nodes_per_layer; j++)
            {
                for(int l=0; l<num_of_nodes_per_layer; l++)
                {
                    std::cout<<WEIGHTS[i][j][l]<<" ";
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;
        }
        MAX_ITER--;
    }
//output
    std::cout<<"\n total iterations "<<1001-MAX_ITER<<"\n";
    for(int i=0; i<num_of_nodes_per_layer; i++)
    {
        std::cout<<"out"<<i<<" : "<<INOUTS[num_of_nodes_per_layer][0][i]<<"\n";
    }
    return 0;
}