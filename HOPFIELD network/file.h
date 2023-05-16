#include <fstream>
#include <string>
#include <sstream>
namespace read_binary_file_for_hopfield
{
void fetch_ins(float **ins,int filenumber,int size_of_image)
{
    std::string str;
    str = std::to_string(filenumber);
    str=str+".txt";
    const char*  filename = str.c_str();
    std::cout<<filename<<std::endl;

    char chr[100];
    sprintf(chr,"input%d",filenumber);
    outtextxy(25+filenumber*(size_of_image),size_of_image+20,chr);

    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        std::cerr << "Error opening the file." << std::endl;
    }
    int itr=0;
    char ch;
    while (inputFile.get(ch))
    {
        int a=(int)ch-(int)'0';
        if(a!=-38)
        {
            if(a!=0)
            {
              ins[0][itr]=(float)a;
              //image enhance
              int x=(filenumber*11)+(itr)%10;
              int y=(int)(itr/10);
              for(int m=x*10;m<x*10+10;m++)
                for(int n=y*10;n<y*10+10;n++)
                    putpixel(m,n,GREEN);
            }

            else
            {
               ins[0][itr]=(float)-1;
            }
            itr++;
        }
    }
    inputFile.close();
}


void fetch_miu(float **ins,int size_of_image)
{
    std::cout<<"miu.txt\n";
    std::ifstream inputFile("miu.txt");

    char chr[100];
    sprintf(chr,"miu[0]");
    outtextxy(25,220,chr);

    if (!inputFile)
    {
        std::cerr << "Error opening the file." << std::endl;
    }
    int itr=0;
    char ch;
    while (inputFile.get(ch))
    {
        int a=(int)ch-(int)'0';
        if(a!=-38)
        {
            if(a!=0)
            {
                ins[0][itr]=(float)a;
                int x=(itr)%10;
                int y=(int)(itr/10);
                for(int m=x*10;m<x*10+10;m++)
                for(int n=y*10;n<y*10+10;n++)
                putpixel(m,n+150,YELLOW);

            }
            else
            {
            ins[0][itr]=(float)0;
            }

            itr++;
        }
    }
    inputFile.close();
}

}

