#include<iostream>
#include<time.h>
int find_avails(int **board,int **avails);
//takes the board and a
//result 2d_array(for storing available place's indexs) and
//returns num of avails
void print_board(int **board);
int CHECK_WIN(int **board);
void initialize_board(int** board);
int main()
{
    srand(time(0));
    //make the board
    int **board;
    board =new int*[3];
    for(int i=0; i<3; i++)
    {
        board[i]=new int[3];
    }
    int num_play=1;
    int **avails;
    avails=new int*[2];
    avails[0]=new int[9];
    avails[1]=new int[9];
while(num_play<10)
{std::cout<<"PLAY NO "<<num_play<<"\n";
initialize_board(board);
int moves=0;
int mmoves=0;
    while(moves+mmoves<10)
    {
        //human plays
        std::cout<<"HUMAN MOVE "<<moves<<"\n";
        moves++;
        int r=find_avails(board,avails);
        r=rand()%r;
        board[avails[0][r]][avails[1][r]]=-1;
        print_board(board);
        if(CHECK_WIN(board))
        {
            std::cout<<"\nHUMAN WON\n";
            break;
        }
        //machine plays
        std::cout<<"MACHINE MOVE "<<mmoves<<"\n";
        mmoves++;
        r=find_avails(board,avails);
        r=rand()%r;//this will be decided by the ML
        board[avails[0][r]][avails[1][r]]=1;
        print_board(board);
        if(CHECK_WIN(board))
        {
            std::cout<<"\nHUMAN WON\n";
            break;
        }
    }
 num_play++;
}
    return 0;
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
int find_avails(int **board,int **avails)
{
    int index=0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(board[i][j]==0)
            {
                avails[0][index]=i;
                avails[1][index]=j;
                index++;
            }
        }
    }
    return index;
}
