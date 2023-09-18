#include<iostream>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include"MATRIX_SCRATCH.h"
#include"GRAPHICAL_VIEW_NEURO.h"
#include"chromose_op.h"
int main()
{
    srand(time(0));
    int gd = DETECT, gm;
    initwindow(1000, 700);
    int num_of_layers;
    //each layer will have diff num of nodes but fully connected with nodes of other neighboring layers
    int *num_of_nodes_per_layer;
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

//first determine what is the chromosome length;
    int population_size;
    std::cout<<"Enter Population size: ";
    std::cin>>population_size;
    int gene_length=5;
    int chromosome_length=0;
    for(int i=0; i<num_of_layers; i++)
    {
        chromosome_length+=num_of_nodes_per_layer[i]*num_of_nodes_per_layer[i+1];
    }
    chromosome_length*=gene_length;
    std::cout<<"\nchromosome Length = "<<chromosome_length<<"\n";

//generating random chromosomes
    char** chromosomes;
    chromosomes=new char*[population_size];
    for(int i=0; i<population_size; i++)
    {
        chromosomes[i]=new char[chromosome_length+1];//+1 for '\0', which indicates end of a chromose
        GENETIC::gen_random_chromosome(chromosomes[i],chromosome_length);
        //visualizing each chromosomes
        //NEURAL_GRAPHIC::view_chromosome(chromosomes,population_size,chromosome_length,gene_length,20,300);
    }

    float **simplified_weight;
    simplified_weight=new float*[population_size];
    for(int i=0; i<population_size; i++)
    {
        simplified_weight[i]=new float[chromosome_length/gene_length];
    }

    float *Fitness;
    Fitness=new float[population_size];
//learning
    int MAX_GENERATION=500;
    while(MAX_GENERATION>=0)
    {
        //extract weights form all the chromosomes
        cleardevice();
        float k=2;
        for(int c=0; c<population_size; c++) //for each chromosome
        {
            cleardevice();
            char gn[100];
            sprintf(gn,"GENERATION: %d",500-MAX_GENERATION);
            outtextxy(450,20,gn);
            GENETIC::weight_extraction(chromosomes[c],simplified_weight[c],chromosome_length,gene_length);//collecting weights in simple format
            int w_c=0;
            for(int L=0; L<num_of_layers; L++)
            {
                for(int col=0; col<num_of_nodes_per_layer[L+1]; col++)
                {
                    for(int row=0; row<num_of_nodes_per_layer[L]; row++)
                    {
                        WEIGHTS[L][row][col]=simplified_weight[c][w_c];
                        w_c++;
                    }
                }
            }
            NEURAL_GRAPHIC::view_chromosome(chromosomes,population_size,chromosome_length,gene_length,180,500);
            //after extraction,visualizing the weights in console;
//forward pass
            for(int i=0; i<num_of_layers; i++)
            {
                MATRIX_OPS::MATRIX_MUL(INOUTS[i],1
                                       ,num_of_nodes_per_layer[i]
                                       ,WEIGHTS[i],num_of_nodes_per_layer[i]
                                       ,num_of_nodes_per_layer[i+1],INOUTS[i+1]);
                MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[i+1],1,num_of_nodes_per_layer[i+1],k);

                //visualize to window
                NEURAL_GRAPHIC::view_network(i,WEIGHTS[i]
                                             ,num_of_nodes_per_layer[i]
                                             ,num_of_nodes_per_layer[i+1]);
                NEURAL_GRAPHIC::view_node(INOUTS[i],i,num_of_nodes_per_layer[i]);


            }
            NEURAL_GRAPHIC::view_node(INOUTS[num_of_layers],num_of_layers,num_of_nodes_per_layer[num_of_layers]);
            for(int ex=0; ex<num_of_nodes_per_layer[num_of_layers]; ex++)
            {
                char ch[100];
                sprintf(ch,"EXPECTED OUTPUT %0.3f",expected[ex]);
                outtextxy(800,20*(ex+1),ch);
            }
            //calculate errors
            float ep=0;
            for(int i=0; i<num_of_nodes_per_layer[num_of_layers]; i++)
            {
                ep+=pow(expected[i]-INOUTS[num_of_layers][0][i],2);
            }
            ep/=2;
            Fitness[c]=1.0/ep;
            char ch[100];
            sprintf(ch,"Fitness = %f",Fitness[c]);
            outtextxy(800,20*(num_of_nodes_per_layer[num_of_layers]+1),ch);
            swapbuffers();
        }
        //GA based backward_propagation
        //mating pool
        GENETIC::mating_pool(chromosomes,Fitness,population_size,chromosome_length);
        //select pairs of chromosomes to have sex
        GENETIC::marriage(chromosomes,population_size,chromosome_length);
        //mutate each of them with small mutation probability
        for(int c=0; c<population_size; c++)
            GENETIC::mutation(0.1,chromosomes[c],chromosome_length);
        //visualize offsprings chromosomes in console

        swapbuffers();
        MAX_GENERATION--;
    }
    std::cout<<"\n total iterations "<<500-MAX_GENERATION<<"\n";

//Trial and error.........
    while(true)
    {
        std::cout<<"trial "<<std::endl;
        for(int i=0; i<num_of_nodes_per_layer[0]; i++)
        {
            std::cout<<"Input"<<i<<" : ";
            std::cin>>INOUTS[0][0][i];
        }
        for(int i=0; i<num_of_layers; i++)
        {
            MATRIX_OPS::MATRIX_MUL(INOUTS[i],1,num_of_nodes_per_layer[i],WEIGHTS[i],num_of_nodes_per_layer[i],num_of_nodes_per_layer[i+1],INOUTS[i+1]);
            MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[i+1],1,num_of_nodes_per_layer[i+1],5);//k=5
            NEURAL_GRAPHIC::view_node(INOUTS[i],i,num_of_nodes_per_layer[i]);
            NEURAL_GRAPHIC::view_network(i,WEIGHTS[i],num_of_nodes_per_layer[i],num_of_nodes_per_layer[i+1]);
        }
        NEURAL_GRAPHIC::view_node(INOUTS[num_of_layers],num_of_layers,num_of_nodes_per_layer[num_of_layers]);
        float ep=0;
        for(int i=0; i<num_of_nodes_per_layer[num_of_layers]; i++)
        {
            ep+=pow(expected[i]-INOUTS[num_of_layers][0][i],2);
        }
        ep/=2;
        char chr[100];
        sprintf(chr,"Error = %f",ep);
        outtextxy(800,20*(num_of_nodes_per_layer[num_of_layers]+1),chr);
        swapbuffers();
        char c;
        std::cout<<"Exit?Y/N : ";
        std::cin>>c;
        if(c=='Y')
            break;
    }
    getch();
    closegraph();
    return 0;
}

///some codes for visualizing in consoles [paste them after the comment in the main code]

//[paste before]visualizing each chromosomes
        /*std::cout<<"chromosome "<<i<<": ";
        for(int j=0; j<chromosome_length; j++)
        {
         std::cout<<chromosomes[i][j];
        }
        std::cout<<std::endl;*/

//[paste before]after extraction,visualizing the weights in console;
/*std::cout<<"\n extracted weight from chromosome"<<c<<":\n";
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
            }*/

//[paste before]visualize offsprings chromosomes in console
         /*std::cout<<"\nNEW OFFSPRINGS:\n";
         for(int i=0; i<population_size; i++)
         {
             std::cout<<"chromosome "<<i<<": ";
             for(int j=0; j<chromosome_length; j++)
             {
                 std::cout<<chromosomes[i][j];
             }
             std::cout<<std::endl;
         }*/
