#include<iostream>
#include<math.h>
class inputline
{
public:
    int input;
    float weight;
};
int Soma(int line_nums,class inputline* received);//performs weighted sum and thresholding
void adjust_weight1(int line_nums,class inputline* received,int out,int expected);
void adjust_weight2(int line_nums,class inputline* received,int out,int expected,float eta);
void adjust_wait3(int line_nums,int delta,float eta,class inputline* received);

int main()
{
int X=0;
char choice;
int expected;
class inputline* inputs;

std::cout<<"AND problem or GENERAL problem [A/G]?";
std::cin>>choice;

if(choice=='G')
{
std::cout<<"Enter Number of input lines : ";
std::cin>>X;
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
std::cout<<"\nExpected Output(0/1) : ";
std::cin>>expected;
}

else if(choice=='A')
{
X=3;
inputs=new inputline[X];
inputs[0].input=1;//bias input
inputs[0].weight=-2;//threashold
inputs[1].weight=1;//weight on input lines
inputs[2].weight=1;
for(int i=1;i<X;i++)
{
    std::cout<<"input on line X"<<i<<" : ";
    std::cin>>inputs[i].input;
    std::cout<<"weight on line"<<i<<" <auto assigned> = "<<inputs[i].weight<<std::endl;
}
//generate expected output table for and
expected=inputs[1].input && inputs[2].input;
std::cout<<"expected out for this input = "<<expected<<std::endl;
}

else
{
    std::cout<<"INVALID CHOICE"<<std::endl;
    return 0;
}



//perceptron
int nums_of_iteration=0;
while(true)
{
nums_of_iteration++;
int out=Soma(X,inputs);
std::cout<<"out="<<out<<std::endl;
int delta=expected-out;
if(delta==0)
    break;
std::cout<<"\nnew adjusted weights on iteration"<<nums_of_iteration<<" are :\n";
adjust_wait3(X,delta,0.2,inputs);//for rule 4a in the algoritm part of the lab report "eta" = 1

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

void adjust_wait3(int line_nums,int delta,float eta,class inputline* received)
{
for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight+(float)delta*eta*(float)received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}

void adjust_weight2(int line_nums,class inputline* received,int out,int expected,float eta)
{
if(expected-out==0)
    return;
else if(expected>out)
{
for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight+eta*received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}
else
{
for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight-eta*received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}
}

void adjust_weight1(int line_nums,class inputline* received,int out,int expected)
{
if(expected-out==0)
    return;
else if(expected>out)
{
for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight+received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}
else
{
    for(int i=1;i<line_nums;i++)
{
received[i].weight=received[i].weight-received[i].input;
std::cout<<"Line"<<i<<" new weight = "<<received[i].weight<<std::endl;
}
}
}


