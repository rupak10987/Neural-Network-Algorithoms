#include<cstring>
namespace GENETIC
{

void crossover(char* Pa,char* Pb,int len);
void marriage(char** chromosome,int population,int ch_len);
void gen_random_chromosome(char* holder,int len);
void mutation(float mutation_probability,char* host,int len);
void mating_pool(char**chromosome,float* Fitness, int num_of_chromosome,int ch_len);
void weight_extraction(char* chromosome, float* weight,int chrmsm_len,int gene_len);



void mating_pool(char**chromosome,float* Fitness, int num_of_chromosome,int ch_len)
{
//find out fmax, fmin
    float fmax[2],fmin[2];
    fmax[0]=-1000;//a small value
    fmax[1]=0;//initialized index
    fmin[0]=1000;//a large value
    fmin[1]=num_of_chromosome-1;//initialized index

//calculate fmax and fmin
    for(int i=0; i<num_of_chromosome; i++)
    {
        if(fmax[0]<=Fitness[i])
        {
            fmax[0]=Fitness[i];
            fmax[1]=i;
        }
        if(fmin[0]>=Fitness[i])
        {
            fmin[0]=Fitness[i];
            fmin[1]=i;
        }
    }

//replace fmin with fmax
    Fitness[(int)fmin[1]]=Fitness[(int)fmax[1]];
    for(int c=0;c<ch_len+1;c++)
    chromosome[(int)fmin[1]][c]=chromosome[(int)fmax[1]][c];

}


void marriage(char** chromosome,int population,int ch_len)
{
    //first shuffle the whole chromosome_list
    for(int i=0;i<population-1;i++)
    {
        int index=rand()%population;
        char temp[ch_len+1];

        for(int c=0;c<ch_len+1;c++)
        temp[c]=chromosome[i][c];

        for(int c=0;c<ch_len+1;c++)
        chromosome[i][c]=chromosome[index][c];

        for(int c=0;c<ch_len+1;c++)
        chromosome[index][c]=temp[c];

    }

    for(int i=0;i<population-1;i+=2)
    {
        GENETIC::crossover(chromosome[i],chromosome[i+1],ch_len);
    }
}

void weight_extraction(char* chromosome, float* weight,int chrmsm_len,int gene_len)
{
    for(int k=0; k<(chrmsm_len/gene_len); k++)
    {
//whats the first value of the gene
        int x1=(int)chromosome[k*5]-(int)'0';
        int x2=(int)chromosome[k*5+1]-(int)'0';
        int x3=(int)chromosome[k*5+2]-(int)'0';
        int x4=(int)chromosome[k*5+3]-(int)'0';
        int x5=(int)chromosome[k*5+4]-(int)'0';
        if(x1>=5 && x1<=9)
        {
            weight[k]=(x2*pow(10,3)+x3*pow(10,2)+x4*10+x5)/(pow(10,4));
        }
        else if(x1>=0 && x1<5)
        {
            weight[k]=-(x2*pow(10,3)+x3*pow(10,2)+x4*10+x5)/(pow(10,4));
        }
    }
}

void gen_random_chromosome(char* holder,int len)
{
    for(int i=0; i<len; i++)
    {
        holder[i]='0'+rand()%10;
    }
    holder[len]='\0';
}

void crossover(char* Pa,char* Pb,int len)
{
    int site1=rand()%len;
    int site2=rand()%len;
    if(site1>site2)
    {
        site1+=site2;
        site2=site1-site2;
        site1-=site2;
    }
    for(int i=site1; i<site2; i++)
    {
        char temp;
        temp=Pa[i];
        Pa[i]=Pb[i];
        Pb[i]=temp;
    }
}

void mutation(float mutation_probability,char* host,int len)
{

    int wheel=rand()%101;
    if(wheel<mutation_probability*101)
    {
        int site=rand()%len+1;
        host[site]='0'+rand()%10;
    }
}
}

