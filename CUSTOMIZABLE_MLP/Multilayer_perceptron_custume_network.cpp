#include<iostream>
#include<time.h>
#include"MATRIX_SCRATCH.h"
#include<math.h>
int main()
{
    srand(time(0));
    int num_of_layers;
    int *num_of_nodes_per_layer;//each layer will have diff num of nodes but fully connected with nodes of other neighboring layers
    std::cout<<"NUMBER OF ACTIVE LAYERS :";
    std::cin>>num_of_layers;
    num_of_nodes_per_layer=new int[num_of_layers+1];
    for(int i=0; i<num_of_layers+1; i++)
    {
        std::cout<<"NUMBER OF NODES IN LAYER"<<i<<" :";
        std::cin>>num_of_nodes_per_layer[i];
    }
    //initializing in/out and weight matrices
    float ***INOUTS;
    float ***WEIGHTS;
    float expected[num_of_nodes_per_layer[num_of_layers]];
    INOUTS=new float**[num_of_layers+1];
    WEIGHTS=new float**[num_of_layers];
    for(int i=0; i<num_of_layers+1; i++)
    {
        INOUTS[i]=new float*[1];//a row vector like [1,1,1,1]
        INOUTS[i][0]=new float[num_of_nodes_per_layer[i]];
    }

    for(int i=0; i<num_of_layers; i++)
    {
        WEIGHTS[i]=new float*[num_of_nodes_per_layer[i]];//row=num of curr layer node
        for(int j=0; j<num_of_nodes_per_layer[i]; j++)
        {
            WEIGHTS[i][j]=new float[num_of_nodes_per_layer[i+1]];//collumn=next layer nodes
        }
    }
    //network i/o
    for(int i=0; i<num_of_layers+1; i++)
    {
        for(int j=0; j<num_of_nodes_per_layer[i]; j++)
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
    for(int i=0; i<num_of_nodes_per_layer[num_of_layers]; i++)
    {
        std::cout<<"EXPECTED OUTPUT"<<i<<" : ";
        std::cin>>expected[i];
    }
//network weights
    char choice;
    std::cout<<"RANDOM WEIGHTS(R) / MANUALLY PUT WEIGHTS (M) :";
    std::cin>>choice;
    for(int i=0; i<num_of_layers; i++)
        for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
            for(int k=0; k<num_of_nodes_per_layer[i]; k++)
            {
                if(choice=='R')
                {
                    WEIGHTS[i][k][j]=(float)((rand()%100)+1)/100.00;//fill vertically
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
        for(int j=0; j<num_of_nodes_per_layer[i]; j++)
        {
            for(int k=0; k<num_of_nodes_per_layer[i+1]; k++)
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
        sigmas[i]=new float*[num_of_nodes_per_layer[i+1]];//[collumn vector]
        for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
        {
            sigmas[i][j]=new float[1];
            sigmas[i][j][0]=0;
        }
    }

//learn
    int MAX_ITER=5000;
    while(MAX_ITER>=0)
    {
        float k=5;
//forwardpass
        for(int i=0; i<num_of_layers; i++)
        {
//referecncing the function for teacher's favour:=
//void MATRIX_MUL(float** MAT_A,int num_of_row_A,int num_of_col_A,float** MAT_B,int num_of_row_B,int num_of_col_B,float** result)
            MATRIX_OPS::MATRIX_MUL(INOUTS[i],1,num_of_nodes_per_layer[i],WEIGHTS[i],num_of_nodes_per_layer[i],num_of_nodes_per_layer[i+1],INOUTS[i+1]);
            MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[i+1],1,num_of_nodes_per_layer[i+1],k);
        }
        float ep=0;
        for(int i=0; i<num_of_nodes_per_layer[num_of_layers]; i++)
        {
            ep+=pow(expected[i]-INOUTS[num_of_layers][0][i],2);
        }
        ep/=2;
        //output show
        for(int ou=0; ou<num_of_nodes_per_layer[num_of_layers]; ou++)
        {
            std::cout<<"out"<<ou<<" : "<<INOUTS[num_of_layers][0][ou]<<"\n";
        }
        std::cout<<"Ep = "<<ep<<"\n\n\n\n";
        if(ep<=0.001)
            break;

//backward_propagation//adjusting weights
        for(int i=num_of_layers-1; i>=0; i--)
        {
            float miu=0.2;

            //STEP1 :calculate sigma for out layer and hidden layers
            if(i==num_of_layers-1)//sigma calculation outlayer
            {

                for(int j=0; j<num_of_nodes_per_layer[num_of_layers]; j++)
                {
                    sigmas[i][j][0]=k*INOUTS[i+1][0][j]*(1-INOUTS[i+1][0][j])*(expected[j]-INOUTS[i+1][0][j]);
                }

            }
            else//sigma calculation hidden layers
            {

                //these segment is for retrieving prev layers weights before update
               // MATRIX_OPS::MATRIX_Transpose(delw,num_of_nodes_per_layer,num_of_nodes_per_layer);
                //MATRIX_OPS::MATRIX_SUB(WEIGHTS[i+1],num_of_nodes_per_layer,num_of_nodes_per_layer,delw,num_of_nodes_per_layer,num_of_nodes_per_layer,delw);
                //retrieved weight is transfered to delw. it's same as prev weights[i+1] dont get confused seeing delw below.

                MATRIX_OPS::MATRIX_MUL(WEIGHTS[i+1],num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i+2],sigmas[i+1],num_of_nodes_per_layer[i+2],1,sigmas[i]);//sum of sigK*Wjk
                 std::cout<<"\nhello\n";
                for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
                    sigmas[i][j][0]=k*INOUTS[i+1][0][j]*(1-INOUTS[i+1][0][j])*sigmas[i][j][0];
            }

            //STEP2: calculate transpose of the weight matric that is to be updated
            float **TRANSPOSED;
            TRANSPOSED=new float*[num_of_nodes_per_layer[i+1]];
            for(int r=0;r<num_of_nodes_per_layer[i+1];r++)
                TRANSPOSED[r]=new float[num_of_nodes_per_layer[i]];
            MATRIX_OPS::MATRIX_Transpose(WEIGHTS[i],num_of_nodes_per_layer[i],num_of_nodes_per_layer[i+1],TRANSPOSED);

            //STEP3: find delW=miu*sig*op
            float **delw;//same dimension as transposed
            delw=new float*[num_of_nodes_per_layer[i+1]];
            for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
                delw[j]=new float[num_of_nodes_per_layer[i]];
            MATRIX_OPS::MATRIX_MUL(sigmas[i],num_of_nodes_per_layer[i+1],1,INOUTS[i+1],1,num_of_nodes_per_layer[i+1],delw);

            MATRIX_OPS::MATRIX_Constant_Multiply(delw,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i],miu);

            //STEP4:weights(t+1)=T(weights(t))+delw
            MATRIX_OPS::MATRIX_ADD(TRANSPOSED,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i],delw,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i],TRANSPOSED);

            //STEP5: retranspose the weight matric
            MATRIX_OPS::MATRIX_Transpose(TRANSPOSED,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i],WEIGHTS[i]);

            //visualize updated weigts to console
            std::cout<<"updated weight matric of L"<<i<<"-L"<<i+1<<" :\n";
            for(int j=0; j<num_of_nodes_per_layer[i]; j++)
            {
                for(int l=0; l<num_of_nodes_per_layer[i+1]; l++)
                {
                    std::cout<<WEIGHTS[i][j][l]<<" ";
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;
        }
        MAX_ITER--;
    }
    std::cout<<"\n total iterations "<<5000-MAX_ITER<<"\n";
    return 0;
}
