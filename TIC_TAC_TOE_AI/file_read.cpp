#include <fstream>
#include<iostream>

int main()
{
int arr[100];
char c;
ifstream ifs;
ifs.open("2.txt");
int itr=0;
while (ifs >> c)
{
    arr[itr]=(int)(c-'0');
    itr++;
}
}


