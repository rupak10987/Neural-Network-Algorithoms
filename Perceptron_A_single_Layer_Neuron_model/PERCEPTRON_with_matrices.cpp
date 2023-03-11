#include<iostream>
#include<time.h>
void MATRIX_MUL(float** MAT_A,int num_of_row_A
                ,int num_of_col_A
                ,float** MAT_B
                ,int num_of_row_B
                ,int num_of_col_B
                ,float** result);
int main()
{
srand(time(0));
int num_of_nodes;
std::cout<<"Enter Number of Nodes :";
std::cin>>num_of_nodes;
float **INS;
float **WEIGHTS;
INS=new float*[1];//row vector
INS[0]=new float[num_of_nodes+1];
WEIGHTS=new float*[num_of_nodes+1];//collumn vector
for(int i=0;i<num_of_nodes+1;i++)
    WEIGHTS[i]=new float[1];

std::cout<<"\nINITIALIZE INPUTS AND WEIGHTS\n";
char choice;
std::cout<<"\nmanually initialize weights or randomly assign? M/R : ";
std::cin>>choice;
for(int i=0;i<num_of_nodes;i++)
{
 std::cout<<"\ninput "<<i<<" : ";
 std::cin>>INS[0][i];
 std::cout<<"weight on line "<<i<<" : ";
 if(choice=='R')
 {
    WEIGHTS[i][0]=rand()%10;
    std::cout<<WEIGHTS[i][0];
 }
 else
    std::cin>>WEIGHTS[i][0];
}
//taking bias input
std::cout<<"\nBias Input : ";
std::cin>>INS[0][num_of_nodes];
//seting bias weight=1
WEIGHTS[num_of_nodes][0]=1;
//taking expected inputs
float expected;
std::cout<<"Expected Output : ";
std::cin>>expected;

while(true)
{
//forward pass
float **result;
result=new float*[1];
result[0]=new float[1];
MATRIX_MUL(INS,1,num_of_nodes+1,WEIGHTS,num_of_nodes+1,1,result);
//threshold
if(result[0][0]>0)
    result[0][0]=1;
else
    result[0][0]=0;

float delta=expected-result[0][0];
std::cout<<"EXPECTED - OUT = "<<delta<<" (DELTA)\n";
if(delta==0)
    break;
//learn
float eta=0.1;
for(int i=0;i<num_of_nodes;i++)
{
    WEIGHTS[i][0]+=eta*delta*INS[0][i];
    std::cout<<"ADJUSTED WEIGHT OF LINE"<<i<<" : "<<WEIGHTS[i][0]<<std::endl;
}
std::cout<<"\n\n";
}
return 0;
}


void MATRIX_MUL(float** MAT_A
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
            for(int k = 0; k < num_of_col_A; ++k)
            {
                result[i][j] += MAT_A[i][k] * MAT_B[k][j];
            }
}
