#include<iostream>
#include<math.h>
class inputline
{
public:
    int input;
    float weight;
};
int Soma(int line_nums,class inputline* received);
void adjust_wait(int line_nums,int delta,float eta,class inputline* received);

int main()
{
int X=0;
int expected=0;
std::cout<<"Enter Number of input lines : ";
std::cin>>X;
class inputline* inputs;
inputs=new inputline[X];
std::cout<<"bias input : ";
std::cin>>inputs[0].input;
std::cout<<"threshold : ";
std::cin>>inputs[0].weight;
for(int i=1;i<X;i++)
{
    std::cout<<"input on line X"<<i<<" : ";
    std::cin>>inputs[i].input;
    std::cout<<"weight on line"<<i<<" : ";
    std::cin>>inputs[i].weight;
}
std::cout<<"\n\nExpected Output(0/1) : ";
std::cin>>expected;
//perceptron
while(true)
{
int out=Soma(X,inputs);
std::cout<<"out="<<out;
int delta=expected-out;
if(delta==0)
    break;
std::cout<<"\nnew adjusted weights are :\n";
adjust_wait(X,delta,0.2,inputs);

}
return 0;
}

int Soma(int line_nums,class inputline* received)
{
float sum=0;
for(int i=0;i<line_nums;i++)
{
  sum+=received[i].input*received[i].weight;
}
if(sum>0)
    return 1;
else
    return 0;


};

void adjust_wait(int line_nums,int delta,float eta,class inputline* received)
{
for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight+(float)delta*eta*(float)received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}
