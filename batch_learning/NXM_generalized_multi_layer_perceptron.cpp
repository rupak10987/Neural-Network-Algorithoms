#include<iostream>
#include<math.h>
#include <string>
#include <string.h>
#include<time.h>
#include<vector>
//all connected networks
float e=2.718281828;
class inputline//each node will contain one or more inputline
{
public:
    float input;
    float weight=0.3;
    float weight_before_update=0;
    std::string from;//for printing
    std::string to;//for printing
};
class Node
{
public:
    float bias=0;//default
    float out;//out put of this node
    float expected_out;//only for outlayer nodes
    float sigma;//for back propagation
    class inputline *inputs=nullptr;//inputs on this node
};
class Node **network;//all connected network
void Soma(int line_nums,class Node* received);
float sigmoid(float x);
void adjust_wait_out_layer(class Node* rcvd,int num_of_lines);
int main()
{
    std::vector<float> errors_collected;
    std::vector<float> weight1;
    std::vector<float> weight2;
    float *errors;
    float **batch;
    srand(time(NULL));
    int Num_of_layers;//input wont be considered as a layer
    int Num_Of_nodes_per_layer;
    int num_of_patterns_to_be_batched;
    std::cout<<"Num of Layers[input doesnt count as a layer] : ";
    std::cin>>Num_of_layers;
    std::cout<<"Num of nodes each layer : ";
    std::cin>>Num_Of_nodes_per_layer;
    std::cout<<"Num pattern to be batched : ";
    std::cin>>num_of_patterns_to_be_batched;
//initialization of nodes;
    network=new Node*[Num_of_layers];
    for(int i=0; i<Num_of_layers; i++)
    {
        network[i]=new Node[Num_Of_nodes_per_layer];
    }
//initialization of network lines
    for(int i=0; i<Num_of_layers; i++)
    {
        for(int j=0; j<Num_Of_nodes_per_layer; j++)
        {
            network[i][j].inputs=new inputline[Num_Of_nodes_per_layer];
        }
    }
    batch=new float*[num_of_patterns_to_be_batched];
//giving inputs to the network
    for(int k=0; k<num_of_patterns_to_be_batched; k++)
    {
        batch[k]=new float[Num_Of_nodes_per_layer];
        for(int i=0; i<Num_Of_nodes_per_layer; i++) //connecting both inputs to each node in hidden layer
        {
            float in;
            std::cout<<"Input "<<i<<" : ";
            std::cin>>in;
            batch[k][i]=in;
            for(int j=0; j<Num_Of_nodes_per_layer; j++)
            {
                network[0][i].inputs[j].input=in;
            }
        }
    }


//weights initialization
    char choice;
    std::cout<<" Manually assign weights(M) / Random weight assign(R): ";
    std::cin>>choice;
    for(int i=0; i<Num_of_layers; i++)
    {
        for(int j=0; j<Num_Of_nodes_per_layer; j++)
        {
            for(int k=0; k<Num_Of_nodes_per_layer; k++)
            {
                std::cout<<"Input weight of Line "<<i<<k<<"TO"<<i+1<<j<<" : ";
                if(choice=='M')
                {
                    std::cin>>network[i][j].inputs[k].weight;
                }
                else
                {
                    network[i][j].inputs[k].weight=(float)(1+(rand()%100))/100.00;
                    std::cout<<network[i][j].inputs[k].weight<<std::endl;
                }
                std::string from_num= {char(i+48),char(k+48)};
                network[i][j].inputs[k].from=from_num;
                std::string to_num= {char(i+1+48),char(j+48)};
                network[i][j].inputs[k].to=to_num;
            }
        }
    }
    /*Weights for ref
      W00to10=0.1
      W01to10=0.4
      W00to11=0.2
      W01to11=0.3
      w10to20=0.2
      w11to20=0.1
      w10to21=0.4
      w11to21=0.3*/

    for(int i=0; i<Num_Of_nodes_per_layer; i++)
    {
        std::cout<<"Expected out"<<i<<" : ";
        std::cin>>network[Num_of_layers-1][i].expected_out;
    }
    int iter=0;
    while(iter<2000)
    {
        float Ep=0;
        //forward pass
        for(int b_iter=0; b_iter<num_of_patterns_to_be_batched; b_iter++)
        {
            std::cout<<"FOR PATTERN : ";
            for(int a=0; a<Num_Of_nodes_per_layer; a++)
            {
                for(int b=0; b<Num_Of_nodes_per_layer; b++)
                {
                    network[0][a].inputs[b].input=batch[b_iter][a];
                }
                std::cout<<batch[b_iter][a]<<", ";
            }
            for(int i=0; i<Num_of_layers; i++) //every layer
            {
                for(int j=0; j<Num_Of_nodes_per_layer; j++) //every node
                {
                    Soma(Num_Of_nodes_per_layer,&network[i][j]);
                    std::cout<<"\nout of layer"<<i<<" Node"<<j<<" = "<<network[i][j].out;
                    if(i!=Num_of_layers-1)
                    {
                        for(int k=0; k<Num_Of_nodes_per_layer; k++)
                        {
                            network[i+1][k].inputs[j].input=network[i][j].out;
                        }
                    }
                }
            }
            for(int itr=0; itr<Num_Of_nodes_per_layer; itr++)
            {
                Ep+=pow(network[Num_of_layers-1][itr].expected_out-network[Num_of_layers-1][itr].out,2);
            }
        }
        Ep/=2;
        std::cout<<"Ep = "<<Ep<<std::endl;
        errors_collected.push_back(Ep);
        if(Ep<=0.1)
        {
            break;
        }
//back propagation
        for(int i=Num_of_layers-1; i>=0; i--)
        {
            for(int j=0; j<Num_Of_nodes_per_layer; j++)
            {
                if(i==Num_of_layers-1)//out layer
                {
                    adjust_wait_out_layer(&network[i][j],Num_Of_nodes_per_layer);
                }
                else//hidden layer
                {
                    //adjust_wait_hidden_layer(&network[i][j],&network[i+1][0],&network[i+1][1],j);
                    float big_K=10;
                    float miu=1;
                    float sum_of_sigmaK_wjk=0;
                    for(int k=0; k<Num_Of_nodes_per_layer; k++)
                    {
                        sum_of_sigmaK_wjk+=(network[i+1][k].sigma)*(network[i+1][k].inputs[j].weight_before_update);
                    }
                    network[i][j].sigma=big_K*network[i][j].out*(1-network[i][j].out)*sum_of_sigmaK_wjk;
                    for(int k=0; k<Num_Of_nodes_per_layer; k++)
                    {
                        network[i][j].inputs[i].weight_before_update=network[i][j].inputs[i].weight;
                        network[i][j].inputs[i].weight=network[i][j].inputs[i].weight + miu*network[i][j].sigma*network[i][j].out;
                        std::cout<<"adjusted weight of line "<<network[i][j].inputs[i].from<<" - "<<network[i][j].inputs[i].to<<" is "<<network[i][j].inputs[i].weight<<std::endl;
                    }
                }
            }
        }
        iter++;
        std::cout<<std::endl;
    }
    std::cout<<"number of iterations taken for convergence = "<<iter<<std::endl;
    for(int i=0; i < errors_collected.size(); i++)
    {
      std::cout<< errors_collected.at(i)<<",";
    }
    std::cout<<std::endl;
//trial phase

    while(true)
    {
        std::cout<<"now give a test pattern for previous expected output"<<std::endl;
        for(int i=0; i<Num_Of_nodes_per_layer; i++) //connecting both inputs to each node in hidden layer
        {
            float in;
            std::cout<<"Input "<<i<<" : ";
            std::cin>>in;
            for(int j=0; j<Num_Of_nodes_per_layer; j++)
            {
                network[0][i].inputs[j].input=in;
            }
        }
        for(int i=0; i<Num_of_layers; i++) //every layer
        {
            for(int j=0; j<Num_Of_nodes_per_layer; j++) //every node
            {
                Soma(Num_Of_nodes_per_layer,&network[i][j]);
                std::cout<<"\nout of layer"<<i<<" Node"<<j<<" = "<<network[i][j].out;
                if(i!=Num_of_layers-1)
                {
                    for(int k=0; k<Num_Of_nodes_per_layer; k++)
                    {
                        network[i+1][k].inputs[j].input=network[i][j].out;
                    }
                }
            }
        }
        char E;
        std::cout<<"EXIT ? [Y/N] :";
        std::cin>>E;
        if(E=='Y')
            break;
    }


    return 0;
}


void Soma(int line_nums,class Node* received)
{
    float net_s=0;
    for(int i=0; i<line_nums; i++)
    {
        net_s+=(received->inputs[i].input)*(received->inputs[i].weight);
    }
    net_s+=received->bias;
    received->out=sigmoid(net_s);
};


float sigmoid(float x)
{
    int k=10;
    return 1/(1+pow(e,-k*x));
}

void adjust_wait_out_layer(class Node* rcvd,int num_of_lines)
{
    float big_K=10;
    float miu=0.9;
    rcvd->sigma=big_K*rcvd->out*(1-rcvd->out)*(rcvd->expected_out-rcvd->out);
    for(int i=0; i<num_of_lines; i++)
    {
        rcvd->inputs[i].weight_before_update=rcvd->inputs[i].weight;
        rcvd->inputs[i].weight=rcvd->inputs[i].weight + miu*rcvd->sigma*rcvd->out;
        std::cout<<"adjusted weight of line "<<rcvd->inputs[i].from<<" - "<<rcvd->inputs[i].to<<" is "<<rcvd->inputs[i].weight<<std::endl;
    }
}



