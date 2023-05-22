#include<iostream>
#include<time.h>
#include<conio.h>
#include<math.h>
int NL=30,NM=60,NS=90,ZE=120,PS=150,PM=180,PL=210;
struct uXX
{
    float uNL=0;
    float uNM=0;
    float uNS=0;
    float uZE=0;
    float uPS=0;
    float uPM=0;
    float uPL=0;
};

float delta_time;
float oldTime=0;

class CAR
{
public:
    float posX=25;
    float posY=250;
    float accelaration=0;
    float speed=120;
    float throttle=0;
    float mass=1;

};

//func declarations
float minimum(float a,float b);
float maximum(float a,float b);
float CALC_AREA(float height);
float Apply_Rule_Base_And_Defuzzy(struct uXX* speed,struct uXX* accelaration);
float calculate_miu(float del1, float del2, float m1, float m2);
void membership(class CAR* car_FUZZY,const char* str,struct uXX* miu);


int main()
{
    //initialize the two cars one is random speed
    //another is controlled by fuzzy controller
    class CAR *car_AUTO=new CAR();

    class CAR *car_FUZZY=new CAR();
    std::cout<<"Speed Difference = ";
    std::cin>>car_FUZZY->speed;
    std::cout<<"acceleration = ";
    std::cin>>car_FUZZY->accelaration;

//DELTATIME AND FPS COUNTING
    delta_time =clock() - oldTime;//0.001;//
    delta_time/=1000;
    oldTime = clock();


//find membership value
    struct uXX miu_speed;
    struct uXX miu_accelaration;
    membership(car_FUZZY,"speed",&miu_speed);
    membership(car_FUZZY,"accelaration",&miu_accelaration);

    float THROTTLE=Apply_Rule_Base_And_Defuzzy(&miu_speed,&miu_accelaration);
    std::cout<<"THROTTLE = sum(Ai*Xi) / sum(Ai)"<<std::endl;
    std::cout<<"THROTTLE = "<<THROTTLE<<std::endl;

    return 0;
}

float minimum(float a,float b)
{
    if(a<=b)
        return a;
    else
        return b;
}
float maximum(float a,float b)
{
    if(a>=b)
        return a;
    else
        return b;
}

//defuzzification-based on center of gravity method
float CALC_AREA(float height)
{
    //base of the triangle a
    float a=60.0;
    //length of the triange cut at h height from base
    float b;
    b=a*(1-height);
    float area=0.5*height*(a+b);
    return area;
}


float calculate_miu(float del1, float del2, float m1, float m2)
{
    float miu1,miu2;
    miu1=del1*m1;
    miu2=del2*m2;
    if(miu1>=miu2)//minimum miu stored in miu1
    {
        miu1=miu2;
    }
    if(miu1<1)
        return miu1;
    else
        return 1;
}

void membership(class CAR* car_FUZZY,const char* str,struct uXX* miu)
{
    float x;
    if(str=="speed")
        x=car_FUZZY->speed;
    else if(str=="accelaration")
        x=car_FUZZY->accelaration;

    int inp=(int)(x/30);
    if(inp<0)
    {
        miu->uNL=1.0;
    }
    else if(inp>=8)
    {
        miu->uPL=1.0;
    }
    else
        switch(inp)
        {
        case 0://less than 30
        {
            miu->uNL=1.0;
            break;
        }
        case 1://31-60
        {
            miu->uNL=calculate_miu(x-(NL-30),NL+30-x,0,0.033);
            miu->uNM=calculate_miu(x-(NM-30),NM+30-x,0.033,0.033);
            break;
        }
        case 2:
        {
            miu->uNM=calculate_miu(x-(NM-30),NM+30-x,0.033,0.033);
            miu->uNS=calculate_miu(x-(NS-30),NS+30-x,0.033,0.033);
            break;
        }
        case 3:
        {
            miu->uNS=calculate_miu(x-(NS-30),NS+30-x,0.033,0.033);
            miu->uZE=calculate_miu(x-(ZE-30),ZE+30-x,0.033,0.033);
            break;
        }
        case 4:
        {
            miu->uZE=calculate_miu(x-(ZE-30),ZE+30-x,0.033,0.033);
            miu->uPS=calculate_miu(x-(PS-30),PS+30-x,0.033,0.033);
            break;
        }
        case 5:
        {
            miu->uPS=calculate_miu(x-(PS-30),PS+30-x,0.033,0.033);
            miu->uPM=calculate_miu(x-(PM-30),PM+30-x,0.033,0.033);
            break;
        }
        case 6:
        {
            miu->uPM=calculate_miu(x-(PM-30),PM+30-x,0.033,0.033);
            miu->uPL=calculate_miu(x-(PL-30),PL+30-x,0.033,0.033);
            break;
        }
        case 7:
        {
            miu->uPL=calculate_miu(x-(PL-30),PL+30-x,0.033,0);
            break;
        }
        }
}


float Apply_Rule_Base_And_Defuzzy(struct uXX* speed,struct uXX* accelaration)
{
///RULE BASE

//IF speed difference is NL AND acceleration is NL THEN throttle is PL
    float hPL=minimum(speed->uNL,accelaration->uNL);

//IF speed difference is NL AND acceleration is NM THEN throttle is PL
    float hPL2=minimum(speed->uNL,accelaration->uNM);

//IF speed difference is NL AND acceleration is NS THEN throttle is PM
    float hPM=minimum(speed->uNL,accelaration->uNS);

//IF speed difference is NL AND acceleration is ze THEN throttle is PM
    float hPM2=minimum(speed->uNL,accelaration->uZE);

//IF speed difference is NL AND acceleration is PS THEN throttle is PM
    float hPM3=minimum(speed->uNL,accelaration->uPS);

//IF speed difference is NL AND acceleration is PM THEN throttle is ze
    float hZE=minimum(speed->uNL,accelaration->uPM);

//IF speed difference is NL AND acceleration is PL THEN throttle is NS
    float hNS=minimum(speed->uNL,accelaration->uPL);

//IF speed difference is NM AND acceleration is NL THEN throttle is PM
    float hPM4=minimum(speed->uNM,accelaration->uNL);

//IF speed difference is NM AND acceleration is NM THEN throttle is PM
    float hPM5=minimum(speed->uNM,accelaration->uNM);

//IF speed difference is NM AND acceleration is NS THEN throttle is ze
    float hZE2=minimum(speed->uNM,accelaration->uNS);

//IF speed difference is NM AND acceleration is ze THEN throttle is NS
    float hNS2=minimum(speed->uNM,accelaration->uZE);

//IF speed difference is NM AND acceleration is PS THEN throttle is NS
    float hNS3=minimum(speed->uNM,accelaration->uPS);

//IF speed difference is NM AND acceleration is PM THEN throttle is NS
    float hNS4=minimum(speed->uNM,accelaration->uPM);

//IF speed difference is NM AND acceleration is PL THEN throttle is NM
    float hNM=minimum(speed->uNM,accelaration->uPL);

//IF speed difference is NS AND acceleration is NL THEN throttle is PM
    float hPM6=minimum(speed->uNS,accelaration->uNL);

//IF speed difference is NS AND acceleration is NM THEN throttle is ze
    float hZE3=minimum(speed->uNS,accelaration->uNM);

//IF speed difference is NS AND acceleration is NS THEN throttle is NS
    float hNS5=minimum(speed->uNS,accelaration->uNS);

//IF speed difference is NS AND acceleration is ze THEN throttle is NM
    float hNM2=minimum(speed->uNS,accelaration->uZE);

//IF speed difference is NS AND acceleration is PS THEN throttle is NM
    float hNM3=minimum(speed->uNS,accelaration->uPS);

//IF speed difference is NS AND acceleration is PM THEN throttle is NM
    float hNM4=minimum(speed->uNS,accelaration->uPM);

//IF speed difference is NS AND acceleration is PL THEN throttle is NM
    float hNM5=minimum(speed->uNS,accelaration->uPL);

//IF speed difference is ze AND acceleration is NL THEN throttle is PS
    float hPS=minimum(speed->uZE,accelaration->uNL);

//IF speed difference is ze AND acceleration is NM THEN throttle is PM
    float hPM7=minimum(speed->uZE,accelaration->uNM);

//IF speed difference is ze AND acceleration is NS THEN throttle is ze
    float hZE4=minimum(speed->uZE,accelaration->uNS);

//IF speed difference is ze AND acceleration is ze THEN throttle is ze
    float hZE5=minimum(speed->uZE,accelaration->uZE);

//IF speed difference is ze AND acceleration is PS THEN throttle is NS
    float hNS6=minimum(speed->uZE,accelaration->uPS);

//IF speed difference is ze AND acceleration is PM THEN throttle is NM
    float hNM6=minimum(speed->uZE,accelaration->uPM);

//IF speed difference is ze AND acceleration is PL THEN throttle is NM
    float hNM7=minimum(speed->uZE,accelaration->uPL);

//IF speed difference is PS AND acceleration is NL THEN throttle is PS
    float hPS2=minimum(speed->uPS,accelaration->uNL);

//IF speed difference is PS AND acceleration is NM THEN throttle is NS
    float hNS7=minimum(speed->uPS,accelaration->uNM);

//IF speed difference is PS AND acceleration is NS THEN throttle is NM
    float hNM8=minimum(speed->uPS,accelaration->uNS);

//IF speed difference is PS AND acceleration is ze THEN throttle is NM
    float hNM9=minimum(speed->uPS,accelaration->uZE);

//IF speed difference is PS AND acceleration is PS THEN throttle is NM
    float hNM10=minimum(speed->uPS,accelaration->uPS);

//IF speed difference is PS AND acceleration is PM THEN throttle is NS
    float hNS8=minimum(speed->uPS,accelaration->uPM);

//IF speed difference is PS AND acceleration is PL THEN throttle is NM
    float hNM11=minimum(speed->uPS,accelaration->uPL);

//IF speed difference is PM AND acceleration is NL THEN throttle is NS
    float hNS9=minimum(speed->uPM,accelaration->uNL);

//IF speed difference is PM AND acceleration is NM THEN throttle is NS
    float hNS10=minimum(speed->uPM,accelaration->uNM);

//IF speed difference is PM AND acceleration is NS THEN throttle is NM
    float hNM12=minimum(speed->uPM,accelaration->uNS);

//IF speed difference is PM AND acceleration is ze THEN throttle is NM
    float hNM13=minimum(speed->uPM,accelaration->uZE);

//IF speed difference is PM AND acceleration is PS THEN throttle is NM
    float hNM14=minimum(speed->uPM,accelaration->uPS);

//IF speed difference is PM AND acceleration is PM THEN throttle is NM
    float hNM15=minimum(speed->uPM,accelaration->uPM);

//IF speed difference is PM AND acceleration is PL THEN throttle is NS
    float hNS11=minimum(speed->uPM,accelaration->uPL);

//IF speed difference is PL AND acceleration is NL THEN throttle is NS
    float hNS12=minimum(speed->uPL,accelaration->uNL);

//IF speed difference is PL AND acceleration is NM THEN throttle is NM
    float hNM16=minimum(speed->uPL,accelaration->uNM);

//IF speed difference is PL AND acceleration is NS THEN throttle is NM
    float hNM17=minimum(speed->uPL,accelaration->uNS);

//IF speed difference is PL AND acceleration is ze THEN throttle is NM
    float hNM18=minimum(speed->uPL,accelaration->uZE);

//IF speed difference is PL AND acceleration is PS THEN throttle is NM
    float hNM19=minimum(speed->uPL,accelaration->uPS);

//IF speed difference is PL AND acceleration is PM THEN throttle is NS
    float hNS13=minimum(speed->uPL,accelaration->uPM);

//IF speed difference is PL AND acceleration is PL THEN throttle is NL
    float hNL=minimum(speed->uPL,accelaration->uPL);



///Taking the maximum of the rule that produce same throttle
    hPL=std::max(hPL,hPL2);//max of hpl

    hPM=std::max(hPM,hPM2);//max of hpm
    hPM=std::max(hPM,hPM3);
    hPM=std::max(hPM,hPM4);
    hPM=std::max(hPM,hPM5);
    hPM=std::max(hPM,hPM6);
    hPM=std::max(hPM,hPM7);
    hPS=std::max(hPS,hPS2);

    hZE=std::max(hZE,hZE2);//MAX OF hZE
    hZE=std::max(hZE,hZE3);
    hZE=std::max(hZE,hZE4);
    hZE=std::max(hZE,hZE5);

    hNS=std::max(hNS,hNS2);//MAX OF hNS
    hNS=std::max(hNS,hNS3);
    hNS=std::max(hNS,hNS4);
    hNS=std::max(hNS,hNS5);
    hNS=std::max(hNS,hNS6);
    hNS=std::max(hNS,hNS7);
    hNS=std::max(hNS,hNS8);
    hNS=std::max(hNS,hNS9);
    hNS=std::max(hNS,hNS10);
    hNS=std::max(hNS,hNS11);
    hNS=std::max(hNS,hNS12);
    hNS=std::max(hNS,hNS13);

    hNM=std::max(hNM,hNM2);//MAX OF hNM
    hNM=std::max(hNM,hNM3);
    hNM=std::max(hNM,hNM4);
    hNM=std::max(hNM,hNM5);
    hNM=std::max(hNM,hNM6);
    hNM=std::max(hNM,hNM7);
    hNM=std::max(hNM,hNM8);
    hNM=std::max(hNM,hNM9);
    hNM=std::max(hNM,hNM10);
    hNM=std::max(hNM,hNM11);
    hNM=std::max(hNM,hNM12);
    hNM=std::max(hNM,hNM13);
    hNM=std::max(hNM,hNM14);
    hNM=std::max(hNM,hNM15);
    hNM=std::max(hNM,hNM16);
    hNM=std::max(hNM,hNM18);
    hNM=std::max(hNM,hNM19);

    hNL=std::max(hNL,hNL);//hNL


///DEFUZZYFICATION

    float SUM_Ai_Xi=CALC_AREA(hPL)*PL+CALC_AREA(hPM)*PM+CALC_AREA(hPS)*PS+CALC_AREA(hZE)*ZE+CALC_AREA(hNS)*NS+CALC_AREA(hNM)*NM+CALC_AREA(hNL)*NL;
    std::cout<<"sum(Ai*Xi) = "<<SUM_Ai_Xi<<"\n";
    float SUM_AI=CALC_AREA(hPL)+CALC_AREA(hPM)+CALC_AREA(hPS)+CALC_AREA(hZE)+CALC_AREA(hNS)+CALC_AREA(hNM)+CALC_AREA(hNL);
    std::cout<<"sum(Ai) = "<<SUM_AI<<"\n";
    if(SUM_AI!=0)
        return SUM_Ai_Xi/SUM_AI;
    else
        return 0;
}
