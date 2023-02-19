#include<iostream>
#include<math.h>
double euclid_dist(double x1,double x2,double y1,double y2)
{
    return pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}
int main()
{
    //initialising variables
    int k;
    std::cout<<"enter K : ";
    std::cin>>k;
    int num_of_traindata=10;
    int num_of_testdata=3;


    double** Train_data;
    Train_data=new double*[num_of_traindata];
    for(int i=0; i<num_of_traindata; i++)
        Train_data[i]=new double[3];//for now only two dimension or in other words two feature vectors
    //TRAIN DATA TABLE
    std::cout<<"------TRAIN DATA TABLE-------"<<std::endl;
    Train_data[0][0]=10;Train_data[0][1]=1;Train_data[0][2]=1;//weight | height | class(1=A and 2=B)
    Train_data[1][0]=11;Train_data[1][1]=2;Train_data[1][2]=1;
    Train_data[2][0]=12;Train_data[2][1]=1;Train_data[2][2]=1;
    Train_data[3][0]=14;Train_data[3][1]=3;Train_data[3][2]=1;
    Train_data[4][0]=16;Train_data[4][1]=1;Train_data[4][2]=1;
    Train_data[5][0]=2;Train_data[5][1]=10;Train_data[5][2]=2;
    Train_data[6][0]=1;Train_data[6][1]=11;Train_data[6][2]=2;
    Train_data[7][0]=0;Train_data[7][1]=12;Train_data[7][2]=2;
    Train_data[8][0]=5;Train_data[8][1]=11;Train_data[8][2]=2;
    Train_data[9][0]=4;Train_data[9][1]=9;Train_data[9][2]=2;
    //view train_data table in console
    std::cout<<"\t"<<"weight"<<"\theight\tclass"<<std::endl;
    for(int i=0; i<num_of_traindata; i++)
    {

        std::cout<<"entry"<<i<<"\t";
        for(int j=0; j<3; j++)
            std::cout<<Train_data[i][j]<<"\t";
        std::cout<<std::endl;
    }


    double** Test_data;
    Test_data=new double*[num_of_testdata];
    for(int i=0; i<num_of_testdata; i++)
        Test_data[i]=new double[3];
    //TEST DATA TABLE
    std::cout<<"\n\n-------TEST DATA TABLE-------"<<std::endl;
    Test_data[0][0]=11;Test_data[0][1]=1;Test_data[0][2]=1;//weight | height | true class(1 for classA & 2 for class B)
    Test_data[1][0]=8;Test_data[1][1]=5;Test_data[1][2]=2;
    Test_data[2][0]=12;Test_data[2][1]=10;Test_data[2][2]=2;
    //view test data table in console
    std::cout<<"\t"<<"weight"<<"\theight\ttrue class"<<std::endl;
    for(int i=0; i<num_of_testdata; i++)
    {
        std::cout<<"entry"<<i<<"\t";
        for(int j=0; j<3; j++)
            std::cout<<Test_data[i][j]<<"\t";
        std::cout<<std::endl;
    }
std::cout<<std::endl;


    //K NEAREST NEIGHBOR
    double distances[k][2];//an array to store measured distances from the test subject to its nearest
    //k neighbours and the class of the neighbour
    int closest_dist_nums_of_cA;//a variable to store the num of class1 closest neighbiors in the distances array
    int closest_dist_nums_of_cB;//a variable to store the num of class2 closest neighbiors in the distances array
    for(int i=0; i<num_of_testdata; i++)//iterate through test subjects
    {
        std::cout<<"\n\n-->For Test entry"<<i<<" closest neighbors distances and class :"<<std::endl;
        for(int j=0; j<k; j++)
        {
            distances[j][0]=10000;//initilize the distance value
            distances[j][1]=-1;//class
        }
        closest_dist_nums_of_cA=0;
        closest_dist_nums_of_cB=0;
        for(int j=0; j<num_of_traindata; j++)//iterate through the Train subjects for every test subject
        {

                double dist=euclid_dist(Train_data[j][0],Test_data[i][0],Train_data[j][1],Test_data[i][1]);
                for(int n=0;n<k;n++)
                {
                    if(dist<distances[n][0])
                    {
                        distances[n][0]=dist; // k closest distances from the test subject
                        distances[n][1]=Train_data[j][2]; //and their classes
                        break;
                    }
                }

        }
        for(int j=0;j<k;j++)
        {
            if(distances[j][1]==1)
                closest_dist_nums_of_cA+=1;
            else
                closest_dist_nums_of_cB+=1;
        }

        //printing result in console
        for(int n=0;n<k;n++)
        {
        std::cout<<"dist"<<n<<"="<<distances[n][0]<<" & cls="<<distances[n][1]<<",\t";
        }

        std::cout<<"\nnums of c1="<<closest_dist_nums_of_cA<<" nums of c2="<<closest_dist_nums_of_cB<<std::endl;
        if(closest_dist_nums_of_cA>=closest_dist_nums_of_cB)
            std::cout<<"So tested class of "<<i<<"'s class = 1"<<std::endl;
        else
            std::cout<<"tested class of "<<i<<"'s class = 2"<<std::endl;
    }

    return 0;


}


