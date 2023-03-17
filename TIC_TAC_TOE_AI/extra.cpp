#include<iostream>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include"MATRIX_SCRATCH.h"
#include"GRAPHICAL_VIEW_NEURO.h"
#include<math.h>
#include<fstream>
int find_avails(int **board,int *avails);
void print_board(int **board);
int CHECK_WIN(int **board);
void initialize_board(int** board);
float forward_pass(float ***INOUTS,float *expected,float ***WEIGHTS,float ***sigmas,int num_of_layers,int *num_of_nodes_per_layer,bool show);
void back_pass(float ***INOUTS,float *expected,float ***WEIGHTS,float ***sigmas,int num_of_layers,int *num_of_nodes_per_layer);


int main()
{
    int gd = DETECT, gm;
    initwindow(1000, 1000);
    //......................initializing the machine.......................//
    srand(time(NULL));
    int num_of_layers=2;
    int *num_of_nodes_per_layer;
    num_of_nodes_per_layer=new int[num_of_layers+1];
    for(int i=0; i<num_of_layers+1; i++)
    {
        num_of_nodes_per_layer[i]=9;
    }
    //initializing in/out and weight matrices
    float ***INOUTS;
    float ***WEIGHTS;
    float *expected;
    expected=new float[num_of_nodes_per_layer[num_of_layers]];
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
    for(int i=0; i<num_of_layers; i++)
        for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
            for(int k=0; k<num_of_nodes_per_layer[i]; k++)
            {
                WEIGHTS[i][k][j]=(float)((rand()%100)+1)/100.00;
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

    //.............make the board.............//
    int **board;
    board =new int*[3];
    for(int i=0; i<3; i++)
    {
        board[i]=new int[3];
    }
    int num_play=1;
    int *avails;
    avails=new int[10];
    int *previous_moves;
    previous_moves=new int[2];

    //...............training play..................//
    char choice;
    std::cout<<"MAKE THE MACHINE LERAN[L] OR PLAY[P] ? ";
    std::cin>>choice;
    if(choice=='L')
    {
        while(num_play<=1000)
        {
            std::cout<<"PLAY NO "<<num_play<<"\n";
            initialize_board(board);
            int moves=0;
            int mmoves=0;
            while(true)
            {
                //human plays
                std::cout<<"HUMAN MOVE "<<moves<<"\n";
                moves++;
                if(num_play<100)
                {

                }
                int r=find_avails(board,avails);
                if(r==0)
                {
                    //no avail able slots
                    std::cout<<"DRAW\n";
                    break;
                }

                r=rand()%r;
                board[avails[r]/3][avails[r]%3]=-1;
                previous_moves[0]=avails[r];
                print_board(board);
                if(CHECK_WIN(board)==-1)
                {
                    std::cout<<"\nHUMAN WON\n";
                    //make the machine learn
                    //back track the board two moves with the help of previos_moves
                    board[previous_moves[0]/3][previous_moves[0]%3]=0;
                    board[previous_moves[1]/3][previous_moves[1]%3]=0;
                    //train the machine with expected output =
                    //board state and the new move at being where it lost previos[0];
                    for(int i=0; i<9; i++)
                    {
                        INOUTS[0][0][i]=board[i/3][i%3];
                        if(board[i/3][i%3]!=0)
                            expected[i]=0;
                        else
                            expected[i]=0.6;
                    }
                    expected[previous_moves[0]]=1;
                    std::cout<<"\nLearning..................................\n";
                    float ep=100;
                    float prev_ep=101;
                    int max_iter=100;
                    while(max_iter>0)
                    {
                        ep=forward_pass(INOUTS,expected,WEIGHTS,sigmas,num_of_layers,num_of_nodes_per_layer,true);
                        std::cout<<"EP="<<ep<<"\n";
                        if(ep<0.01 || prev_ep<ep)
                            break;
                        prev_ep=ep;
                        back_pass(INOUTS,expected,WEIGHTS,sigmas,num_of_layers,num_of_nodes_per_layer);
                        max_iter--;
                    }
                    break;
                }
                //machine plays
                std::cout<<"MACHINE MOVE "<<mmoves<<"\n";
                mmoves++;
                for(int i=0; i<9; i++)
                {
                    INOUTS[0][0][i]=board[i/3][i%3];
                }
                forward_pass(INOUTS,expected,WEIGHTS,sigmas,num_of_layers,num_of_nodes_per_layer,false);
                r=find_avails(board,avails);
                if(r==0)
                {
                    //no avail able slots
                    std::cout<<"DRAW\n";
                    break;
                }
                //find the best position highest one
                float max_out=-100;
                int best_pos=0;
                for(int i=0; i<9; i++)
                {
                    if(max_out<INOUTS[num_of_layers][0][i])
                    {
                        bool in=false;
                        for(int j=0; j<r; j++)
                        {
                            if(avails[j]==i)
                            {
                                in=true;
                                break;
                            }
                        }
                        if(in)
                        {
                            max_out=INOUTS[num_of_layers][0][i];
                            best_pos=i;
                        }
                    }
                }
                board[best_pos/3][best_pos%3]=1;
                previous_moves[1]=best_pos;
                print_board(board);
                if(CHECK_WIN(board)==1)
                {
                    std::cout<<"\MACHINE WON\n";
                    break;
                }
            }
            num_play++;
        }

        //save weights
        std::ofstream WEIGHT01("WEIGHT01.txt");
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                WEIGHT01<<WEIGHTS[0][i][j];
                WEIGHT01<<" ";
            }
            WEIGHT01<<"\n";
        }
        WEIGHT01.close();
        std::ofstream WEIGHT12("WEIGHT12.txt");
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                WEIGHT12<<WEIGHTS[1][i][j];
                WEIGHT12<<" ";
            }
            WEIGHT12<<"\n";
        }
        WEIGHT12.close();
    }


    std::ifstream INWEIGHT01("WEIGHT01.txt");
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            INWEIGHT01 >> WEIGHTS[0][i][j];
        }
    }
    INWEIGHT01.close();
    std::ifstream INWEIGHT12("WEIGHT12.txt");
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            INWEIGHT12 >> WEIGHTS[1][i][j];
        }
    }
    INWEIGHT12.close();

    initialize_board(board);
    //......play
    while(true)
    {
        int A=find_avails(board,avails);
        int pos;
        std::cout<<"ENTER POSITION[0-8] : ";
        std::cin>>pos;
        //finding the correct input from user
        while(true)
        {
            bool flag=false;
            for(int i=find_avails(board,avails); i>=0; i--)
            {
                if(pos==avails[i])
                {
                    flag=true;
                    break;
                }
            }
            if(flag)
            {
                board[pos/3][pos%3]=-1;
                break;
            }
            else
            {
                std::cout<<"INVALID POSITION\nENTER POSITION[0-8] : ";
                std::cin>>pos;
            }
        }
        print_board(board);
        if(CHECK_WIN(board)==-1)
        {
            std::cout<<"\HUMAN WON\n";
            break;
        }
        //let the machine play
        std::cout<<"\nmachine move\n";
        for(int i=0; i<9; i++)
        {
            INOUTS[0][0][i]=board[i/3][i%3];
        }
        forward_pass(INOUTS,expected,WEIGHTS,sigmas,num_of_layers,num_of_nodes_per_layer,false);
        int
        r=find_avails(board,avails);
        if(r==0)
        {
            //no avail able slots
            std::cout<<"DRAW\n";
            break;
        }
        //find the best position highest one
        float max_out=-100;
        int best_pos=0;
        for(int i=0; i<9; i++)
        {
            if(max_out<INOUTS[num_of_layers][0][i])
            {
                bool in=false;
                for(int j=0; j<r; j++)
                {
                    if(avails[j]==i)
                    {
                        in=true;
                        break;
                    }
                }
                if(in)
                {
                    max_out=INOUTS[num_of_layers][0][i];
                    best_pos=i;
                }
            }
        }
        board[best_pos/3][best_pos%3]=1;
        previous_moves[1]=best_pos;
        print_board(board);
        if(CHECK_WIN(board)==1)
        {
            std::cout<<"\MACHINE WON\n";
            break;
        }
    }

    //if loss learn
    return 0;
}










float forward_pass(float ***INOUTS,float *expected,float ***WEIGHTS,float ***sigmas,int num_of_layers,int *num_of_nodes_per_layer,bool show)
{
    float k=2;
    cleardevice();
    for(int i=0; i<num_of_layers; i++)
    {
        MATRIX_OPS::MATRIX_MUL(INOUTS[i],1
                               ,num_of_nodes_per_layer[i]
                               ,WEIGHTS[i],num_of_nodes_per_layer[i]
                               ,num_of_nodes_per_layer[i+1],INOUTS[i+1]);
        MATRIX_OPS::MATRIX_SIGMOID_TRANSFORM(INOUTS[i+1],1,num_of_nodes_per_layer[i+1],k);

        //visualize to window
       // NEURAL_GRAPHIC::view_node(INOUTS[i],i,num_of_nodes_per_layer[i]);
        //NEURAL_GRAPHIC::view_network(i,WEIGHTS[i]
                                     //,num_of_nodes_per_layer[i]
                                     //,num_of_nodes_per_layer[i+1]);

    }
    //NEURAL_GRAPHIC::view_node(INOUTS[num_of_layers],num_of_layers,num_of_nodes_per_layer[num_of_layers]);
    for(int ex=0; ex<num_of_nodes_per_layer[num_of_layers]; ex++)
    {
        char ch[100];
        sprintf(ch,"EXPECTED OUTPUT %0.3f",expected[ex]);
       // outtextxy(800,20*(ex+1),ch);
    }
    float ep=0;
    for(int i=0; i<num_of_nodes_per_layer[num_of_layers]; i++)
    {
        ep+=pow(expected[i]-INOUTS[num_of_layers][0][i],2);
    }
    ep/=2;
    if(show)
    {
        char ch[100];
        sprintf(ch,"EP = %f",ep);
        //outtextxy(800,20*(num_of_nodes_per_layer[num_of_layers]+1),ch);

    }
    swapbuffers();
    return ep;
}






void back_pass(float ***INOUTS,float *expected,float ***WEIGHTS,float ***sigmas,int num_of_layers,int *num_of_nodes_per_layer)
{
    float k=2;
    float** T_saved_delw;
    for(int i=num_of_layers-1; i>=0; i--)
    {
        float miu=5;

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
            MATRIX_OPS::MATRIX_SUB(WEIGHTS[i+1],num_of_nodes_per_layer[i+1]
                                   ,num_of_nodes_per_layer[i+2],T_saved_delw
                                   ,num_of_nodes_per_layer[i+1]
                                   ,num_of_nodes_per_layer[i+2],T_saved_delw);
            //retrieved weight is transfered to T_saved_delw.
            //it's same as prev weights[i+1] dont get confused seeing delw below.
            MATRIX_OPS::MATRIX_MUL(T_saved_delw,num_of_nodes_per_layer[i+1]
                                   ,num_of_nodes_per_layer[i+2],sigmas[i+1]
                                   ,num_of_nodes_per_layer[i+2],1,sigmas[i]);//sum of sigK*Wjk
            for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
                sigmas[i][j][0]=k*INOUTS[i+1][0][j]*(1-INOUTS[i+1][0][j])*sigmas[i][j][0];
        }

        //STEP2: calculate transpose of the weight matric that is to be updated
        float **TRANSPOSED;
        TRANSPOSED=new float*[num_of_nodes_per_layer[i+1]];
        for(int r=0; r<num_of_nodes_per_layer[i+1]; r++)
            TRANSPOSED[r]=new float[num_of_nodes_per_layer[i]];
        MATRIX_OPS::MATRIX_Transpose(WEIGHTS[i],num_of_nodes_per_layer[i]
                                     ,num_of_nodes_per_layer[i+1]
                                     ,TRANSPOSED);

        //STEP3: find delW=miu*sig*op
        float **delw;//same dimension as transposed
        delw=new float*[num_of_nodes_per_layer[i+1]];
        T_saved_delw=new float*[num_of_nodes_per_layer[i]];
        for(int j=0; j<num_of_nodes_per_layer[i+1]; j++)
        {
            delw[j]=new float[num_of_nodes_per_layer[i]];
        }
        for(int j=0; j<num_of_nodes_per_layer[i]; j++)
        {
            T_saved_delw[j]= new float[num_of_nodes_per_layer[i+1]];
        }
        MATRIX_OPS::MATRIX_MUL(sigmas[i],num_of_nodes_per_layer[i+1],1
                               ,INOUTS[i],1,num_of_nodes_per_layer[i],delw);////////////
        MATRIX_OPS::MATRIX_Transpose(delw,num_of_nodes_per_layer[i+1]
                                     ,num_of_nodes_per_layer[i]
                                     ,T_saved_delw);//saving delw as transposed
        MATRIX_OPS::MATRIX_Constant_Multiply(delw,num_of_nodes_per_layer[i+1]
                                             ,num_of_nodes_per_layer[i],miu);

        //STEP4:weights(t+1)=T(weights(t))+delw
        MATRIX_OPS::MATRIX_ADD(TRANSPOSED,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i]
                               ,delw,num_of_nodes_per_layer[i+1],num_of_nodes_per_layer[i],TRANSPOSED);

        //STEP5: retranspose the weight matric
        MATRIX_OPS::MATRIX_Transpose(TRANSPOSED,num_of_nodes_per_layer[i+1]
                                     ,num_of_nodes_per_layer[i],WEIGHTS[i]);
    }
}


void initialize_board(int **board)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            board[i][j]=0;
        }
    }
}

int CHECK_WIN(int **board)
{
    int diagL=0;
    int diagR=0;
    for(int i=0; i<3; i++)
    {
        int row_win_sum=0;
        int coll_win_sum=0;
        for(int j=0; j<3; j++)
        {
            row_win_sum+=board[i][j];
            coll_win_sum+=board[j][i];
        }
        if(row_win_sum==3 || coll_win_sum==3)
            return 1;
        else if(row_win_sum==-3 || coll_win_sum==-3)
            return -1;
        diagL+=board[i][i];
        diagR+=board[2-i][2-i];
    }
    //diagonal check
    if(diagL==3 || diagR==3)
        return 1;
    else if(diagL==-3 || diagR==-3)
        return -1;
    else
        return 0;
}



void print_board(int **board)
{
    for(int i=0; i<3; i++)
    {
        std::cout<<"\n";
        for(int j=0; j<3; j++)
        {
            switch(board[i][j])
            {
            case 0:
            {
                if(j==1)
                    std::cout<<"   ";
                else
                    std::cout<<"|"<<"   "<<"|";
                break;
            }
            case 1:
            {
                if(j==1)
                    std::cout<<" O ";
                else
                    std::cout<<"|"<<" O "<<"|";
                break;
            }
            case -1:
            {
                if(j==1)
                    std::cout<<" X ";
                else
                    std::cout<<"|"<<" X "<<"|";
                break;
            }
            }
        }
        std::cout<<"\n_____________\n";
    }
}
int find_avails(int **board,int *avails)
{
    int index=0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(board[i][j]==0)
            {
                avails[index]=i*3+j;
                index++;
            }
        }
    }
    return index;
}
